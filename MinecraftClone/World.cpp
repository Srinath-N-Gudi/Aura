#include "World.h"
#include <cmath>
#include <algorithm>
#include <glad/glad.h>

World::World(int renderDistance, int seed)
    : m_Seed(seed), m_RenderDistance(renderDistance) {
    BlockRegistry::Init();
}

World::~World() {
    m_Chunks.clear();
}

ChunkKey World::GetChunkCoord(int x, int z) const {
    ChunkKey key;
    key.x = x / CHUNK_SIZE;
    key.z = z / CHUNK_SIZE;
    return key;
}

Chunk* World::GetChunk(int chunkX, int chunkZ) const {
    ChunkKey key;
    key.x = chunkX;
    key.z = chunkZ;
    std::unordered_map<ChunkKey, std::unique_ptr<Chunk>, ChunkKeyHash>::const_iterator it = m_Chunks.find(key);
    return it != m_Chunks.end() ? it->second.get() : nullptr;
}

Chunk* World::GetChunkAtBlock(int x, int z) const {
    int cx = (x >= 0) ? x / CHUNK_SIZE : (x - CHUNK_SIZE + 1) / CHUNK_SIZE;
    int cz = (z >= 0) ? z / CHUNK_SIZE : (z - CHUNK_SIZE + 1) / CHUNK_SIZE;
    return GetChunk(cx, cz);
}

void World::Update(glm::vec3 playerPos) {
    int playerChunkX = (int)std::floor(playerPos.x / CHUNK_SIZE);
    int playerChunkZ = (int)std::floor(playerPos.z / CHUNK_SIZE);

    LoadChunksAround(playerChunkX, playerChunkZ);
    UnloadDistantChunks(playerChunkX, playerChunkZ);

    for (std::unordered_map<ChunkKey, std::unique_ptr<Chunk>, ChunkKeyHash>::iterator it = m_Chunks.begin();
         it != m_Chunks.end(); ++it) {
        if (it->second->IsDirty()) {
            it->second->BuildMesh();
        }
    }
}

void World::LoadChunksAround(int centerChunkX, int centerChunkZ) {
    for (int dx = -m_RenderDistance; dx <= m_RenderDistance; dx++) {
        for (int dz = -m_RenderDistance; dz <= m_RenderDistance; dz++) {
            if (dx * dx + dz * dz > m_RenderDistance * m_RenderDistance)
                continue;

            int cx = centerChunkX + dx;
            int cz = centerChunkZ + dz;
            ChunkKey key;
            key.x = cx;
            key.z = cz;

            if (m_Chunks.find(key) == m_Chunks.end()) {
                std::unique_ptr<Chunk> chunk(new Chunk(cx, cz, this));
                chunk->GenerateTerrain(m_Seed);
                chunk->BuildMesh();
                m_Chunks[key] = std::move(chunk);
            }
        }
    }
}

void World::UnloadDistantChunks(int centerChunkX, int centerChunkZ) {
    int unloadDist = m_RenderDistance + 3;
    std::vector<ChunkKey> toRemove;

    for (std::unordered_map<ChunkKey, std::unique_ptr<Chunk>, ChunkKeyHash>::iterator it = m_Chunks.begin();
         it != m_Chunks.end(); ++it) {
        int dx = it->first.x - centerChunkX;
        int dz = it->first.z - centerChunkZ;
        if (dx * dx + dz * dz > unloadDist * unloadDist)
            toRemove.push_back(it->first);
    }

    for (size_t i = 0; i < toRemove.size(); i++)
        m_Chunks.erase(toRemove[i]);
}

void World::Render() {
    for (std::unordered_map<ChunkKey, std::unique_ptr<Chunk>, ChunkKeyHash>::iterator it = m_Chunks.begin();
         it != m_Chunks.end(); ++it) {
        if (it->second->HasMesh())
            it->second->Render();
    }
}

BlockType World::GetBlock(int x, int y, int z) const {
    if (y < 0 || y >= CHUNK_HEIGHT) return BlockType::Air;

    Chunk* chunk = GetChunkAtBlock(x, z);
    if (!chunk) return BlockType::Air;

    int localX = ((x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    int localZ = ((z % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    return chunk->GetBlock(localX, y, localZ);
}

void World::SetBlock(int x, int y, int z, BlockType type) {
    if (y < 0 || y >= CHUNK_HEIGHT) return;

    Chunk* chunk = GetChunkAtBlock(x, z);
    if (!chunk) return;

    int localX = ((x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    int localZ = ((z % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    chunk->SetBlock(localX, y, localZ, type);

    if (localX == 0)          { Chunk* c = GetChunk(chunk->GetChunkX()-1, chunk->GetChunkZ()); if(c) { BlockType bt = c->GetBlock(CHUNK_SIZE-1, y, localZ); c->SetBlock(CHUNK_SIZE-1, y, localZ, bt); } }
    if (localX == CHUNK_SIZE-1) { Chunk* c = GetChunk(chunk->GetChunkX()+1, chunk->GetChunkZ()); if(c) { BlockType bt = c->GetBlock(0, y, localZ); c->SetBlock(0, y, localZ, bt); } }
    if (localZ == 0)          { Chunk* c = GetChunk(chunk->GetChunkX(), chunk->GetChunkZ()-1); if(c) { BlockType bt = c->GetBlock(localX, y, CHUNK_SIZE-1); c->SetBlock(localX, y, CHUNK_SIZE-1, bt); } }
    if (localZ == CHUNK_SIZE-1) { Chunk* c = GetChunk(chunk->GetChunkX(), chunk->GetChunkZ()+1); if(c) { BlockType bt = c->GetBlock(localX, y, 0); c->SetBlock(localX, y, 0, bt); } }
}

bool World::Raycast(glm::vec3 origin, glm::vec3 direction, float maxDist,
                    glm::vec3& hitPos, glm::vec3& prevPos) {
    direction = glm::normalize(direction);

    float t = 0.0f;
    float step = 0.05f;
    glm::vec3 prev = origin;

    while (t < maxDist) {
        glm::vec3 checkPos = origin + direction * t;
        int bx = (int)std::floor(checkPos.x);
        int by = (int)std::floor(checkPos.y);
        int bz = (int)std::floor(checkPos.z);

        BlockType block = GetBlock(bx, by, bz);
        if (block != BlockType::Air && block != BlockType::Water) {
            hitPos = glm::vec3((float)bx, (float)by, (float)bz);
            prevPos = prev;
            return true;
        }

        prev = glm::vec3((float)bx, (float)by, (float)bz);
        t += step;
    }
    return false;
}
