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
#include "project/projectserializer.h"
#include "scene/scene.h"
#include "scene/groupnode.h"
#include "scene/parametricmeshnode.h"
#include "utils/dataserializer.h"
#include "utils/localsettings.h"
#include "utils/variantex.h"
#include "utils/zipfile.h"

Project::Project() : ProjectItem( ProjectItem::Project, NULL ),
    m_errorItem( NULL ),
    m_errorContext( NoContext )
{
    m_project = this;

    m_settings.insert( "Color", QColor( 255, 64, 144 ) );
    m_settings.insert( "Color2", QColor( 255, 164, 64 ) );

    m_settings.insert( "BgColor", QColor( 0, 25, 178 ) );
    m_settings.insert( "BgColor2", QColor( 50, 128, 255 ) );

    m_settings.insert( "MinLod", 8 );
    m_settings.insert( "MaxLod", 16 );
    m_settings.insert( "GeometryThreshold", 0.0464159f );
    m_settings.insert( "AttributeThreshold", 0.0464159f );
    m_settings.insert( "DrawEdges", false );
}

Project::~Project()
{
}

void Project::setCode( const QString& text )
{
    m_code = text;
}

void Project::setSetting( const QString& key, const QVariant& value )
{
    m_settings.insert( key, value );
}

QVariant Project::setting( const QString& key, const QVariant& defaultValue /*= QVariant()*/ ) const
{
    return m_settings.value( key, defaultValue );
}

void Project::setErrorInfo( ProjectItem* item, Context context /*= NoContext*/ )
{
    m_errorItem = item;
    m_errorContext = context;
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

ProjectItem* Project::cloneItem( ProjectItem* item )
{
    ProjectItem* cloned = createItem( item->type(), item->parent() );

    if ( cloned == NULL )
        return NULL;

    ProjectSerializer serializer( this );

    serializer.serialize( item );
    serializer.deserialize( cloned );

    return cloned;
}


bool Project::load( const QString& path )
{
    ZipFileReader reader( path );

    if ( !reader.open() )
        return false;

    if ( !loadProject( reader ) )
        return false;

    if ( !loadSettings( reader ) )
        return false;

    reader.close();

    return true;
}

bool Project::save( const QString& path )
{
    ZipFileWriter writer( path );

    if ( !writer.open() )
        return false;

    saveProject( writer );
    saveSettings( writer );

    writer.close();

    return true;
}

bool Project::loadProject( const ZipFileReader& reader )
{
    QByteArray data = reader.fileData( "project.dat" );

    if ( data.isNull() )
        return false;

    DataSerializer serializer( &data );

    if ( !serializer.openForReading() )
        return false;

    ProjectSerializer projectSerializer( this );
    serializer.stream() >> projectSerializer;

    projectSerializer.deserialize( this );

    return true;
}

void Project::saveProject( ZipFileWriter& writer )
{
    ProjectSerializer projectSerializer( this );
    projectSerializer.serialize( this );

    QByteArray data;
    DataSerializer serializer( &data );

    serializer.openForWriting();

    serializer.stream() << projectSerializer;
    serializer.close();

    writer.addFile( "project.dat", data );
}

bool Project::loadSettings( const ZipFileReader& reader )
{
    QByteArray data = reader.fileData( "settings.dat" );

    if ( data.isNull() )
        return false;

    DataSerializer serializer( &data );

    if ( !serializer.openForReading() )
        return false;

    QVariantMap settings;
    serializer.stream() >> settings;

    for ( QVariantMap::const_iterator it = settings.begin(); it != settings.end(); ++it )
        m_settings[ it.key() ] = it.value();

    return true;
}

void Project::saveSettings( ZipFileWriter& writer )
{
    QByteArray data;
    DataSerializer serializer( &data );

    serializer.openForWriting();

    serializer.stream() << m_settings;
    serializer.close();

    writer.addFile( "settings.dat", data );
}

void Project::serialize( QVariantMap& data, SerializationContext* context ) const
{
    ProjectItem::serialize( data, context );
    data[ "Code" ] << m_code;
}

void Project::deserialize( const QVariantMap& data, SerializationContext* context )
{
    ProjectItem::deserialize( data, context );
    data[ "Code" ] >> m_code;
}

bool Project::initializeScene( Scene* scene, ProjectItem* root )
{
    setErrorInfo( NULL );

    if ( !scene->addCode( m_code ) ) {
        setErrorInfo( this );
        return false;
    }

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

static bool compareItems( ProjectItem* i1, ProjectItem* i2 )
{
    return QString::localeAwareCompare( i1->name(), i2->name() ) < 0;
}

bool Project::createChildNodes( ProjectItem* item, SceneNode* parent )
{
    QList<ProjectItem*> items = item->items();

    qSort( items.begin(), items.end(), &compareItems );

    foreach ( ProjectItem* child, items ) {
        SceneNode* node = child->createNode( parent );
        if ( !node )
            return false;
        if ( !createChildNodes( child, node ) )
            return false;
    }
    return true;
}
