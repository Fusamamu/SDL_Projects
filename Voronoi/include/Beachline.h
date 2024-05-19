#pragma once
#include "VoronoiDiagram.h"

namespace Voronoi
{
    struct Beachline
    {
        Arc* Root;

        Beachline();
        ~Beachline();

        Arc* CreateArc(Site* _site);
        Arc* GetLeftMostArc() const;
        Arc* LocateArcAbove(const Vector2& _point, double _l) const;

        void InsertBefore(Arc* _x, Arc* _y);
        void InsertAfter (Arc* _x, Arc* _y);
        void Replace(Arc* _x, Arc* _y);
        void Remove(Arc* _z);
    };
}