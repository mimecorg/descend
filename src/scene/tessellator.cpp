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

#include "scene/tessellator.h"

Tessellator::Tessellator() :
    m_lodMin( 0 ),
    m_lodMax( 0 ),
    m_geometryThreshold( 0 ),
    m_attributeThreshold( 0 ),
    m_uMin( 0 ),
    m_uMax( 0 ),
    m_vMin( 0 ),
    m_vMax( 0 )
{
}

Tessellator::~Tessellator()
{
}

void Tessellator::setLodRange( int min, int max )
{
    m_lodMin = min;
    m_lodMax = max;
}

void Tessellator::setGeometryThreshold( float threshold )
{
    m_geometryThreshold = threshold;
}

void Tessellator::setAttributeThreshold( float threshold )
{
    m_attributeThreshold = threshold;
}

void Tessellator::setURange( float min, float max )
{
    m_uMin = min;
    m_uMax = max;
}

void Tessellator::setVRange( float min, float max )
{
    m_vMin = min;
    m_vMax = max;
}

// Calculate the distance from point v2 to the line going through v0 and v1 relative to the distance from v0 to v1.
float Tessellator::delta( const QVector3D& v0, const QVector3D& v1, const QVector3D& v2 )
{
    QVector3D line = v1 - v0;

    float length = line.length();

    if ( qFuzzyCompare( length, 0.0f ) )
        return 0.0f;

    return v2.distanceToLine( v0, line / length ) / length;
}
