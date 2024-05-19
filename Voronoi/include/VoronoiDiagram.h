#pragma once
#include <vector>
#include <list>
#include <unordered_set>
#include "Vector2.h"

namespace Voronoi
{
    struct Face;

    struct Vertex
    {
        Vector2 Point;
        std::list<Vertex>::iterator iter;
    };

    struct HalfEdge
    {
        Vertex* Origin = nullptr;
        Vertex* Dest   = nullptr;

        HalfEdge* Twin = nullptr;

        Face* IncidentFace = nullptr;
        HalfEdge* Prev = nullptr;
        HalfEdge* Next = nullptr;

        std::list<HalfEdge>::iterator iter;
    };

    struct Site
    {
        std::size_t Index;
        Vector2 Point;
        Face* Face;
    };

    struct Face
    {
        Site* Site;
        HalfEdge* OuterComponent;
    };

    struct Arc
    {
        enum class Color { RED, BLACK };

        Arc* Parent;
        Arc* Left;
        Arc* Right;

        Site* Site;
        HalfEdge* LeftHalfEdge;
        HalfEdge* RightHalfEdge;

        //Event*
        Arc* Prev;
        Arc* Next;

        Color Color;
    };

    struct VoronoiDiagram
    {
        std::vector<Site> Sites;
        std::vector<Face> Faces;

        std::list<Vertex>   Vertices;
        std::list<HalfEdge> HalfEdges;

        VoronoiDiagram(const std::vector<Vector2>& _points);

        Site* GetSite(std::size_t _index);
        Face* GetFace(std::size_t _index);

        Vertex* CreateVertex(Vector2 _point);
        HalfEdge* CreateHalfEdge(Face* face);
    };
}