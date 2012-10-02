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

#include "project/parametricmeshitem.h"

#include "project/project.h"
#include "scene/parametricmeshnode.h"

ParametricMeshItem::ParametricMeshItem( ProjectItem::Type type, ProjectItem* parent ) : ProjectItem( type, parent ),
    m_attributeType( Renderer::NoAttribute )
{
    if ( type == Surface )
        m_color.setFlags( SceneNodeColor::DualColors );
}

ParametricMeshItem::~ParametricMeshItem()
{
}

void ParametricMeshItem::setInitCode( const QString& text )
{
    m_initCode = text;
}

void ParametricMeshItem::setCalcCode( const QString& text )
{
    m_calcCode = text;
}

void ParametricMeshItem::setAttributeType( Renderer::AttributeType type )
{
    m_attributeType = type;
}

void ParametricMeshItem::setColor( const SceneNodeColor& color )
{
    m_color = color;
}

SceneNode* ParametricMeshItem::createNode( SceneNode* parent )
{
    Renderer::MeshType meshType;

    switch ( type() ) {
        case Curve:
            meshType = Renderer::CurveMesh;
            break;
        case Surface:
            meshType = Renderer::SurfaceMesh;
            break;
        default:
            return NULL;
    }

    ParametricMeshNode* node = new ParametricMeshNode( meshType, m_attributeType, m_color, parent );

    if ( !node->addInitCode( m_initCode ) ) {
        m_project->setErrorInfo( this, Project::InitContext );
        return NULL;
    }

    if ( !node->addCalcCode( m_calcCode ) ) {
        m_project->setErrorInfo( this, Project::CalcContext );
        return NULL;
    }

    return node;
}
