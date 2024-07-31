#pragma once

class GameObject;
class Vector2;

class Tile
{
public:
    int RowIdx;
    int ColumnIdx;

    bool IsWalkable;

    float GCost, HCost;

    Tile* Parent;
    GameObject* Object;
public:
    Tile();
    ~Tile();

    Vector2 GetPosition();

    float GetFCost() const;
    bool operator==(const Tile& _tile) const;
};

namespace std
{
    template<>
    struct hash<Tile>
    {
        std::size_t operator()(const Tile& _tile) const
        {
            return std::hash<int>()(_tile.RowIdx) ^ std::hash<int>()(_tile.ColumnIdx);
        }
    };
}