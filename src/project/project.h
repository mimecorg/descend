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

#ifndef PROJECT_H
#define PROJECT_H

#include "project/projectitem.h"

class Scene;
class LocalSettings;

class Project : public ProjectItem
{
public:
    enum Context
    {
        NoContext,
        InitContext,
        CalcContext
    };

public:
    Project();
    ~Project();

public:
    void setCode( const QString& text );
    const QString& code() const { return m_code; }

    void setSetting( const QString& key, const QVariant& value );
    QVariant setting( const QString& key, const QVariant& defaultValue = QVariant() ) const;

    ProjectItem* createItem( ProjectItem::Type type, ProjectItem* parent );

    bool initializeScene( Scene* scene, ProjectItem* root );

    void setErrorInfo( ProjectItem* item, Context context = NoContext );
    ProjectItem* errorItem() const { return m_errorItem; }
    Context errorContext() const { return m_errorContext; }

private:
    bool createChildNodes( ProjectItem* item, SceneNode* parent );

private:
    QString m_code;

    QVariantMap m_settings;

    ProjectItem* m_errorItem;
    Context m_errorContext;
};

#endif
