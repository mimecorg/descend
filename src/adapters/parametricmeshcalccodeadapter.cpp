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

#include "adapters/parametricmeshcalccodeadapter.h"
#include "project/parametricmeshitem.h"

ParametricMeshCalcCodeAdapter::ParametricMeshCalcCodeAdapter( ParametricMeshItem* mesh ) : m_mesh( mesh )
{
}

ParametricMeshCalcCodeAdapter::~ParametricMeshCalcCodeAdapter()
{
}

void ParametricMeshCalcCodeAdapter::setCode( const QString& text )
{
    m_mesh->setCalcCode( text );
}

QString ParametricMeshCalcCodeAdapter::code() const
{
    return m_mesh->calcCode();
}

bool ParametricMeshCalcCodeAdapter::hasInputsOutputs() const
{
    return true;
}

QStringList ParametricMeshCalcCodeAdapter::inputs( Renderer::AttributeType /*attr*/, const SceneNodeColor& /*color*/ ) const
{
    QStringList list;
    list << "u";
    if ( m_mesh->type() == ProjectItem::Surface )
        list << "v";
    return list;
}

QStringList ParametricMeshCalcCodeAdapter::outputs( Renderer::AttributeType attr, const SceneNodeColor& /*color*/ ) const
{
    QStringList list;
    list << "pos";
    if ( attr == Renderer::RgbAttribute )
        list << "color";
    return list;
}
