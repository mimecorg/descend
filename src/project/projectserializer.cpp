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

#include "project/projectserializer.h"

#include "project/project.h"

ProjectSerializer::ProjectSerializer( Project* project ) :
    m_project( project ),
    m_root( NULL )
{
}

ProjectSerializer::~ProjectSerializer()
{
}

void ProjectSerializer::serialize( ProjectItem* root )
{
    m_data.clear();
    m_items.clear();
    
    m_items.append( root );
    m_data.append( QVariantMap() );

    m_root = root;

    SerializationContext context( this );

    root->serialize( m_data[ 0 ], &context );
}

void ProjectSerializer::deserialize( ProjectItem* root )
{
    m_items.clear();

    m_items.reserve( m_data.count() );
    m_items.append( root );
    for ( int i = 1; i < m_data.count(); i++ )
        m_items.append( NULL );

    m_root = root;

    SerializationContext context( this );

    root->deserialize( m_data.at( 0 ), &context );
}

QVariant ProjectSerializer::serializeHelper( ProjectItem* item )
{
    if ( item == NULL )
        return QVariant();

    for ( int i = 0; i < m_items.count(); i++ ) {
        if ( m_items.at( i ) == item )
            return i;
    }

    if ( !m_root->contains( item ) ) {
        // NOTE: serialization of external references (outside the root item) is currently unsupported
        return QVariant();
    }

    int index = m_items.count();

    m_items.append( item );
    m_data.append( QVariantMap() );

    SerializationContext context( this );

    item->serialize( m_data[ index ], &context );

    return index;
}

ProjectItem* ProjectSerializer::deserializeHelper( const QVariant& handle )
{
    if ( !handle.isValid() )
        return NULL;

    int index = handle.toInt();

    if ( index < 0 || index >= m_items.count() )
        return NULL;

    ProjectItem* item = m_items.at( index );

    if ( item == NULL ) {
        QVariantMap data = m_data.at( index );

        ProjectItem::Type type = static_cast<ProjectItem::Type>( data[ "T" ].toInt() );

        item = m_project->createItem( type, NULL );

        if ( item == NULL )
            return NULL;

        m_items[ index ] = item;

        item->m_parent = deserializeHelper( data[ "P" ] );
        item->m_project = m_project;

        SerializationContext context( this );

        item->deserialize( data, &context );
    }

    return item;
}

QDataStream& operator <<( QDataStream& stream, const ProjectSerializer& serializer )
{
    return stream << serializer.m_data;
}

QDataStream& operator >>( QDataStream& stream, ProjectSerializer& serializer )
{
    return stream >> serializer.m_data;
}

SerializationContext::SerializationContext( ProjectSerializer* serializer ) :
    m_serializer( serializer )
{
}

SerializationContext::~SerializationContext()
{
}

QVariant SerializationContext::serialize( ProjectItem* item )
{
    return m_serializer->serializeHelper( item );
}

ProjectItem* SerializationContext::deserialize( const QVariant& handle )
{
    return m_serializer->deserializeHelper( handle );
}
