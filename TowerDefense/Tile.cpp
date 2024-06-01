#include <iostream>
#include "include/GameObject.h"
#include "Tile.h"

Tile::Tile():
IsWalkable(true),
RowIdx(-1),
ColumnIdx(-1),
GCost(0),
HCost(0),
Parent(nullptr),
Object(new GameObject())
{

}

Tile::~Tile()
{
    delete Object;
}

Vector2 Tile::GetPosition()
{
    return Object->Transform->Position;
}

float Tile::GetFCost() const
{
    return GCost + HCost;
}

bool Tile::operator==(const Tile& _tile) const
{
    return RowIdx == _tile.RowIdx && ColumnIdx == _tile.ColumnIdx;
}