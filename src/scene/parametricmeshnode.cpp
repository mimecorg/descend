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

#include "scene/parametricmeshnode.h"

#include "misc/miscengine.h"
#include "misc/miscsymbol.h"
#include "misc/miscunit.h"
#include "misc/miscvalue.h"
#include "scene/scene.h"
#include "scene/tessellator.h"

#include <QVector2D>

ParametricMeshNode::ParametricMeshNode( Renderer::MeshType type, Renderer::AttributeType attr, const SceneNodeColor& color, SceneNode* parent ) : SceneNode( parent ),
    m_meshType( type ),
    m_attributeType( attr ),
    m_color( color ),
    m_vertexBuffer( QGLBuffer::VertexBuffer ),
    m_indexBuffer( QGLBuffer::IndexBuffer ),
    m_count( 0 )
{
    m_initUnit = new MiscUnit( parent->unit() ),

    m_initUnit->addVariable( MiscSymbol( Misc::MatrixType, m_scene->identifier( Scene::Transform ) ) );

    if ( attr == Renderer::NoAttribute && color.type( 0 ) == SceneNodeColor::Calculated ) {
        m_initUnit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::Color ) ) );

        if ( color.flags() & SceneNodeColor::DualColors )
            m_initUnit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::Color2 ) ) );
    }

    m_initUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::U_Min ) ) );
    m_initUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::U_Max ) ) );

    if ( type == Renderer::SurfaceMesh ) {
        m_initUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::V_Min ) ) );
        m_initUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::V_Max ) ) );
    }

    m_calcUnit = new MiscUnit( m_initUnit );

    m_calcUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::U ), Misc::ConstantSymbol ) );
    if ( type == Renderer::SurfaceMesh )
        m_calcUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::V ), Misc::ConstantSymbol ) );

    m_calcUnit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::Pos ) ) );
    if ( attr == Renderer::RgbAttribute )
        m_calcUnit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::Color ) ) );
}

ParametricMeshNode::~ParametricMeshNode()
{
}

bool ParametricMeshNode::addInitCode( const QString& text )
{
    MiscCode code = m_initUnit->compileUnit( text );

    if ( !code.isValid() )
        return false;

    m_initCodes.append( code );

    return true;
}

bool ParametricMeshNode::addCalcCode( const QString& text )
{
    MiscCode code = m_calcUnit->compileUnit( text );

    if ( !code.isValid() )
        return false;

    m_calcCodes.append( code );

    return true;
}

bool ParametricMeshNode::calculate( const SceneNodeContext& parentContext )
{
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();

    Tessellator* tessellator = calculateCommon( parentContext );

    if ( !tessellator )
        return false;

    m_count = tessellator->count();

    QByteArray vertexArray = tessellator->vertexArray( m_attributeType );
    QByteArray indexArray = tessellator->indexArray();

    tessellator->clear();

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate( vertexArray.constData(), vertexArray.size() );

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate( indexArray.constData(), indexArray.size() );

    return true;
}

bool ParametricMeshNode::exportMesh( QDataStream& stream, MeshHeader* header, const SceneNodeContext& parentContext )
{
    if ( m_meshType == Renderer::SurfaceMesh ) {
        Tessellator* tessellator = calculateCommon( parentContext );

        if ( !tessellator )
            return false;

        tessellator->exportMesh( stream, header );
    }

    return true;
}

Tessellator* ParametricMeshNode::calculateCommon( const SceneNodeContext& parentContext )
{
    MiscEngine* engine = m_scene->engine();

    m_initUnit->setVariable( m_scene->identifier( Scene::Transform ), MiscValue( Misc::MatrixType, engine ) );

    if ( m_attributeType == Renderer::NoAttribute && m_color.type( 0 ) == SceneNodeColor::Calculated ) {
        m_initUnit->setVariable( m_scene->identifier( Scene::Color ), MiscValue( Misc::VectorType, engine ) );

        if ( m_color.flags() & SceneNodeColor::DualColors )
            m_initUnit->setVariable( m_scene->identifier( Scene::Color2 ), MiscValue( Misc::VectorType, engine ) );
    }

    m_initUnit->setVariable( m_scene->identifier( Scene::U_Min ), MiscValue( 0.0f, engine ) );
    m_initUnit->setVariable( m_scene->identifier( Scene::U_Max ), MiscValue( 1.0f, engine ) );

    if ( m_meshType == Renderer::SurfaceMesh ) {
        m_initUnit->setVariable( m_scene->identifier( Scene::V_Min ), MiscValue( 0.0f, engine ) );
        m_initUnit->setVariable( m_scene->identifier( Scene::V_Max ), MiscValue( 1.0f, engine ) );
    }

    foreach ( const MiscCode& code, m_initCodes ) {
        if ( !engine->execute( code ) )
            return NULL;
    }

    m_context = parentContext;

    m_context.transform( m_initUnit->variable( m_scene->identifier( Scene::Transform ) ).toMatrix() );

    if ( m_attributeType == Renderer::NoAttribute )
        m_context.initializeColor( parentContext, m_color, m_initUnit, m_scene );

    Tessellator* tessellator = Renderer::currentRenderer()->tessellator( m_meshType );

    float uMin = m_initUnit->variable( m_scene->identifier( Scene::U_Min ) ).toFloat();
    float uMax = m_initUnit->variable( m_scene->identifier( Scene::U_Max ) ).toFloat();
    tessellator->setURange( uMin, uMax );

    if ( m_meshType == Renderer::SurfaceMesh ) {
        float vMin = m_initUnit->variable( m_scene->identifier( Scene::V_Min ) ).toFloat();
        float vMax = m_initUnit->variable( m_scene->identifier( Scene::V_Max ) ).toFloat();
        tessellator->setVRange( vMin, vMax );
    }

    if ( !tessellator->tessellate( this ) )
        return NULL;

    return tessellator;
}

bool ParametricMeshNode::calculateVertex( float param, QVector3D* pos, QVector3D* attr )
{
    Q_ASSERT( m_meshType == Renderer::CurveMesh );

    MiscEngine* engine = m_scene->engine();

    m_calcUnit->setVariable( m_scene->identifier( Scene::U ), MiscValue( param, engine ) );

    return calculateVertexCommon( pos, attr );
}

bool ParametricMeshNode::calculateVertex( const QVector2D& param, QVector3D* pos, QVector3D* attr )
{
    Q_ASSERT( m_meshType == Renderer::SurfaceMesh );

    MiscEngine* engine = m_scene->engine();

    m_calcUnit->setVariable( m_scene->identifier( Scene::U ), MiscValue( param.x(), engine ) );
    m_calcUnit->setVariable( m_scene->identifier( Scene::V ), MiscValue( param.y(), engine ) );

    return calculateVertexCommon( pos, attr );
}

bool ParametricMeshNode::calculateVertexCommon( QVector3D* pos, QVector3D* attr )
{
    MiscEngine* engine = m_scene->engine();

    m_calcUnit->setVariable( m_scene->identifier( Scene::Pos ), MiscValue( Misc::VectorType, engine ) );
    if ( m_attributeType == Renderer::RgbAttribute )
        m_calcUnit->setVariable( m_scene->identifier( Scene::Color ), MiscValue( Misc::VectorType, engine ) );

    foreach ( const MiscCode& code, m_calcCodes ) {
        if ( !engine->execute( code ) )
            return false;
    }

    QVector3D position = m_calcUnit->variable( m_scene->identifier( Scene::Pos ) ).toVector().toVector3D();

    *pos = m_context.matrix().map( position );

    if ( m_attributeType == Renderer::RgbAttribute )
        *attr = m_calcUnit->variable( m_scene->identifier( Scene::Color ) ).toVector().toVector3D();

    return true;
}

void ParametricMeshNode::render()
{
    if ( m_attributeType == Renderer::NoAttribute )
        Renderer::currentRenderer()->setColor( m_context.color( 0 ), m_context.color( 1 ) );

    Renderer::currentRenderer()->renderMesh( m_meshType, m_attributeType, m_vertexBuffer, m_indexBuffer, m_count );
}
