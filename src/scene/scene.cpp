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

#include "scene/scene.h"

#include "misc/miscengine.h"
#include "misc/miscunit.h"

static const char* const identifiers[ Scene::IdCount ] =
{
    "u", "v", "u_min", "u_max", "v_min", "v_max",
    "pos", "color", "color2", "transform"
};

Scene::Scene() : SceneNode( NULL )
{
    m_scene = this;

    m_engine = new MiscEngine();

    for ( int i = 0; i < IdCount; i++ )
        m_identifiers[ i ] = MiscIdentifier( identifiers[ i ], m_engine );

    m_unit = new MiscUnit( m_engine );
}

Scene::~Scene()
{
    delete m_engine;
}

bool Scene::addCode( const QString& text )
{
    MiscCode code = m_unit->compileUnit( text );

    if ( !code.isValid() )
        return false;

    m_codes.append( code );

    return true;
}

bool Scene::calculate( const SceneNodeContext& parentContext )
{
    foreach ( const MiscCode& code, m_codes ) {
        if ( !m_engine->execute( code ) )
            return false;
    }

    return calculateNodes( parentContext );
}

void Scene::render()
{
    renderNodes();
}
