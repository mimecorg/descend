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

#ifndef SURFACEITEM_H
#define SURFACEITEM_H

#include "project/parametricmeshitem.h"

class SurfaceItem : public ParametricMeshItem
{
public:
    SurfaceItem( ProjectItem* parent );
    ~SurfaceItem();

public:
    void setReverseWinding( bool on );
    bool isReverseWinding() const { return m_reverseWinding; }

    void setFrontColor( const QColor& color );
    const QColor& frontColor() const { return m_frontColor; }

    void setBackColor( const QColor& color );
    const QColor& backColor() const { return m_backColor; }

private:
    bool m_reverseWinding;

    QColor m_frontColor;
    QColor m_backColor;
};

#endif
