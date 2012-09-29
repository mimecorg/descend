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

#ifndef GROUPITEM_H
#define GROUPITEM_H

#include "project/projectitem.h"
#include "scene/scenenodecolor.h"

class GroupItem : public ProjectItem
{
public:
    GroupItem( ProjectItem* parent );
    ~GroupItem();

public:
    void setCode( const QString& text );
    const QString& code() const { return m_code; }

    void setColor( const SceneNodeColor& color );
    const SceneNodeColor& color() const { return m_color; }

private:
    QString m_code;

    SceneNodeColor m_color;
};

#endif
