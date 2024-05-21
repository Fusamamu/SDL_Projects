#include <iostream>
#include <queue>
#include <unordered_map>
#include <thread>
#include <chrono>
#include "TileGrid.h"
#include "Application.h"

//const char* MAP_1 =
//        "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
//        "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
//        "o o o o o o o o o o o o o o o - - - - - - - - - - -\n"
//        "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
//        "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
//        "- - - o - - - - - - o o o o o o o o o o o o o o - -\n"
//        "- - - o - - - - - - - - - - - - - - - - - - - - - -\n"
//        "- - - o o o o o o o o o o o o o o o - - - - - - - -\n"
//        "- - - o - - - - - - - - - - - - - - - - - - - - - -\n"
//        "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
//        "- - - - o o o o o o o o o o o o o o o - - - - - - -\n"
//        "- - - - - - - - o - - - - - - - - - - - - - - - - -\n"
//        "- - - - - - - - o - - - - - - - - - - - - - - - - -\n"
//        "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
//        "- - - o o o o o - - - - - - o - - - - - - - - - - -\n"
//        "- - - - - - - - - - - - - - o - - - - - - - - - - -\n"
//        "- - - - - - - - - o o o o o o o o o o o o o - - - -\n"
//        "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
//        "- - - - - - o - - - - - - - - - - - - - - - - - - -\n"
//        "- - - - - - o - - - - - - - - - - - - - - - - - - -\n"
//        "- - - - - - o - - - - - - - o o o o o o o - - - - -\n"
//        "- - - - - - o - - - - - - - - - - - - - - - - - - -\n"
//        "- - - - - - o - - - - - - - - - - - - - - - - - - -\n";

TileGrid::TileGrid():
TileUnitSize(32)
{

}

TileGrid::TileGrid(std::size_t  _rowCount, std::size_t _columnCount):
RowCount(_rowCount),
ColumnCount(_columnCount),
Tiles(_rowCount * _columnCount),
TileUnitSize(32)
{
    //LoadMap(MAP_1);
}

TileGrid::~TileGrid()
{
    Tiles.clear();
}

void TileGrid::LoadMap(const char* _mapData, Scene* _scene)
{
    bool _completeFirstLine = false;

    int _rowCount    = 0;
    int _columnCount = 0;

    const char* _current = _mapData;

    while(*_current != '\0')
    {
        if(!_completeFirstLine)
        {
            if(*_current != ' ' && *_current != '\n')
                _columnCount++;

            if(*_current == '\n')
                _completeFirstLine = true;
        }

        if(*_current == '\n')
            _rowCount++;

        _current++;
    }

    Tiles.clear();
    Tiles.reserve(_rowCount * _columnCount);

    RowCount    = _rowCount;
    ColumnCount = _columnCount;

    const char* _tempMap = _mapData;

    Application& _app = Application::GetInstance();

    std::size_t _i = 0;
    std::size_t _j = 0;

    while(*_tempMap != '\0')
    {
        if(*_tempMap == ' ')
        {
            _tempMap++;
            continue;
        }

        if(*_tempMap == '\n')
        {
            _i++;
            if(_i > _rowCount - 1)
                _i = _rowCount - 1;
            _j = 0;

            _tempMap++;
            continue;
        }

        Tile* _tile = new Tile();

        _tile->RowIdx    = _i;
        _tile->ColumnIdx = _j;

        int _targetPosY = _i * TileUnitSize + _app.SCREEN_HEIGHT / 2 - GetTotalHeight()/2;
        int _targetPosX = _j * TileUnitSize + _app.SCREEN_WIDTH / 2  - GetTotalWidth() /2;

        _tile->Object = new GameObject("Tile", _scene);
        _tile->Object->Transform->Position.x = static_cast<float>(_targetPosX);
        _tile->Object->Transform->Position.y = static_cast<float>(_targetPosY);

        _tile->Object->Renderer->Texture = _app.GetTexture("TileSet");

        if(*_tempMap == '-')
            _tile->Object->Renderer->Src = { 5 * 32, 3 * 32 , 32, 32 };
        if(*_tempMap == 'o')
        {
            _tile->IsWalkable = false;
            _tile->Object->Renderer->Src = { 5 * 32, 4 * 32 , 32, 32 };
        }

        _tile->Object->Renderer->Dest = {
                _targetPosX,
                _targetPosY,
                TileUnitSize,
                TileUnitSize};

        Tiles[_i * RowCount + _j] = _tile;

        _j++;
        if(_j > _columnCount - 1)
            _j = _columnCount - 1;
        _tempMap++;
    }
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

