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

#ifndef TESSELLATOR_H
#define TESSELLATOR_H

#include "scene/renderer.h"

class ParametricMeshNode;

class Tessellator
{
public:
    Tessellator();
    virtual ~Tessellator();

public:
    void setLodRange( int min, int max );

    void setGeometryThreshold( float threshold );
    void setAttributeThreshold( float threshold );

    void setPRange( float min, float max );
    void setQRange( float min, float max );

public:
    virtual void clear() = 0;

    virtual bool tessellate( ParametricMeshNode* node ) = 0;

    virtual int count() const = 0;

    virtual QByteArray vertexArray( Renderer::AttributeType attr ) = 0;
    virtual QByteArray indexArray() = 0;

protected:
    static float delta( const QVector3D& v0, const QVector3D& v1, const QVector3D& v2 );

protected:
    int m_lodMin;
    int m_lodMax;

    float m_geometryThreshold;
    float m_attributeThreshold;

    float m_pMin;
    float m_pMax;
    float m_qMin;
    float m_qMax;
};

#endif
