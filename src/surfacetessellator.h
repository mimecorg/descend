/**************************************************************************
* This file is part of the Descend program
* Copyright (C) 2012 Michał Męciński
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#ifndef SURFACETESSELLATOR_H
#define SURFACETESSELLATOR_H

#include <QVector>

class QVector2D;
class QVector3D;
class QSize;

class AbstractSurface
{
public:
    virtual QVector3D vertexFromParameter( const QVector2D& parameter ) = 0;
};

struct Triangle
{
    int m_v[ 3 ];
};

class SurfaceTessellator
{
public:
    SurfaceTessellator();
    ~SurfaceTessellator();

public:
    void tessellate( AbstractSurface* surface, int minOrder, int maxOrder, float alpha );

    QVector<QVector3D> vertices();
    QVector<QVector3D> normals();
    QVector<Triangle> indices();

private:
    struct Vertex
    {
        QVector2D m_param;
        QVector3D m_coord;
        QVector3D m_normal;
    };

    struct Edge
    {
        // division order of the edge
        int m_order;
        // start and end vertices of the edge
        int m_v[ 2 ];
        // faces sharing this edge in CW and reversed order or -1
        int m_f[ 2 ];
    };

    struct Face
    {
        // edges of the face in CW order
        int m_e[ 3 ];
        // direction of edges (index of first vertex in CW order)
        int m_dir[ 3 ];
    };

private:
    void initializeMesh( AbstractSurface* surface, float epsilon );

    void calculateVertex( AbstractSurface* surface, Vertex* vertex, const QVector2D& param, float epsilon );

private:
    QVector<Vertex> m_vertices;
    QVector<Edge> m_edges;
    QVector<Face> m_faces;
};

#endif
