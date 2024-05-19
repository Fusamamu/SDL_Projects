#include "VoronoiDiagram.h"

namespace Voronoi
{
    VoronoiDiagram::VoronoiDiagram(const std::vector<Vector2> &_points)
    {
        Sites.reserve(_points.size());
        Faces.reserve(_points.size());

        for(std::size_t _i = 0; _i < _points.size(); ++_i)
        {
            Sites.push_back({_i, _points[_i], nullptr});
            Faces.push_back({&Sites.back(), nullptr});
            Sites.back().Face = &Faces.back();
        }
    }

    Vertex* VoronoiDiagram::CreateVertex(Voronoi::Vector2 _point)
    {
        Vertices.emplace_back();
        Vertices.back().Point = _point;
        Vertices.back().iter  = std::prev(Vertices.end());
        return &Vertices.back();
    }

    HalfEdge* VoronoiDiagram::CreateHalfEdge(Voronoi::Face *_face)
    {
        HalfEdges.emplace_back();
        HalfEdges.back().IncidentFace = _face;
        HalfEdges.back().iter = std::prev(HalfEdges.end());

        if(_face->OuterComponent != nullptr)
            _face->OuterComponent = &HalfEdges.back();

        return &HalfEdges.back();
    }
}