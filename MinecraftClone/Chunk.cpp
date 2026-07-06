#include "Chunk.h"
#include "World.h"
#include "SimplexNoise.h"
#include <cmath>
#include <algorithm>
#include <cstddef>

// Face vertex data: 6 faces, 4 vertices each
// Order: Front(+Z), Back(-Z), Top(+Y), Bottom(-Y), Right(+X), Left(-X)
constexpr glm::vec3 FACE_VERTICES[6][4] = {
    // Front (+Z)
    {{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}},
    // Back (-Z)
    {{1,0,0}, {0,0,0}, {0,1,0}, {1,1,0}},
    // Top (+Y)
    {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}},
    // Bottom (-Y)
    {{0,0,0}, {1,0,0}, {1,0,1}, {0,0,1}},
    // Right (+X)
    {{1,0,1}, {1,0,0}, {1,1,0}, {1,1,1}},
    // Left (-X)
    {{0,0,0}, {0,0,1}, {0,1,1}, {0,1,0}}
};

constexpr glm::vec3 FACE_NORMALS[6] = {
    { 0, 0, 1}, { 0, 0,-1},
    { 0, 1, 0}, { 0,-1, 0},
    { 1, 0, 0}, {-1, 0, 0}
};

constexpr unsigned int FACE_INDICES[6] = { 0, 1, 2, 2, 3, 0 };

Chunk::Chunk(int x, int z, World* world)
    : m_ChunkX(x), m_ChunkZ(z), m_World(world) {
    for (int x = 0; x < CHUNK_SIZE; x++)
        for (int y = 0; y < CHUNK_HEIGHT; y++)
            for (int z = 0; z < CHUNK_SIZE; z++)
                m_Blocks[x][y][z] = BlockType::Air;
}

Chunk::~Chunk() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_IBO) glDeleteBuffers(1, &m_IBO);
}

void Chunk::GenerateTerrain(int seed) {
    SimplexNoise noise(seed);
    SimplexNoise detailNoise(seed + 1000);

    int worldX = m_ChunkX * CHUNK_SIZE;
    int worldZ = m_ChunkZ * CHUNK_SIZE;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            double wx = (worldX + x) * 0.01;
            double wz = (worldZ + z) * 0.01;

            double heightVal = noise.octave2D(wx, wz, 6, 0.5, 2.0);
            double detail = detailNoise.octave2D(wx * 4, wz * 4, 3, 0.5, 2.0) * 0.1;

            int height = (int)((heightVal + detail) * 20 + 32);
            height = std::max(1, std::min(CHUNK_HEIGHT - 2, height));

            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                if (y == 0) {
                    m_Blocks[x][y][z] = BlockType::Bedrock;
                } else if (y < height - 4) {
                    m_Blocks[x][y][z] = BlockType::Stone;

                    // Ores
                    double oreNoise = noise.noise3D(
                        (worldX + x) * 0.1,
                        y * 0.1,
                        (worldZ + z) * 0.1
                    );
                    if (y < 16 && oreNoise > 0.7)
                        m_Blocks[x][y][z] = BlockType::DiamondOre;
                    else if (y < 32 && oreNoise > 0.6)
                        m_Blocks[x][y][z] = BlockType::GoldOre;
                    else if (y < 48 && oreNoise > 0.5)
                        m_Blocks[x][y][z] = BlockType::IronOre;
                    else if (oreNoise > 0.4)
                        m_Blocks[x][y][z] = BlockType::CoalOre;
                } else if (y < height) {
                    m_Blocks[x][y][z] = BlockType::Dirt;
                } else if (y == height) {
                    if (height < 28)
                        m_Blocks[x][y][z] = BlockType::Sand;
                    else if (height > 50)
                        m_Blocks[x][y][z] = BlockType::Snow;
                    else
                        m_Blocks[x][y][z] = BlockType::Grass;
                } else if (y <= 26) {
                    m_Blocks[x][y][z] = BlockType::Water;
                }
            }

            // Trees
            if (height > 30 && height < 48) {
                double treeNoise = noise.noise2D(
                    (worldX + x) * 0.5,
                    (worldZ + z) * 0.5
                );
                if (treeNoise > 0.6 && x > 2 && x < CHUNK_SIZE - 3 && z > 2 && z < CHUNK_SIZE - 3) {
                    int treeBase = height + 1;
                    int treeHeight = 5 + (int)(treeNoise * 3);

                    // Trunk
                    for (int ty = treeBase; ty < treeBase + treeHeight && ty < CHUNK_HEIGHT; ty++)
                        m_Blocks[x][ty][z] = BlockType::Wood;

                    // Leaves
                    for (int lx = -2; lx <= 2; lx++)
                        for (int ly = -1; ly <= 2; ly++)
                            for (int lz = -2; lz <= 2; lz++) {
                                int bx = x + lx, by = treeBase + treeHeight + ly, bz = z + lz;
                                if (bx >= 0 && bx < CHUNK_SIZE && bz >= 0 && bz < CHUNK_SIZE && by < CHUNK_HEIGHT) {
                                    if (m_Blocks[bx][by][bz] == BlockType::Air) {
                                        if (std::abs(lx) + std::abs(lz) + std::abs(ly) < 4)
                                            m_Blocks[bx][by][bz] = BlockType::Leaves;
                                    }
                                }
                            }
                }
            }
        }
    }
    m_Dirty = true;
}

void Chunk::BuildMesh() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                BlockType block = m_Blocks[x][y][z];
                if (block == BlockType::Air) continue;

                glm::vec3 pos((float)x, (float)y, (float)z);

                for (int face = 0; face < 6; face++) {
                    int nx = x + (int)FACE_NORMALS[face].x;
                    int ny = y + (int)FACE_NORMALS[face].y;
                    int nz = z + (int)FACE_NORMALS[face].z;

                    if (IsBlockTransparent(nx, ny, nz)) {
                        AddFace(vertices, indices, pos, FACE_VERTICES[face],
                               FACE_NORMALS[face], block);
                    }
                }
            }
        }
    }

    m_IndexCount = (int)indices.size();
    if (m_IndexCount == 0) return;

    // Create/Update VAO
    if (!m_VAO) {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_IBO);
    }

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // TexCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    // Color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    m_Dirty = false;
}

void Chunk::Render() {
    if (m_IndexCount == 0 || !m_VAO) return;
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

BlockType Chunk::GetBlock(int x, int y, int z) const {
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
        return BlockType::Air;
    return m_Blocks[x][y][z];
}

void Chunk::SetBlock(int x, int y, int z, BlockType type) {
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
        return;
    m_Blocks[x][y][z] = type;
    m_Dirty = true;
}

bool Chunk::IsSolid(int x, int y, int z) const {
    return BlockRegistry::IsSolid(GetBlock(x, y, z));
}

bool Chunk::IsBlockTransparent(int x, int y, int z) const {
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE) {
        // Check world for cross-chunk transparency
        if (m_World) {
            int worldX = m_ChunkX * CHUNK_SIZE + x;
            int worldZ = m_ChunkZ * CHUNK_SIZE + z;
            return BlockRegistry::IsTransparent(m_World->GetBlock(worldX, y, worldZ));
        }
        return true;
    }
    return BlockRegistry::IsTransparent(m_Blocks[x][y][z]);
}

void Chunk::AddFace(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                    glm::vec3 pos, const glm::vec3* faceVerts, const glm::vec3& normal,
                    BlockType type) {
    float r, g, b;
    BlockRegistry::GetColor(type, r, g, b);
    glm::vec3 blockColor(r, g, b);

    // Darken bottom faces for ambient occlusion effect
    if (normal.y < 0) {
        blockColor *= 0.7f;
    } else if (normal.x != 0 || normal.z != 0) {
        blockColor *= 0.85f;
    }

    unsigned int baseIndex = (unsigned int)vertices.size();

    for (int i = 0; i < 4; i++) {
        Vertex v;
        v.position = pos + faceVerts[i];
        v.normal = normal;
        v.texCoord = glm::vec2(faceVerts[i].x + faceVerts[i].z, faceVerts[i].y);
        v.color = blockColor;
        vertices.push_back(v);
    }

    indices.push_back(baseIndex + 0);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
    indices.push_back(baseIndex + 0);
}
