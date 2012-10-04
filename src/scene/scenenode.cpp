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

#include "scene/scenenode.h"

SceneNode::SceneNode( SceneNode* parent ) :
    m_scene( NULL )
{
    if ( parent != NULL ) {
        m_scene = parent->m_scene;

        parent->m_nodes.append( this );
    }
}

SceneNode::~SceneNode()
{
    qDeleteAll( m_nodes );
}

int SceneNode::elementsCount() const
{
    int count = 0;

    foreach ( SceneNode* node, m_nodes )
        count += node->elementsCount();

    return count;
}

bool SceneNode::calculateNodes( const SceneNodeContext& context )
{
    foreach ( SceneNode* node, m_nodes ) {
        if ( !node->calculate( context ) )
            return false;
    }

    return true;
}

void SceneNode::renderNodes()
{
    foreach ( SceneNode* node, m_nodes )
        node->render();
}
