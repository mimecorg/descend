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

#ifndef PROJECTSERIALIZER_H
#define PROJECTSERIALIZER_H

class Project;
class ProjectItem;

class ProjectSerializer
{
public:
    explicit ProjectSerializer( Project* project );
    ~ProjectSerializer();

public:
    void serialize( ProjectItem* root );
    void deserialize( ProjectItem* root );

    friend QDataStream& operator <<( QDataStream& stream, const ProjectSerializer& serializer );
    friend QDataStream& operator >>( QDataStream& stream, ProjectSerializer& serializer );

private:
    QVariant serializeHelper( ProjectItem* item );
    ProjectItem* deserializeHelper( const QVariant& handle );

private:
    Project* m_project;

    QList<QVariantMap> m_data;
    QList<ProjectItem*> m_items;

    friend class SerializationContext;
};

class SerializationContext
{
public:
    explicit SerializationContext( ProjectSerializer* serializer );
    ~SerializationContext();

public:
    QVariant serialize( ProjectItem* item );
    ProjectItem* deserialize( const QVariant& handle );

private:
    ProjectSerializer* m_serializer;
};

#endif
