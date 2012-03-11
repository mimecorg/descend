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

#include "surfacetessellator.h"

#include <QVector2D>
#include <QVector3D>
#include <QSize>
#include <QVarLengthArray>
#include <qmath.h>

SurfaceTessellator::SurfaceTessellator()
{
}

SurfaceTessellator::~SurfaceTessellator()
{
}

// Calculate the distance from point v2 to the line going through v0 and v1 relative to the distance from v0 to v1.
static inline float delta( const QVector3D& v0, const QVector3D& v1, const QVector3D& v2 )
{
    QVector3D line = v1 - v0;
    float length = line.length();
    return v2.distanceToLine( v0, line / length ) / length;
}

void SurfaceTessellator::tessellate( AbstractSurface* surface, int minOrder, int maxOrder, float alpha )
{
    float epsilon = 0.1f / (float)( 1 << ( maxOrder / 2 ) );

    float minDot = cosf( alpha * 3.14159f / 180.0f );
    float maxDelta = 0.5f * tanf( alpha * 3.14159f / 180.0f );

    initializeMesh( surface, epsilon );

    int remaining = m_edges.count();

    for ( int order = 0; order < maxOrder - 1 && remaining > 0; order++ ) {
        int count = m_edges.count();

        for ( int e0 = 0; e0 < count; e0++ ) {
            if ( m_edges[ e0 ].m_order != order )
                continue;

            remaining--;

            int v0 = m_edges[ e0 ].m_v[ 0 ];
            int v1 = m_edges[ e0 ].m_v[ 1 ];

            // calculate a new vertex in the middle of the edge
            Vertex vertex;
            calculateVertex( surface, &vertex, 0.5f * ( m_vertices[ v0 ].m_param + m_vertices[ v1 ].m_param ), epsilon );

            // if minimum order is reached, check the angle between the normals and the distance between the vertex and the edge
            if ( order < minOrder
                 || QVector3D::dotProduct( vertex.m_normal, m_vertices[ v0 ].m_normal ) < minDot
                 || QVector3D::dotProduct( vertex.m_normal, m_vertices[ v1 ].m_normal ) < minDot
                 || delta( m_vertices[ v0 ].m_coord, m_vertices[ v1 ].m_coord, vertex.m_coord ) > maxDelta ) {

                // add the new vertex
                int v2 = m_vertices.count();
                m_vertices.append( vertex );

                // divide the edge
                Edge edge;
                edge.m_order = order + 2;
                edge.m_v[ 0 ] = v2;
                edge.m_v[ 1 ] = v1;
                edge.m_f[ 0 ] = m_edges[ e0 ].m_f[ 0 ];
                edge.m_f[ 1 ] = m_edges[ e0 ].m_f[ 1 ];

                int e1 = m_edges.count();
                m_edges.append( edge );

                m_edges[ e0 ].m_order = order + 2;
                m_edges[ e0 ].m_v[ 1 ] = v2;

                remaining++;

                // process adjacent faces
                for ( int i = 0; i < 2; i++ ) {
                    int f0 = m_edges[ e0 ].m_f[ i ];

                    if ( f0 < 0 )
                        continue;

                    // find the index of the common edge
                    int j0 = -1;
                    for ( int j = 0; j < 3; j++ ) {
                        if ( m_faces[ f0 ].m_e[ j ] == e0 ) {
                            j0 = j;
                            break;
                        }
                    }

                    Q_ASSERT( j0 >= 0 );

                    // next and previous edge in CW direction
                    int j3 = ( j0 + 2 - i ) % 3;
                    int j4 = ( j0 + 1 + i ) % 3;

                    int e3 = m_faces[ f0 ].m_e[ j3 ];
                    int e4 = m_faces[ f0 ].m_e[ j4 ];

                    // third vertex of the face
                    int v3;
                    if ( i == 0 )
                        v3 = m_edges[ e3 ].m_v[ m_faces[ f0 ].m_dir[ j3 ] ];
                    else
                        v3 = m_edges[ e4 ].m_v[ m_faces[ f0 ].m_dir[ j4 ] ];

                    // add new edge and adjust two other edges
                    int f1 = m_faces.count();

                    edge.m_order = order + 2;
                    edge.m_v[ 0 ] = v2;
                    edge.m_v[ 1 ] = v3;
                    edge.m_f[ i ] = f0;
                    edge.m_f[ 1 - i ] = f1;

                    int e2 = m_edges.count();
                    m_edges.append( edge );

                    m_edges[ e1 ].m_f[ i ] = f1;
                    m_edges[ e4 ].m_f[ m_faces[ f0 ].m_dir[ j4 ] ] = f1;

                    remaining++;

                    // divide the face
                    Face face;
                    face.m_e[ j0 ] = e1;
                    face.m_dir[ j0 ] = i;
                    face.m_e[ j3 ] = e2;
                    face.m_dir[ j3 ] = 1 - i;
                    face.m_e[ j4 ] = e4;
                    face.m_dir[ j4 ] = m_faces[ f0 ].m_dir[ j4 ];

                    m_faces.append( face );

                    m_faces[ f0 ].m_e[ j4 ] = e2;
                    m_faces[ f0 ].m_dir[ j4 ] = i;
                }
            }
        }
    }
}

// Create an initial mesh consisting of 4 vertices and 2 faces.
void SurfaceTessellator::initializeMesh( AbstractSurface* surface, float epsilon )
{
    Vertex v0, v1, v2, v3;

    calculateVertex( surface, &v0, QVector2D( 0.0f, 0.0f ), epsilon );
    calculateVertex( surface, &v1, QVector2D( 1.0f, 0.0f ), epsilon );
    calculateVertex( surface, &v2, QVector2D( 0.0f, 1.0f ), epsilon );
    calculateVertex( surface, &v3, QVector2D( 1.0f, 1.0f ), epsilon );

    m_vertices.append( v0 );
    m_vertices.append( v1 );
    m_vertices.append( v2 );
    m_vertices.append( v3 );

    Edge e0 = { 1, { 0, 1 }, { 0, -1 } };
    Edge e1 = { 1, { 2, 3 }, { -1, 1 } };
    Edge e2 = { 1, { 0, 2 }, { -1, 1 } };
    Edge e3 = { 1, { 1, 3 }, { 0, -1 } };
    Edge e4 = { 0, { 0, 3 }, { 1, 0 } };

    m_edges.append( e0 );
    m_edges.append( e1 );
    m_edges.append( e2 );
    m_edges.append( e3 );
    m_edges.append( e4 );

    Face f0 = { { 0, 3, 4 }, { 0, 0, 1 } };
    Face f1 = { { 4, 1, 2 }, { 0, 1, 1 } };

    m_faces.append( f0 );
    m_faces.append( f1 );
}

// The same as QVector3D::normal(), but it doesn't round the vector down to zero for very small distances.
static QVector3D preciseNormal( const QVector3D& v1, const QVector3D& v2, const QVector3D& v3 )
{
    QVector3D v = QVector3D::crossProduct( ( v2 - v1 ), ( v3 - v1 ) );

    return v / qSqrt( (double)v.x() * (double)v.x() + (double)v.y() * (double)v.y() + (double)v.z() * (double)v.z() );
}

void SurfaceTessellator::calculateVertex( AbstractSurface* surface, Vertex* vertex, const QVector2D& param, float epsilon )
{
    vertex->m_param = param;
    vertex->m_coord = surface->vertexFromParameter( param );

    // calculate two samples with small offset from original vertex
    QVector2D p1 = param + QVector2D( epsilon, 0.0f );
    QVector2D p2 = param + QVector2D( 0.0f, epsilon );

    QVector3D v1 = surface->vertexFromParameter( p1 );
    QVector3D v2 = surface->vertexFromParameter( p2 );

    bool eq1 = qFuzzyCompare( vertex->m_coord, v1 );
    bool eq2 = qFuzzyCompare( vertex->m_coord, v2 );

    if ( eq1 || eq2 ) {
        // if one of the points coincides with the vertex, calculate a third sample
        QVector2D p3 = p1 + QVector2D( 0.0f, epsilon );
        QVector3D v3 = surface->vertexFromParameter( p3 );

        vertex->m_normal = eq1 ? preciseNormal( vertex->m_coord, v3, v2 ) : preciseNormal( vertex->m_coord, v1, v3 );
    } else {
        vertex->m_normal = preciseNormal( vertex->m_coord, v1, v2 );
    }
}

QVector<QVector3D> SurfaceTessellator::vertices()
{
    QVector<QVector3D> result;
    result.resize( m_vertices.count() );

    for ( int i = 0; i < m_vertices.count(); i++ )
        result[ i ] = m_vertices[ i ].m_coord;

    return result;
}

QVector<QVector3D> SurfaceTessellator::normals()
{
    QVector<QVector3D> result;
    result.resize( m_vertices.count() );

    for ( int i = 0; i < m_vertices.count(); i++ )
        result[ i ] = m_vertices[ i ].m_normal;

    return result;
}

QVector<Triangle> SurfaceTessellator::indices()
{
    QVector<Triangle> result;
    result.resize( m_faces.count() );

    for ( int i = 0; i < m_faces.count(); i++ ) {
        for ( int j = 0; j < 3; j++ )
            result[ i ].m_v[ j ] = m_edges[ m_faces[ i ].m_e[ j ] ].m_v[ m_faces[ i ].m_dir[ j ] ];
    }

    return result;
}
