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

#ifndef SCENENODE_H
#define SCENENODE_H

class Scene;
class MiscUnit;
class SceneNodeColor;
class SceneNodeContext;

#include <QList>

class SceneNode
{
public:
    SceneNode( SceneNode* parent );
    virtual ~SceneNode();

public:
    virtual MiscUnit* unit() const = 0;

    virtual bool calculate( const SceneNodeContext& parentContext ) = 0;

    virtual void render() = 0;

    virtual int elementsCount() const;

public:
    Scene* scene() const { return m_scene; }

protected:
    bool calculateNodes( const SceneNodeContext& context );

    void renderNodes();

protected:
    Scene* m_scene;

    QList<SceneNode*> m_nodes;
};

#endif
