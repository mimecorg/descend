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

#include "scene/groupnode.h"

#include "misc/miscengine.h"
#include "misc/miscsymbol.h"
#include "misc/miscunit.h"
#include "misc/miscvalue.h"
#include "scene/scene.h"

GroupNode::GroupNode( SceneNode* parent ) : SceneNode( parent )
{
    m_unit = new MiscUnit( parent->unit() );

    m_unit->addVariable( MiscSymbol( Misc::MatrixType, m_scene->identifier( Scene::M_Matrix ) ) );
}

GroupNode::~GroupNode()
{
}

bool GroupNode::addCode( const QString& text )
{
    MiscCode code = m_unit->compileUnit( text );

    if ( !code.isValid() )
        return false;

    m_codes.append( code );

    return true;
}

bool GroupNode::calculate( const QMatrix4x4& matrix /*= QMatrix4x4()*/ )
{
    MiscEngine* engine = m_scene->engine();

    m_unit->setVariable( m_scene->identifier( Scene::M_Matrix ), MiscValue( Misc::MatrixType, engine ) );

    foreach ( const MiscCode& code, m_codes ) {
        if ( !engine->execute( code ) )
            return false;
    }

    QMatrix4x4 groupMatrix = m_unit->variable( m_scene->identifier( Scene::M_Matrix ) ).toMatrix();

    return calculateNodes( groupMatrix * matrix );
}

void GroupNode::render()
{
    renderNodes();
}
