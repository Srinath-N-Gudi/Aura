#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Block.h"

constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_HEIGHT = 64;

class World;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 color;
};

class Chunk {
public:
    Chunk(int x, int z, World* world);
    ~Chunk();

    void GenerateTerrain(int seed);
    void BuildMesh();
    void UploadMesh();
    void Render();

    BlockType GetBlock(int x, int y, int z) const;
    void SetBlock(int x, int y, int z, BlockType type);

    int GetChunkX() const { return m_ChunkX; }
    int GetChunkZ() const { return m_ChunkZ; }
    bool IsDirty() const { return m_Dirty; }
    bool HasMesh() const { return m_VAO != 0; }

    bool IsSolid(int x, int y, int z) const;

private:
    int m_ChunkX, m_ChunkZ;
    World* m_World;
    BlockType m_Blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    bool m_Dirty = true;

    GLuint m_VAO = 0, m_VBO = 0, m_IBO = 0;
    int m_IndexCount = 0;

    void AddFace(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                 glm::vec3 pos, const glm::vec3* faceVerts, const glm::vec3& normal,
                 BlockType type);

    bool IsBlockTransparent(int x, int y, int z) const;
};
