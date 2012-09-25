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

#ifndef PROJECTITEM_H
#define PROJECTITEM_H

class ProjectItem
{
public:
    enum Type
    {
        Project,
        Folder,
        Group,
        Curve,
        Surface
    };

public:
    ProjectItem( Type type, ProjectItem* parent );
    virtual ~ProjectItem();

public:
    Type type() const { return m_type; }

    ProjectItem* parent() const { return m_parent; }
    const QList<ProjectItem*>& items() const { return m_items; }

    void setName( const QString& name );
    const QString& name() const { return m_name; }

protected:
    Type m_type;

    ProjectItem* m_parent;

    QList<ProjectItem*> m_items;

    QString m_name;
};

#endif
