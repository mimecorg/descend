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

#include "project/projectitem.h"

#include "project/project.h"
#include "project/projectserializer.h"
#include "utils/variantex.h"

ProjectItem::ProjectItem( Type type, ProjectItem* parent ) :
    m_type( type ),
    m_parent( parent ),
    m_project( NULL )
{
    if ( parent != NULL ) {
        m_project = parent->m_project;

        parent->m_items.append( this );
    }
}

ProjectItem::~ProjectItem()
{
    if ( m_parent != NULL )
        m_parent->m_items.removeOne( this );

    if ( m_project != NULL && m_project->errorItem() == this )
        m_project->setErrorInfo( NULL );

    foreach ( ProjectItem* item, m_items ) {
        item->m_parent = NULL;
        delete item;
    }
}

void ProjectItem::setName( const QString& name )
{
    m_name = name;
}

bool ProjectItem::contains( ProjectItem* item ) const
{
    while ( item != NULL ) {
        if ( item == this )
            return true;
        item = item->parent();
    }
    return false;
}

void ProjectItem::serialize( QVariantMap& data, SerializationContext* context ) const
{
    data[ "T" ] << static_cast<int>( m_type );
    data[ "P" ] << context->serialize( m_parent );
    if ( m_items.count() > 0 ) {
        QVariantList list;
        foreach ( ProjectItem* item, m_items )
            list.append( context->serialize( item ) );
        data[ "Ch" ] << list;
    }
    data[ "N" ] << m_name;
}

void ProjectItem::deserialize( const QVariantMap& data, SerializationContext* context )
{
    QVariantList list;
    data[ "Ch" ] >> list;
    foreach ( QVariant handle, list ) {
        ProjectItem* item = context->deserialize( handle );
        if ( item != NULL )
            m_items.append( item );
    }
    data[ "N" ] >> m_name;
}

SceneNode* ProjectItem::createNode( SceneNode* /*parent*/ )
{
    return NULL;
}
