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

#include "adapters/parametricmeshgeneraladapter.h"
#include "project/parametricmeshitem.h"

ParametricMeshGeneralAdapter::ParametricMeshGeneralAdapter( ParametricMeshItem* mesh ) : m_mesh( mesh )
{
}

ParametricMeshGeneralAdapter::~ParametricMeshGeneralAdapter()
{
}

bool ParametricMeshGeneralAdapter::hasAttributeType() const
{
    return true;
}

void ParametricMeshGeneralAdapter::setAttributeType( Renderer::AttributeType type )
{
    m_mesh->setAttributeType( type );
}

Renderer::AttributeType ParametricMeshGeneralAdapter::attributeType() const
{
    return m_mesh->attributeType();
}

SceneNodeColor::ColorFlags ParametricMeshGeneralAdapter::hasColorFlags()
{
    if ( m_mesh->type() == ProjectItem::Surface ) 
        return SceneNodeColor::DualColors | SceneNodeColor::SwapColors;
    else
        return 0;
}

void ParametricMeshGeneralAdapter::setColor( const SceneNodeColor& color )
{
    m_mesh->setColor( color );
}

SceneNodeColor ParametricMeshGeneralAdapter::color() const
{
    return m_mesh->color();
}
