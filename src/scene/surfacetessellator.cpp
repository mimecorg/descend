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

#include "scene/surfacetessellator.h"

#include "scene/parametricmeshnode.h"

SurfaceTessellator::SurfaceTessellator()
{
}

SurfaceTessellator::~SurfaceTessellator()
{
}

void SurfaceTessellator::clear()
{
    m_vertices.clear();
    m_edges.clear();
    m_faces.clear();
}

bool SurfaceTessellator::tessellate( ParametricMeshNode* node )
{
    clear();

    if ( !initialize( node ) )
        return false;

    float dotThreshold = 0.5f / sqrtf( 0.25f + m_geometryThreshold * m_geometryThreshold );

    int remaining = m_edges.count();

    for ( int lod = 0; lod < m_lodMax - 1 && remaining > 0; lod++ ) {
        int count = m_edges.count();

        for ( int e0 = 0; e0 < count; e0++ ) {
            if ( m_edges[ e0 ].m_lod != lod )
                continue;

            remaining--;

            int v0 = m_edges[ e0 ].m_v[ 0 ];
            int v1 = m_edges[ e0 ].m_v[ 1 ];

            // calculate a new vertex in the middle of the edge
            Vertex vertex;
            if ( !calculateVertex( node, &vertex, 0.5f * ( m_vertices[ v0 ].m_param + m_vertices[ v1 ].m_param ) ) )
                return false;

            // if minimum lod is reached, check the angle between the normals and the distance between the vertex and the edge
            if ( lod < m_lodMin
                 || QVector3D::dotProduct( vertex.m_normal, m_vertices[ v0 ].m_normal ) < dotThreshold
                 || QVector3D::dotProduct( vertex.m_normal, m_vertices[ v1 ].m_normal ) < dotThreshold
                 || delta( m_vertices[ v0 ].m_pos, m_vertices[ v1 ].m_pos, vertex.m_pos ) > m_geometryThreshold
                 || ( node->attributeType() != Renderer::NoAttribute
                      && delta( m_vertices[ v0 ].m_attr, m_vertices[ v1 ].m_attr, vertex.m_attr ) > m_attributeThreshold ) ) {

                // add the new vertex
                int v2 = m_vertices.count();
                m_vertices.append( vertex );

                // divide the edge
                Edge edge;
                edge.m_lod = lod + 2;
                edge.m_v[ 0 ] = v2;
                edge.m_v[ 1 ] = v1;
                edge.m_f[ 0 ] = m_edges[ e0 ].m_f[ 0 ];
                edge.m_f[ 1 ] = m_edges[ e0 ].m_f[ 1 ];

                int e1 = m_edges.count();
                m_edges.append( edge );

                m_edges[ e0 ].m_lod = lod + 2;
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

                    edge.m_lod = lod + 2;
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

    return true;
}

// Create an initial mesh consisting of 4 vertices and 2 faces.
bool SurfaceTessellator::initialize( ParametricMeshNode* node )
{
    Vertex v0, v1, v2, v3;

    if ( !calculateVertex( node, &v0, QVector2D( m_pMin, m_qMin ) ) )
        return false;
    if ( !calculateVertex( node, &v1, QVector2D( m_pMax, m_qMin ) ) )
        return false;
    if ( !calculateVertex( node, &v2, QVector2D( m_pMin, m_qMax ) ) )
        return false;
    if ( !calculateVertex( node, &v3, QVector2D( m_pMax, m_qMax ) ) )
        return false;

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

    return true;
}

// The same as QVector3D::normal(), but it doesn't round the vector down to zero for very small distances.
static QVector3D preciseNormal( const QVector3D& v1, const QVector3D& v2, const QVector3D& v3 )
{
    QVector3D v = QVector3D::crossProduct( ( v2 - v1 ), ( v3 - v1 ) );

    return v / qSqrt( (double)v.x() * (double)v.x() + (double)v.y() * (double)v.y() + (double)v.z() * (double)v.z() );
}

bool SurfaceTessellator::calculateVertex( ParametricMeshNode* node, Vertex* vertex, const QVector2D& param )
{
    vertex->m_param = param;

    if ( !node->calculateVertex( param, &vertex->m_pos, &vertex->m_attr ) )
        return false;

    float dp = 0.1f * ( m_pMax - m_pMin ) / (float)( 1 << ( m_lodMax / 2 ) );
    float dq = 0.1f * ( m_qMax - m_qMin ) / (float)( 1 << ( m_lodMax / 2 ) );

    // calculate two samples with small offset from original vertex
    QVector2D p1 = param + QVector2D( dp, 0.0f );
    QVector2D p2 = param + QVector2D( 0.0f, dq );

    QVector3D v1, v2, attr;
    if ( !node->calculateVertex( p1, &v1, &attr ) || !node->calculateVertex( p2, &v2, &attr ) )
        return false;

    bool eq1 = qFuzzyCompare( vertex->m_pos, v1 );
    bool eq2 = qFuzzyCompare( vertex->m_pos, v2 );

    if ( !eq1 && !eq2 ) {
        vertex->m_normal = preciseNormal( vertex->m_pos, v1, v2 );
    } else {
        // if one of the points coincides with the vertex, calculate a third sample
        QVector2D p3 = param + QVector2D( dp, dq );
        QVector3D v3;
        if ( !node->calculateVertex( p3, &v3, &attr ) )
            return false;

        vertex->m_normal = eq1 ? preciseNormal( vertex->m_pos, v3, v2 ) : preciseNormal( vertex->m_pos, v1, v3 );
    }

    return true;
}

int SurfaceTessellator::count() const
{
    return m_faces.count();
}

QByteArray SurfaceTessellator::vertexArray( Renderer::AttributeType attr )
{
    int count = m_vertices.count();

    int size = 6 * sizeof( float );

    if ( attr == Renderer::RgbAttribute )
        size += 3 * sizeof( float );

    QByteArray buffer;
    buffer.resize( count * size );

    float* data = reinterpret_cast<float*>( buffer.data() );

    const Vertex* vertex = m_vertices.constData();

    for ( int i = 0; i < count; i++ ) {
        *data++ = vertex[ i ].m_pos.x();
        *data++ = vertex[ i ].m_pos.y();
        *data++ = vertex[ i ].m_pos.z();

        *data++ = vertex[ i ].m_normal.x();
        *data++ = vertex[ i ].m_normal.y();
        *data++ = vertex[ i ].m_normal.z();

        if ( attr == Renderer::RgbAttribute ) {
            *data++ = vertex[ i ].m_attr.x();
            *data++ = vertex[ i ].m_attr.y();
            *data++ = vertex[ i ].m_attr.z();
        }
    }

    return buffer;
}

QByteArray SurfaceTessellator::indexArray()
{
    int count = m_faces.count();

    int size = 3 * sizeof( int );

    QByteArray buffer;
    buffer.resize( count * size );

    int* data = reinterpret_cast<int*>( buffer.data() );

    const Face* face = m_faces.constData();

    for ( int i = 0; i < count; i++ ) {
        for ( int j = 0; j < 3; j++ )
            *data++ = m_edges[ face[ i ].m_e[ j ] ].m_v[ face[ i ].m_dir[ j ] ];
    }

    return buffer;
}
