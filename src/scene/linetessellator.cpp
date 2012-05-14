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

#include "scene/linetessellator.h"

#include "scene/parametricmeshnode.h"

LineTessellator::LineTessellator()
{
}

LineTessellator::~LineTessellator()
{
}

void LineTessellator::clear()
{
    m_vertices.clear();
    m_edges.clear();
}

bool LineTessellator::tessellate( ParametricMeshNode* node )
{
    clear();

    if ( !initialize( node ) )
        return false;

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

            // if minimum lod is reached, check the distance between the vertex and the edge
            if ( lod < m_lodMin
                 || delta( m_vertices[ v0 ].m_pos, m_vertices[ v1 ].m_pos, vertex.m_pos ) > m_geometryThreshold
                 || ( node->attributeType() == Renderer::NoAttribute
                      && delta( m_vertices[ v0 ].m_attr, m_vertices[ v1 ].m_attr, vertex.m_attr ) > m_attributeThreshold ) ) {

                // add the new vertex
                int v2 = m_vertices.count();
                m_vertices.append( vertex );

                // divide the edge
                Edge edge;
                edge.m_lod = lod + 1;
                edge.m_v[ 0 ] = v2;
                edge.m_v[ 1 ] = v1;

                m_edges.append( edge );

                m_edges[ e0 ].m_lod = lod + 1;
                m_edges[ e0 ].m_v[ 1 ] = v2;

                remaining++;
            }
        }
    }

    return true;
}

bool LineTessellator::initialize( ParametricMeshNode* node )
{
    Vertex v0, v1;

    if ( !calculateVertex( node, &v0, m_pMin ) )
        return false;
    if ( !calculateVertex( node, &v1, m_pMax ) )
        return false;

    m_vertices.append( v0 );
    m_vertices.append( v1 );

    Edge e0 = { 0, { 0, 1 } };

    m_edges.append( e0 );

    return true;
}

bool LineTessellator::calculateVertex( ParametricMeshNode* node, Vertex* vertex, float param )
{
    vertex->m_param = param;

    if ( !node->calculateVertex( param, &vertex->m_pos, &vertex->m_attr ) )
        return false;

    return true;
}

int LineTessellator::count() const
{
    return m_edges.count();
}

QByteArray LineTessellator::vertexArray( Renderer::AttributeType attr )
{
    int count = m_vertices.count();

    int size = 3 * sizeof( float );

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

        if ( attr == Renderer::RgbAttribute ) {
            *data++ = vertex[ i ].m_attr.x();
            *data++ = vertex[ i ].m_attr.y();
            *data++ = vertex[ i ].m_attr.z();
        }
    }

    return buffer;
}

QByteArray LineTessellator::indexArray()
{
    int count = m_edges.count();

    int size = 2 * sizeof( int );

    QByteArray buffer;
    buffer.resize( count * size );

    int* data = reinterpret_cast<int*>( buffer.data() );

    const Edge* edge = m_edges.constData();

    for ( int i = 0; i < count; i++ ) {
        *data++ = edge[ i ].m_v[ 0 ];
        *data++ = edge[ i ].m_v[ 1 ];
    }

    return buffer;
}
