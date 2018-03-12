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

#include "scene/scenenodecontext.h"

#include "misc/miscunit.h"
#include "misc/miscvalue.h"
#include "scene/scene.h"
#include "scene/scenenodecolor.h"

SceneNodeContext::SceneNodeContext()
{
}

SceneNodeContext::SceneNodeContext( const SceneNodeContext& other ) :
    m_matrix( other.m_matrix )
{
    m_color[ 0 ] = other.m_color[ 0 ];
    m_color[ 1 ] = other.m_color[ 1 ];
}

SceneNodeContext::~SceneNodeContext()
{
}

SceneNodeContext& SceneNodeContext::operator =( const SceneNodeContext& other )
{
    m_matrix = other.m_matrix;
    m_color[ 0 ] = other.m_color[ 0 ];
    m_color[ 1 ] = other.m_color[ 1 ];
    return *this;
}

void SceneNodeContext::setMatrix( const QMatrix4x4& matrix )
{
    m_matrix = matrix;
}

void SceneNodeContext::transform( const QMatrix4x4& matrix )
{
    m_matrix = matrix * m_matrix;
    m_matrix.optimize();
}

void SceneNodeContext::setColor( int index, const QColor& color )
{
    Q_ASSERT( index >= 0 && index < 2 );

    m_color[ index ] = color;
}

QColor SceneNodeContext::color( int index ) const
{
    Q_ASSERT( index >= 0 && index < 2 );

    return m_color[ index ];
}

static QColor vectorToColor( const QVector4D& vector )
{
    return QColor::fromRgbF( qBound( 0.0, (double)vector.x(), 1.0 ), qBound( 0.0, (double)vector.y(), 1.0 ), qBound( 0.0, (double)vector.z(), 1.0 ) );
}

void SceneNodeContext::initializeColor( const SceneNodeContext& parentContext, const SceneNodeColor& color, MiscUnit* unit, Scene* scene )
{
    switch ( color.type( 0 ) ) {
        case SceneNodeColor::InheritSecondary:
            m_color[ 0 ] = parentContext.m_color[ 1 ];
            break;
        case SceneNodeColor::Custom:
            m_color[ 0 ] = color.color( 0 );
            break;
        case SceneNodeColor::Calculated:
            m_color[ 0 ] = vectorToColor( unit->variable( scene->identifier( Scene::Color ) ).toVector() );
            break;
        default:
            break;
    }

    if ( color.flags() & SceneNodeColor::DualColors ) {
        switch ( color.type( 1 ) ) {
            case SceneNodeColor::InheritPrimary:
                m_color[ 1 ] = parentContext.m_color[ 0 ];
                break;
            case SceneNodeColor::Custom:
                m_color[ 1 ] = color.color( 1 );
                break;
            case SceneNodeColor::Calculated:
                m_color[ 1 ] = vectorToColor( unit->variable( scene->identifier( Scene::Color2 ) ).toVector() );
                break;
            default:
                break;
        }
    } else {
        m_color[ 1 ] = m_color[ 0 ];
    }

    if ( color.flags() & SceneNodeColor::SwapColors )
        qSwap( m_color[ 0 ], m_color[ 1 ] );
}
