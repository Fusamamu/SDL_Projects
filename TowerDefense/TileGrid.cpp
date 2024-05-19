#include "TileGrid.h"
#include "Application.h"
#include <queue>
#include <iostream>
#include <unordered_map>

TileGrid::TileGrid()
{

}

TileGrid::TileGrid(std::size_t  _rowCount, std::size_t _columnCount):
RowCount(_rowCount),
ColumnCount(_columnCount),
Tiles(_rowCount * _columnCount),
TileUnitSize(64)
{
    Application& _app = Application::GetInstance();

    for(std::size_t _i = 0; _i < RowCount; ++_i)
        for(std::size_t _j = 0; _j < ColumnCount; ++_j)
        {
            Tile* _tile = new Tile();

            _tile->RowIdx    = _i;
            _tile->ColumnIdx = _j;

            int _targetPosY = _i * TileUnitSize + _app.SCREEN_HEIGHT / 2 - GetTotalHeight()/2;
            int _targetPosX = _j * TileUnitSize + _app.SCREEN_WIDTH / 2 - GetTotalWidth()/2;

            _tile->Object = new GameObject();
            _tile->Object->Transform->Position.x = static_cast<float>(_targetPosX);
            _tile->Object->Transform->Position.y = static_cast<float>(_targetPosY);

            _tile->Object->Renderer->Texture = _app.GetTexture("TileSet");
            _tile->Object->Renderer->Src = { 0, 0, 128, 128 };
            _tile->Object->Renderer->Dest = {
                    _targetPosX,
                    _targetPosY,
                    TileUnitSize,
                    TileUnitSize};

            _tile->Object->Renderer->UsePrimitive = true;
            _tile->Object->Renderer->Color = { 20, 20, 20, 100 };

            _tile->Object->Renderer->RenderBound = true;
            _tile->Object->Renderer->BoundColor = { 100, 100, 100, 100};

            Tiles[_i * RowCount + _j] = _tile;
        }
}

TileGrid::~TileGrid()
{
    Tiles.clear();
}

Tile* TileGrid::GetTile(int _rowIdx, int _columnIdx)
{
    if(_rowIdx >= RowCount || _rowIdx < 0 || _columnIdx >= ColumnCount || _columnIdx < 0)
        return nullptr;

    return Tiles[_rowIdx * RowCount + _columnIdx];
}

std::vector<Tile*> TileGrid::GetNeighbors(Tile* _tile)
{
    std::vector<Tile*> _result;

    for(int _dx = -1; _dx <= 1; ++ _dx)
        for(int _dy = -1; _dy <= 1; ++_dy)
        {
            if(_dx == 0 && _dy == 0)
                continue;

            Tile* _t = GetTile(_tile->RowIdx + _dx, _tile->ColumnIdx + _dy);

            if(_t != nullptr && _t->IsWalkable)
                _result.push_back(_t);
        }

    return _result;
}

unsigned int TileGrid::GetTotalWidth() const
{
    return ColumnCount * TileUnitSize;
}

unsigned int TileGrid::GetTotalHeight() const
{
    return RowCount * TileUnitSize;
}

float TileGrid::Heuristic(Tile* _a, Tile* _b)
{
    return std::abs(_a->RowIdx - _b->RowIdx) + std::abs(_a->ColumnIdx - _b->RowIdx);
}

std::vector<Tile*> TileGrid::ReconstructPath(Tile* _endTile)
{
    std::vector<Tile*> _result;

    Tile* _currentTile = _endTile;

    while(_currentTile != nullptr)
    {
        _result.push_back(_currentTile);
        _currentTile = _currentTile->Parent;
    }

    std::reverse(_result.begin(), _result.end());

    return _result;
}

std::vector<Tile*> TileGrid::GetAStarPath(TileGrid& _tileGrid, Tile* _start, Tile* _end)
{
    auto _compare = [](Tile *_a, Tile *_b) {
        return _a->GetFCost() > _b->GetFCost();
    };

    std::priority_queue<Tile *, std::vector<Tile *>, decltype(_compare)> _openSet(_compare);
    std::unordered_map<Tile*, bool> _openSetMap;
    std::unordered_map<Tile*, bool> _closedSet;

    _start->GCost = 0;
    _start->HCost = Heuristic(_start, _end);

    _openSet.push(_start);
    _openSetMap[_start] = true;

    while(!_openSet.empty())
    {
        Tile* _current = _openSet.top();
        _openSet.pop();
        _openSetMap[_current] = false;

        if(*_current == *_end)
            return ReconstructPath(_current);

        _closedSet[_current] = true;

        for(Tile* _neighbor : _tileGrid.GetNeighbors(_current))
        {
            if(_closedSet.find(_neighbor) != _closedSet.end())
                continue;

            float _tentativeGCost = _current->GCost + Heuristic(_current, _neighbor);

            if(_tentativeGCost < _neighbor->GCost || _openSetMap.find(_neighbor) == _openSetMap.end())
            {
                _neighbor->GCost = _tentativeGCost;
                _neighbor->HCost = Heuristic(_neighbor, _end);
                _neighbor->Parent = _current;

                if(_openSetMap.find(_neighbor) == _openSetMap.end() || !_openSetMap[_neighbor])
                {
                    _openSet.push(_neighbor);
                    _openSetMap[_neighbor] = true;
                }
            }
        }
    }

    return {};
}

