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

#ifndef GROUPNODE_H
#define GROUPNODE_H

#include "misc/misccode.h"
#include "scene/scenenode.h"
#include "scene/scenenodecolor.h"

class GroupNode : public SceneNode
{
public:
    GroupNode( const SceneNodeColor& color, SceneNode* parent );
    ~GroupNode();

public:
    const SceneNodeColor& color() const { return m_color; }

    bool addCode( const QString& text );

public:
    MiscUnit* unit() const { return m_unit; }

    bool calculate( const SceneNodeContext& parentContext );

    void render();

protected:
    SceneNodeColor m_color;

    MiscUnit* m_unit;

    QList<MiscCode> m_codes;
};

#endif
