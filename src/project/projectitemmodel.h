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

#ifndef PROJECTITEMMODEL_H
#define PROJECTITEMMODEL_H

#include "project/projectitem.h"

class Project;

class ProjectItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ProjectItemModel( Project* project, QObject* parent );
    ~ProjectItemModel();

public:
    QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& index ) const;

    int rowCount( const QModelIndex& index = QModelIndex() ) const;
    int columnCount( const QModelIndex& index = QModelIndex() ) const;

    QVariant data( const QModelIndex& index, int role ) const;

    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    Qt::ItemFlags flags( const QModelIndex& index ) const;

    ProjectItem* itemFromIndex( const QModelIndex& index ) const;

    bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );

    QModelIndex insertItem( ProjectItem::Type type, const QString& name, const QModelIndex& parent );

    bool deleteItem( const QModelIndex& index );

private:
    Project* m_project;
};

#endif
