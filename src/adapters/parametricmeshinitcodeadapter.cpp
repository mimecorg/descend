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

#include "adapters/parametricmeshinitcodeadapter.h"
#include "project/parametricmeshitem.h"

ParametricMeshInitCodeAdapter::ParametricMeshInitCodeAdapter( ParametricMeshItem* mesh ) : m_mesh( mesh )
{
}

ParametricMeshInitCodeAdapter::~ParametricMeshInitCodeAdapter()
{
}

void ParametricMeshInitCodeAdapter::setCode( const QString& text )
{
    m_mesh->setInitCode( text );
}

QString ParametricMeshInitCodeAdapter::code() const
{
    return m_mesh->initCode();
}

bool ParametricMeshInitCodeAdapter::hasInputsOutputs() const
{
    return true;
}

QStringList ParametricMeshInitCodeAdapter::inputs( Renderer::AttributeType /*attr*/, const SceneNodeColor& /*color*/ ) const
{
    return QStringList();
}

QStringList ParametricMeshInitCodeAdapter::outputs( Renderer::AttributeType attr, const SceneNodeColor& color ) const
{
    QStringList list;
    list << "transform";
    if ( attr == Renderer::NoAttribute && color.type( 0 ) == SceneNodeColor::Calculated ) {
        list << "color";
        if ( color.flags() & SceneNodeColor::DualColors )
            list << "color2";
    }
    list << "u_min" << "u_max";
    if ( m_mesh->type() == ProjectItem::Surface )
        list << "v_min" << "v_max";
    return list;
}
