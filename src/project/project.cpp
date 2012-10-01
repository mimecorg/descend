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

#include "project/project.h"
#include "project/folderitem.h"
#include "project/groupitem.h"
#include "project/parametricmeshitem.h"
#include "scene/scene.h"
#include "scene/groupnode.h"
#include "scene/parametricmeshnode.h"

Project::Project() : ProjectItem( ProjectItem::Project, NULL )
{
}

Project::~Project()
{
}

void Project::setCode( const QString& text )
{
    m_code = text;
}

ProjectItem* Project::createItem( ProjectItem::Type type, ProjectItem* parent )
{
    switch ( type ) {
        case ProjectItem::Folder:
            return new FolderItem( parent );

        case ProjectItem::Group:
            return new GroupItem( parent );

        case ProjectItem::Curve:
        case ProjectItem::Surface:
            return new ParametricMeshItem( type, parent );

        default:
            return NULL;
    }
}

bool Project::initializeScene( Scene* scene, ProjectItem* root )
{
    if ( !scene->addCode( m_code ) )
        return false;

    QList<ProjectItem*> items;

    for ( ProjectItem* item = root; item != this; item = item->parent() )
        items.prepend( item );

    SceneNode* parent = scene;

    foreach ( ProjectItem* item, items ) {
        SceneNode* node = item->createNode( parent );
        if ( !node )
            return false;
        parent = node;
    }

    return createChildNodes( root, parent );
}

bool Project::createChildNodes( ProjectItem* item, SceneNode* parent )
{
    foreach ( ProjectItem* child, item->items() ) {
        SceneNode* node = child->createNode( parent );
        if ( !node )
            return false;
        if ( !createChildNodes( child, node ) )
            return false;
    }
    return true;
}
