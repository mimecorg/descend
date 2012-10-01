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

#include "scene/scenenodecolor.h"

SceneNodeColor::SceneNodeColor( ColorFlags flags /*= 0*/ ) :
    m_flags( flags )
{
    m_type[ 0 ] = InheritPrimary;
    m_type[ 1 ] = InheritSecondary;
}

SceneNodeColor::SceneNodeColor( const SceneNodeColor& other ) :
    m_flags( other.m_flags )
{
    m_type[ 0 ] = other.m_type[ 0 ];
    m_type[ 1 ] = other.m_type[ 1 ];
    m_color[ 0 ] = other.m_color[ 0 ];
    m_color[ 1 ] = other.m_color[ 1 ];
}

SceneNodeColor::~SceneNodeColor()
{
}

SceneNodeColor& SceneNodeColor::operator =( const SceneNodeColor& other )
{
    m_flags = other.m_flags;
    m_type[ 0 ] = other.m_type[ 0 ];
    m_type[ 1 ] = other.m_type[ 1 ];
    m_color[ 0 ] = other.m_color[ 0 ];
    m_color[ 1 ] = other.m_color[ 1 ];
    return *this;
}

void SceneNodeColor::setFlags( ColorFlags flags )
{
    m_flags = flags;
}

void SceneNodeColor::setType( int index, ColorType type )
{
    Q_ASSERT( index >= 0 && index < 2 );

    m_type[ index ] = type;
}

SceneNodeColor::ColorType SceneNodeColor::type( int index ) const
{
    Q_ASSERT( index >= 0 && index < 2 );

    return m_type[ index ];
}

void SceneNodeColor::setColor( int index, const QColor& color )
{
    Q_ASSERT( index >= 0 && index < 2 );

    m_color[ index ] = color;
}

QColor SceneNodeColor::color( int index ) const
{
    Q_ASSERT( index >= 0 && index < 2 );

    return m_color[ index ];
}
