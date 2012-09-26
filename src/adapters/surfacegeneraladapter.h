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

#ifndef SURFACEGENERALADAPTER_H
#define SURFACEGENERALADAPTER_H

#include "adapters/generaladapter.h"

class SurfaceItem;

class SurfaceGeneralAdapter : public GeneralAdapter
{
public:
    SurfaceGeneralAdapter( SurfaceItem* surface );
    ~SurfaceGeneralAdapter();

public:
    bool hasRendering() const;
    bool hasReverseWinding() const;

    void setAttributeType( Renderer::AttributeType type );
    Renderer::AttributeType attributeType() const;

    void setReverseWinding( bool on );
    bool isReverseWinding() const;

    bool hasColor() const;
    bool hasBackColor() const;

    void setFrontColor( const QColor& color );
    QColor frontColor() const;

    void setBackColor( const QColor& color );
    QColor backColor() const;

private:
    SurfaceItem* m_surface;
};

#endif
