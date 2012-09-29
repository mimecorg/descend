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

#ifndef SCENENODECONTEXT_H
#define SCENENODECONTEXT_H

class SceneNodeColor;
class MiscUnit;
class Scene;

class SceneNodeContext
{
public:
    SceneNodeContext();
    SceneNodeContext( const SceneNodeContext& other );
    ~SceneNodeContext();

public:
    SceneNodeContext& operator =( const SceneNodeContext& other );

public:
    void setMatrix( const QMatrix4x4& matrix );
    const QMatrix4x4& matrix() const { return m_matrix; }

    void transform( const QMatrix4x4& matrix );

    void setColor( int index, const QColor& color );
    QColor color( int index ) const;

    void initializeColor( const SceneNodeContext& parentContext, const SceneNodeColor& color, MiscUnit* unit, Scene* scene );

private:
    QMatrix4x4 m_matrix;

    QColor m_color[ 2 ];
};

#endif
