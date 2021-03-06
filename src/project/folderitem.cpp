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

#include "project/folderitem.h"

#include "project/project.h"
#include "scene/groupnode.h"
#include "utils/variantex.h"

FolderItem::FolderItem( ProjectItem* parent ) : ProjectItem( ProjectItem::Folder, parent )
{
}

FolderItem::~FolderItem()
{
}

void FolderItem::setCode( const QString& text )
{
    m_code = text;
}

void FolderItem::serialize( QVariantMap& data, SerializationContext* context ) const
{
    ProjectItem::serialize( data, context );
    data[ "Code" ] << m_code;
}

void FolderItem::deserialize( const QVariantMap& data, SerializationContext* context )
{
    ProjectItem::deserialize( data, context );
    data[ "Code" ] >> m_code;
}

SceneNode* FolderItem::createNode( SceneNode* parent )
{
    GroupNode* node = new GroupNode( SceneNodeColor( SceneNodeColor::DualColors ), parent );

    if ( !node->addCode( m_code ) ) {
        m_project->setErrorInfo( this );
        return NULL;
    }

    return node;
}
