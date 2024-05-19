#pragma once
#include <vector>
#include "Tile.h"

class TileGrid
{
public:
    int RowCount;
    int ColumnCount;

    std::vector<Tile*> Tiles;

    float OffsetX, OffsetY;

    int TileUnitSize;
public:
    TileGrid(std::size_t _rowCount, std::size_t _columnCount);
    TileGrid();
    ~TileGrid();

    Tile* GetTile(int _rowIdx, int _columnIdx);
    std::vector<Tile*> GetNeighbors(Tile* _tile);

    unsigned int GetTotalWidth() const;
    unsigned int GetTotalHeight() const;

    static float Heuristic(Tile* _a, Tile* _b);
    static std::vector<Tile*> ReconstructPath(Tile* _endTile);
    static std::vector<Tile*> GetAStarPath(TileGrid& _tileGrid, Tile* _start, Tile* _end);
};