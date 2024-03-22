#pragma once
#include <memory>
#include "BBox.h"

using NODE_ID = std::uint32_t;

static constexpr NODE_ID null = NODE_ID(-1);

static constexpr std::size_t MAX_QUAD_TREE_DEPTH = 5;

struct QuadTree
{
    struct Node
    {
        NODE_ID Children[2][2]
        {
            {null, null },
            {null, null }
        };
    };

    BBox Box;
    NODE_ID Root;
    std::vector<Node> Nodes;
    std::vector<BBox> Boxes;
};

template<typename Iterator>
NODE_ID Build_Impl(QuadTree& _quadTree, const BBox& _box, Iterator _begin, Iterator _end, std::size_t _depthLimit)
{
    if(_begin == _end)
        return null;

    NODE_ID _result = _quadTree.Nodes.size();
    _quadTree.Nodes.emplace_back();

    if(_begin + 1 == _end)
        return _result;

    if(_depthLimit == 0)
        return _result;

    Point _center = Middle(_box.Min, _box.Max);

    Iterator _split_y       = std::partition(_begin, _end    , [_center](const Point& _p) { return _p.Y < _center.Y; } );
    Iterator _split_x_lower = std::partition(_begin, _split_y, [_center](const Point& _p) { return _p.X < _center.X; } );
    Iterator _split_x_upper = std::partition(_split_y, _end  , [_center](const Point& _p) { return _p.X < _center.X; } );

    BBox SE_Box { _box.Min, _center };
    BBox SW_Box { { _center.X, _box.Min.Y }, { _box.Max.X, _center.Y } };
    BBox NE_Box { { _box.Min.X, _center.Y }, { _center.X, _box.Max.Y } };
    BBox NW_Box { _center, _box.Max };

    _quadTree.Boxes.push_back(SE_Box);
    _quadTree.Boxes.push_back(SW_Box);
    _quadTree.Boxes.push_back(NE_Box);
    _quadTree.Boxes.push_back(NW_Box);

    _quadTree.Nodes[_result].Children[0][0] = Build_Impl(_quadTree, SE_Box, _begin, _split_x_lower  , _depthLimit - 1);
    _quadTree.Nodes[_result].Children[0][1] = Build_Impl(_quadTree, SW_Box, _split_x_lower, _split_y, _depthLimit - 1);
    _quadTree.Nodes[_result].Children[1][0] = Build_Impl(_quadTree, NE_Box, _split_y, _split_x_upper, _depthLimit - 1);
    _quadTree.Nodes[_result].Children[1][1] = Build_Impl(_quadTree, NW_Box, _split_x_upper, _end    , _depthLimit - 1);

    return _result;
}

template<typename Iterator>
QuadTree Build(Iterator _begin, Iterator _end)
{
    QuadTree _result;
    _result.Root = Build_Impl(_result, ComputeBBox(_begin, _end), _begin, _end);
    return _result;
}

template<typename Iterator>
void BuildQuadTree(QuadTree& _quadTree, Iterator _begin, Iterator _end)
{
    _quadTree.Root = Build_Impl(_quadTree, _quadTree.Box, _begin, _end, MAX_QUAD_TREE_DEPTH);
}

