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

#include "project/projectitemmodel.h"

#include "project/project.h"
#include "utils/iconloader.h"

ProjectItemModel::ProjectItemModel( Project* project, QObject* parent ) : QAbstractItemModel( parent ),
    m_project( project ),
    m_markedItem( NULL )
{
}

ProjectItemModel::~ProjectItemModel()
{
}

QModelIndex ProjectItemModel::index( int row, int column, const QModelIndex& parent /*= QModelIndex()*/ ) const
{
    if ( !parent.isValid() ) {
        if ( row == 0 )
            return createIndex( row, column, m_project );
        else
            return QModelIndex();
    }

    ProjectItem* item = static_cast<ProjectItem*>( parent.internalPointer() );

    if ( row >= 0 && row < item->items().count() )
        return createIndex( row, column, item->items().at( row ) );
    else
        return QModelIndex();
}

QModelIndex ProjectItemModel::parent( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    ProjectItem* item = static_cast<ProjectItem*>( index.internalPointer() );

    if ( item == m_project )
        return QModelIndex();

    ProjectItem* parent = item->parent();

    if ( parent == m_project )
        return createIndex( 0, 0, m_project );

    ProjectItem* grandParent = parent->parent();

    return createIndex( grandParent->items().indexOf( parent ), 0, parent );
}

int ProjectItemModel::rowCount( const QModelIndex& index /*= QModelIndex()*/ ) const
{
    if ( !index.isValid() )
        return 1;

    ProjectItem* item = static_cast<ProjectItem*>( index.internalPointer() );

    return item->items().count();
}

int ProjectItemModel::columnCount( const QModelIndex& /*index*/ /*= QModelIndex()*/ ) const
{
    return 1;
}

QVariant ProjectItemModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() || index.column() != 0 )
        return QVariant();

    ProjectItem* item = static_cast<ProjectItem*>( index.internalPointer() );

    switch ( role ) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return item->name();

        case Qt::DecorationRole:
            switch ( item->type() ) {
                case ProjectItem::Project:
                    return IconLoader::pixmap( "item-project" );

                case ProjectItem::Folder:
                    return IconLoader::pixmap( "item-folder" );

                case ProjectItem::Group:
                    return IconLoader::pixmap( "item-group" );

                case ProjectItem::Curve:
                    return IconLoader::pixmap( "item-curve" );

                case ProjectItem::Surface:
                    return IconLoader::pixmap( "item-surface" );
            }
            break;

        case Qt::FontRole:
            if ( item == m_markedItem ) {
                QFont font;
                font.setBold( true );
                return font;
            }
    }

    return QVariant();
}

QVariant ProjectItemModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const
{
    if ( role != Qt::DisplayRole || orientation != Qt::Horizontal || section != 0 )
        return QVariant();

    return "Name";
}

Qt::ItemFlags ProjectItemModel::flags( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    ProjectItem* item = static_cast<ProjectItem*>( index.internalPointer() );

    if ( item == m_project )
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

ProjectItem* ProjectItemModel::itemFromIndex( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return NULL;

    return static_cast<ProjectItem*>( index.internalPointer() );
}

QModelIndex ProjectItemModel::indexFromItem( ProjectItem* item ) const
{
    if ( !item )
        return QModelIndex();

    if ( item == m_project )
        return createIndex( 0, 0, m_project );

    ProjectItem* parent = item->parent();

    return createIndex( parent->items().indexOf( item ), 0, item );
}

bool ProjectItemModel::setData( const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/ )
{
    if ( role != Qt::EditRole || !index.isValid() || index.column() != 0 )
        return false;

    QString name = value.toString().trimmed();

    if ( name.isEmpty() )
        return false;

    ProjectItem* item = static_cast<ProjectItem*>( index.internalPointer() );

    if ( name == item->name() )
        return false;

    item->setName( name );

    emit dataChanged( index, index );

    emit projectModified();

    return true;
}

QModelIndex ProjectItemModel::insertItem( ProjectItem::Type type, const QString& name, const QModelIndex& parent )
{
    if ( !parent.isValid() )
        return QModelIndex();

    ProjectItem* item = static_cast<ProjectItem*>( parent.internalPointer() );

    int row = item->items().count();

    emit beginInsertRows( parent, row, row );

    ProjectItem* newItem = m_project->createItem( type, item );
    newItem->setName( name );

    emit endInsertRows();

    emit projectModified();

    return createIndex( row, 0, newItem );
}

QModelIndex ProjectItemModel::cloneItem( const QModelIndex& index, const QString& name )
{
    if ( !index.isValid() )
        return QModelIndex();

    ProjectItem* item = static_cast<ProjectItem*>( index.internalPointer() );

    if ( item == m_project )
        return QModelIndex();

    ProjectItem* parentItem = item->parent();

    int row = parentItem->items().count();

    emit beginInsertRows( this->parent( index ), row, row );

    ProjectItem* newItem = m_project->cloneItem( item );
    newItem->setName( name );

    emit endInsertRows();

    emit projectModified();

    return createIndex( row, 0, newItem );
}

bool ProjectItemModel::deleteItem( const QModelIndex& index )
{
    if ( !index.isValid() )
        return false;

    ProjectItem* item = static_cast<ProjectItem*>( index.internalPointer() );

    if ( item == m_project )
        return false;

    if ( item->contains( m_markedItem ) )
        m_markedItem = NULL;

    ProjectItem* parentItem = item->parent();

    int row = parentItem->items().indexOf( item );

    emit beginRemoveRows( this->parent( index ), row, row );

    delete item;

    emit endRemoveRows();

    emit projectModified();

    return true;
}

void ProjectItemModel::setMarkedItem( ProjectItem* item )
{
    if ( m_markedItem != item ) {
        QModelIndex oldIndex = indexFromItem( m_markedItem );
        QModelIndex newIndex = indexFromItem( item );

        m_markedItem = item;

        if ( oldIndex.isValid() )
            emit dataChanged( oldIndex, oldIndex );

        if ( newIndex.isValid() )
            emit dataChanged( newIndex, newIndex );
    }
}
