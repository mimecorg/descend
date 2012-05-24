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

#ifndef SCENE_H
#define SCENE_H

#include "misc/misccode.h"
#include "misc/miscidentifier.h"
#include "scene/scenenode.h"

class Scene : public SceneNode
{
public:
    enum Id
    {
        M_Matrix,
        M_Color,
        M_FrontColor,
        M_BackColor,
        P_Min,
        P_Max,
        Q_Min,
        Q_Max,
        P,
        Q,
        V_Pos,
        V_Color,
        IdCount
    };

public:
    Scene();
    ~Scene();

public:
    MiscEngine* engine() const { return m_engine; }

    const MiscIdentifier& identifier( Id id ) const { return m_identifiers[ id ]; }

    bool addCode( const QString& text );

public:
    MiscUnit* unit() const { return m_unit; }

    bool calculate( const QMatrix4x4& matrix = QMatrix4x4() );

    void render();

private:
    MiscEngine* m_engine;

    MiscIdentifier m_identifiers[ IdCount ];

    MiscUnit* m_unit;

    QList<MiscCode> m_codes;

};

#endif