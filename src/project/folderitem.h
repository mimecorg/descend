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

#ifndef FOLDERITEM_H
#define FOLDERITEM_H

#include "project/projectitem.h"

class FolderItem : public ProjectItem
{
public:
    FolderItem( ProjectItem* parent );
    ~FolderItem();

public:
    void setCode( const QString& text );
    const QString& code() const { return m_code; }

public: // overrides
    void serialize( QVariantMap& data, SerializationContext* context ) const;
    void deserialize( const QVariantMap& data, SerializationContext* context );

    SceneNode* createNode( SceneNode* parent );

private:
    QString m_code;
};

#endif
