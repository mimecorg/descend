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

#ifndef GENERALADAPTER_H
#define GENERALADAPTER_H

#include "scene/renderer.h"

class GeneralAdapter
{
public:
    GeneralAdapter() { }
    virtual ~GeneralAdapter() { }

public:
    virtual bool hasRendering() const { return false; }
    virtual bool hasReverseWinding() const { return false; }

    virtual void setAttributeType( Renderer::AttributeType /*type*/ ) { }
    virtual Renderer::AttributeType attributeType() const { return Renderer::NoAttribute; }

    virtual void setReverseWinding( bool /*on*/ ) { }
    virtual bool isReverseWinding() const { return false; }

    virtual bool hasColor() const { return false; }
    virtual bool hasBackColor() const { return false; }

    virtual void setFrontColor( const QColor& /*color*/ ) { }
    virtual QColor frontColor() const { return QColor(); }

    virtual void setBackColor( const QColor& /*color*/ ) { }
    virtual QColor backColor() const { return QColor(); }
};

#endif
