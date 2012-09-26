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

#include "adapters/curvegeneraladapter.h"
#include "project/curveitem.h"

CurveGeneralAdapter::CurveGeneralAdapter( CurveItem* curve ) : m_curve( curve )
{
}

CurveGeneralAdapter::~CurveGeneralAdapter()
{
}

bool CurveGeneralAdapter::hasRendering() const
{
    return true;
}

void CurveGeneralAdapter::setAttributeType( Renderer::AttributeType type )
{
    m_curve->setAttributeType( type );
}

Renderer::AttributeType CurveGeneralAdapter::attributeType() const
{
    return m_curve->attributeType();
}

bool CurveGeneralAdapter::hasColor() const
{
    return true;
}

void CurveGeneralAdapter::setFrontColor( const QColor& color )
{
    m_curve->setColor( color );
}

QColor CurveGeneralAdapter::frontColor() const
{
    return m_curve->color();
}
