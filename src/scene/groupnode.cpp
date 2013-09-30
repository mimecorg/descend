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
#include "scene/scenenodecontext.h"

GroupNode::GroupNode( const SceneNodeColor& color, SceneNode* parent ) : SceneNode( parent ),
    m_color( color )
{
    m_unit = new MiscUnit( parent->unit() );

    m_unit->addVariable( MiscSymbol( Misc::MatrixType, m_scene->identifier( Scene::Transform ) ) );

    if ( color.type( 0 ) == SceneNodeColor::Calculated ) {
        m_unit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::Color ) ) );

        if ( color.flags() & SceneNodeColor::DualColors )
            m_unit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::Color2 ) ) );
    }
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

bool GroupNode::calculate( const SceneNodeContext& parentContext )
{
    SceneNodeContext context;

    if ( !calculateCommon( parentContext, &context ) )
        return false;

    return calculateNodes( context );
}

bool GroupNode::exportMesh( QDataStream& stream, MeshHeader* header, const SceneNodeContext& parentContext )
{
    SceneNodeContext context;

    if ( !calculateCommon( parentContext, &context ) )
        return false;

    return exportNodes( stream, header, context );
}

bool GroupNode::calculateCommon( const SceneNodeContext& parentContext, SceneNodeContext* context )
{
    MiscEngine* engine = m_scene->engine();

    m_unit->setVariable( m_scene->identifier( Scene::Transform ), MiscValue( Misc::MatrixType, engine ) );

    if ( m_color.type( 0 ) == SceneNodeColor::Calculated ) {
        m_unit->setVariable( m_scene->identifier( Scene::Color ), MiscValue( Misc::VectorType, engine ) );

        if ( m_color.flags() & SceneNodeColor::DualColors )
            m_unit->setVariable( m_scene->identifier( Scene::Color2 ), MiscValue( Misc::VectorType, engine ) );
    }

    foreach ( const MiscCode& code, m_codes ) {
        if ( !engine->execute( code ) )
            return false;
    }

    *context = parentContext;

    context->transform( m_unit->variable( m_scene->identifier( Scene::Transform ) ).toMatrix() );

    context->initializeColor( parentContext, m_color, m_unit, m_scene );

    return true;
}

void GroupNode::render()
{
    renderNodes();
}
