#pragma once
#include <string>
#include <unordered_map>

enum class BlockType : unsigned char {
    Air = 0,
    Grass,
    Dirt,
    Stone,
    Sand,
    Water,
    Wood,
    Leaves,
    Bedrock,
    CoalOre,
    IronOre,
    GoldOre,
    DiamondOre,
    Cobblestone,
    Planks,
    Glass,
    Brick,
    Snow,
    Clay,
    Gravel
};

struct BlockInfo {
    std::string name;
    float r, g, b;
    bool transparent;
    bool solid;
};

class BlockRegistry {
public:
    static void Init() {
        GetMap()[BlockType::Air]        = { "Air",        0.0f, 0.0f, 0.0f, true,  false };
        GetMap()[BlockType::Grass]      = { "Grass",      0.3f, 0.7f, 0.2f, false, true  };
        GetMap()[BlockType::Dirt]       = { "Dirt",       0.55f, 0.35f, 0.15f, false, true  };
        GetMap()[BlockType::Stone]      = { "Stone",      0.5f, 0.5f, 0.5f, false, true  };
        GetMap()[BlockType::Sand]       = { "Sand",       0.85f, 0.8f, 0.55f, false, true  };
        GetMap()[BlockType::Water]      = { "Water",      0.2f, 0.4f, 0.9f, true,  false };
        GetMap()[BlockType::Wood]       = { "Wood",       0.45f, 0.3f, 0.15f, false, true  };
        GetMap()[BlockType::Leaves]     = { "Leaves",     0.15f, 0.55f, 0.1f, true,  true  };
        GetMap()[BlockType::Bedrock]    = { "Bedrock",    0.2f, 0.2f, 0.2f, false, true  };
        GetMap()[BlockType::CoalOre]    = { "CoalOre",    0.3f, 0.3f, 0.3f, false, true  };
        GetMap()[BlockType::IronOre]    = { "IronOre",    0.6f, 0.55f, 0.5f, false, true  };
        GetMap()[BlockType::GoldOre]    = { "GoldOre",    0.85f, 0.75f, 0.2f, false, true  };
        GetMap()[BlockType::DiamondOre] = { "DiamondOre", 0.3f, 0.75f, 0.8f, false, true  };
        GetMap()[BlockType::Cobblestone]= { "Cobblestone",0.45f, 0.45f, 0.45f, false, true  };
        GetMap()[BlockType::Planks]     = { "Planks",     0.7f, 0.55f, 0.3f, false, true  };
        GetMap()[BlockType::Glass]      = { "Glass",      0.8f, 0.9f, 1.0f, true,  true  };
        GetMap()[BlockType::Brick]      = { "Brick",      0.6f, 0.3f, 0.2f, false, true  };
        GetMap()[BlockType::Snow]       = { "Snow",       0.95f, 0.95f, 0.95f, false, true  };
        GetMap()[BlockType::Clay]       = { "Clay",       0.65f, 0.6f, 0.55f, false, true  };
        GetMap()[BlockType::Gravel]     = { "Gravel",     0.55f, 0.5f, 0.5f, false, true  };
    }

    static const BlockInfo& Get(BlockType type) {
        std::unordered_map<BlockType, BlockInfo>& blocks = GetMap();
        std::unordered_map<BlockType, BlockInfo>::iterator it = blocks.find(type);
        if (it != blocks.end())
            return it->second;
        return blocks[BlockType::Air];
    }

    static bool IsTransparent(BlockType type) {
        return Get(type).transparent;
    }

    static bool IsSolid(BlockType type) {
        return Get(type).solid;
    }

    static void GetColor(BlockType type, float& r, float& g, float& b) {
        const BlockInfo& info = Get(type);
        r = info.r;
        g = info.g;
        b = info.b;
    }

private:
    static std::unordered_map<BlockType, BlockInfo>& GetMap() {
        static std::unordered_map<BlockType, BlockInfo> blocks;
        return blocks;
    }
};
