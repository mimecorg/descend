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

#ifndef PARAMETRICMESHITEM_H
#define PARAMETRICMESHITEM_H

#include "project/projectitem.h"
#include "scene/renderer.h"
#include "scene/scenenodecolor.h"

class ParametricMeshItem : public ProjectItem
{
public:
    ParametricMeshItem( ProjectItem::Type type, ProjectItem* parent );
    ~ParametricMeshItem();

public:
    void setInitCode( const QString& text );
    const QString& initCode() const { return m_initCode; }

    void setCalcCode( const QString& text );
    const QString& calcCode() const { return m_calcCode; }

    void setAttributeType( Renderer::AttributeType type );
    Renderer::AttributeType attributeType() const { return m_attributeType; }

    void setColor( const SceneNodeColor& color );
    const SceneNodeColor& color() const { return m_color; }

private:
    QString m_initCode;
    QString m_calcCode;

    Renderer::AttributeType m_attributeType;

    SceneNodeColor m_color;
};

#endif
