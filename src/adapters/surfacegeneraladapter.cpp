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

#include "adapters/surfacegeneraladapter.h"
#include "project/surfaceitem.h"

SurfaceGeneralAdapter::SurfaceGeneralAdapter( SurfaceItem* surface ) : m_surface( surface )
{
}

SurfaceGeneralAdapter::~SurfaceGeneralAdapter()
{
}

bool SurfaceGeneralAdapter::hasRendering() const
{
    return true;
}

bool SurfaceGeneralAdapter::hasReverseWinding() const
{
    return true;
}

void SurfaceGeneralAdapter::setAttributeType( Renderer::AttributeType type )
{
    m_surface->setAttributeType( type );
}

Renderer::AttributeType SurfaceGeneralAdapter::attributeType() const
{
    return m_surface->attributeType();
}

void SurfaceGeneralAdapter::setReverseWinding( bool on )
{
    m_surface->setReverseWinding( on );
}

bool SurfaceGeneralAdapter::isReverseWinding() const
{
    return m_surface->isReverseWinding();
}

bool SurfaceGeneralAdapter::hasColor() const
{
    return true;
}

bool SurfaceGeneralAdapter::hasBackColor() const
{
    return true;
}

void SurfaceGeneralAdapter::setFrontColor( const QColor& color )
{
    m_surface->setFrontColor( color );
}

QColor SurfaceGeneralAdapter::frontColor() const
{
    return m_surface->frontColor();
}

void SurfaceGeneralAdapter::setBackColor( const QColor& color )
{
    m_surface->setBackColor( color );
}

QColor SurfaceGeneralAdapter::backColor() const
{
    return m_surface->backColor();
}
