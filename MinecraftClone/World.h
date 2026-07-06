#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "Chunk.h"
#include "Block.h"
#include "SimplexNoise.h"

struct ChunkKey {
    int x, z;
    bool operator==(const ChunkKey& other) const { return x == other.x && z == other.z; }
};

struct ChunkKeyHash {
    size_t operator()(const ChunkKey& k) const {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.z) << 16);
    }
};

class World {
public:
    World(int renderDistance = 8, int seed = 12345);
    ~World();

    void Update(glm::vec3 playerPos);
    void Render();

    BlockType GetBlock(int x, int y, int z) const;
    void SetBlock(int x, int y, int z, BlockType type);

    Chunk* GetChunk(int chunkX, int chunkZ) const;
    Chunk* GetChunkAtBlock(int x, int z) const;

    bool Raycast(glm::vec3 origin, glm::vec3 direction, float maxDist,
                 glm::vec3& hitPos, glm::vec3& prevPos);

    int GetSeed() const { return m_Seed; }
    int GetRenderDistance() const { return m_RenderDistance; }

private:
    int m_Seed;
    int m_RenderDistance;
    std::unordered_map<ChunkKey, std::unique_ptr<Chunk>, ChunkKeyHash> m_Chunks;

    ChunkKey GetChunkCoord(int x, int z) const;
    void LoadChunksAround(int centerChunkX, int centerChunkZ);
    void UnloadDistantChunks(int centerChunkX, int centerChunkZ);
};
