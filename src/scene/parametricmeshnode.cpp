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

ParametricMeshNode::ParametricMeshNode( Renderer::MeshType type, Renderer::AttributeType attr, SceneNode* parent ) : SceneNode( parent ),
    m_meshType( type ),
    m_attributeType( attr ),
    m_reverseWinding( false ),
    m_vertexBuffer( QGLBuffer::VertexBuffer ),
    m_indexBuffer( QGLBuffer::IndexBuffer ),
    m_count( 0 )
{
    m_initUnit = new MiscUnit( parent->unit() ),

    m_initUnit->addVariable( MiscSymbol( Misc::MatrixType, m_scene->identifier( Scene::M_Matrix ) ) );

    if ( attr == Renderer::NoAttribute ) {
        if ( type == Renderer::CurveMesh )
            m_initUnit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::M_Color ) ) );

        if ( type == Renderer::SurfaceMesh ) {
            m_initUnit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::M_FrontColor ) ) );
            m_initUnit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::M_BackColor ) ) );
        }
    }

    m_initUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::P_Min ) ) );
    m_initUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::P_Max ) ) );

    if ( type == Renderer::SurfaceMesh ) {
        m_initUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::Q_Min ) ) );
        m_initUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::Q_Max ) ) );
    }

    m_calcUnit = new MiscUnit( m_initUnit );

    m_calcUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::P ) ) );
    if ( type == Renderer::SurfaceMesh )
        m_calcUnit->addVariable( MiscSymbol( Misc::FloatType, m_scene->identifier( Scene::Q ) ) );

    m_calcUnit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::V_Pos ) ) );
    if ( attr == Renderer::RgbAttribute )
        m_calcUnit->addVariable( MiscSymbol( Misc::VectorType, m_scene->identifier( Scene::V_Color ) ) );
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

void ParametricMeshNode::setColor( const QColor& front, const QColor back )
{
    m_frontColor = front;
    m_backColor = back;
}

void ParametricMeshNode::setReverseWinding( bool on )
{
    m_reverseWinding = on;
}

static QVector4D colorToVector( const QColor& color )
{
    return QVector4D( color.redF(), color.greenF(), color.blueF(), 1.0 );
}

static QColor vectorToColor( const QVector4D& vector )
{
    return QColor::fromRgbF( qBound( 0.0, vector.x(), 1.0 ), qBound( 0.0, vector.y(), 1.0 ), qBound( 0.0, vector.z(), 1.0 ) );
}

bool ParametricMeshNode::calculate( const QMatrix4x4& matrix /*= QMatrix4x4()*/ )
{
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();

    MiscEngine* engine = m_scene->engine();

    m_initUnit->setVariable( m_scene->identifier( Scene::M_Matrix ), MiscValue( Misc::MatrixType, engine ) );

    if ( m_attributeType == Renderer::NoAttribute ) {
        if ( m_meshType == Renderer::CurveMesh )
            m_initUnit->setVariable( m_scene->identifier( Scene::M_Color ), MiscValue( colorToVector( m_frontColor ), engine ) );

        if ( m_meshType == Renderer::SurfaceMesh ) {
            m_initUnit->setVariable( m_scene->identifier( Scene::M_FrontColor ), MiscValue( colorToVector( m_frontColor ), engine ) );
            m_initUnit->setVariable( m_scene->identifier( Scene::M_BackColor ), MiscValue( colorToVector( m_backColor ), engine ) );
        }
    }

    m_initUnit->setVariable( m_scene->identifier( Scene::P_Min ), MiscValue( 0.0f, engine ) );
    m_initUnit->setVariable( m_scene->identifier( Scene::P_Max ), MiscValue( 1.0f, engine ) );

    if ( m_meshType == Renderer::SurfaceMesh ) {
        m_initUnit->setVariable( m_scene->identifier( Scene::Q_Min ), MiscValue( 0.0f, engine ) );
        m_initUnit->setVariable( m_scene->identifier( Scene::Q_Max ), MiscValue( 1.0f, engine ) );
    }

    foreach ( const MiscCode& code, m_initCodes ) {
        if ( !engine->execute( code ) )
            return false;
    }

    QMatrix4x4 meshMatrix = m_initUnit->variable( m_scene->identifier( Scene::M_Matrix ) ).toMatrix();

    m_matrix = meshMatrix * matrix;
    m_matrix.optimize();

    if ( m_attributeType == Renderer::NoAttribute ) {
        if ( m_meshType == Renderer::CurveMesh )
            m_realFrontColor = vectorToColor( m_initUnit->variable( m_scene->identifier( Scene::M_Color ) ).toVector() );

        if ( m_meshType == Renderer::SurfaceMesh ) {
            m_realFrontColor = vectorToColor( m_initUnit->variable( m_scene->identifier( Scene::M_FrontColor ) ).toVector() );
            m_realBackColor = vectorToColor( m_initUnit->variable( m_scene->identifier( Scene::M_BackColor ) ).toVector() );

            if ( m_reverseWinding )
                qSwap( m_realFrontColor, m_realBackColor );
        }
    }

    Tessellator* tessellator = Renderer::currentRenderer()->tessellator( m_meshType );

    float pMin = m_initUnit->variable( m_scene->identifier( Scene::P_Min ) ).toFloat();
    float pMax = m_initUnit->variable( m_scene->identifier( Scene::P_Max ) ).toFloat();
    tessellator->setPRange( pMin, pMax );

    if ( m_meshType == Renderer::SurfaceMesh ) {
        float qMin = m_initUnit->variable( m_scene->identifier( Scene::Q_Min ) ).toFloat();
        float qMax = m_initUnit->variable( m_scene->identifier( Scene::Q_Max ) ).toFloat();
        tessellator->setQRange( qMin, qMax );
    }

    if ( !tessellator->tessellate( this ) )
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

bool ParametricMeshNode::calculateVertex( float param, QVector3D* pos, QVector3D* attr )
{
    Q_ASSERT( m_meshType == Renderer::CurveMesh );

    MiscEngine* engine = m_scene->engine();

    m_calcUnit->setVariable( m_scene->identifier( Scene::P ), MiscValue( param, engine ) );

    return calculateCommon( pos, attr );
}

bool ParametricMeshNode::calculateVertex( const QVector2D& param, QVector3D* pos, QVector3D* attr )
{
    Q_ASSERT( m_meshType == Renderer::SurfaceMesh );

    MiscEngine* engine = m_scene->engine();

    m_calcUnit->setVariable( m_scene->identifier( Scene::P ), MiscValue( param.x(), engine ) );
    m_calcUnit->setVariable( m_scene->identifier( Scene::Q ), MiscValue( param.y(), engine ) );

    return calculateCommon( pos, attr );
}

bool ParametricMeshNode::calculateCommon( QVector3D* pos, QVector3D* attr )
{
    MiscEngine* engine = m_scene->engine();

    m_calcUnit->setVariable( m_scene->identifier( Scene::V_Pos ), MiscValue( Misc::VectorType, engine ) );
    if ( m_attributeType == Renderer::RgbAttribute )
        m_calcUnit->setVariable( m_scene->identifier( Scene::V_Color ), MiscValue( Misc::VectorType, engine ) );

    foreach ( const MiscCode& code, m_calcCodes ) {
        if ( !engine->execute( code ) )
            return false;
    }

    QVector3D position = m_calcUnit->variable( m_scene->identifier( Scene::V_Pos ) ).toVector().toVector3D();

    *pos = m_matrix.map( position );

    if ( m_attributeType == Renderer::RgbAttribute )
        *attr = m_calcUnit->variable( m_scene->identifier( Scene::V_Color ) ).toVector().toVector3D();

    return true;
}

void ParametricMeshNode::render()
{
    if ( m_attributeType == Renderer::NoAttribute )
        Renderer::currentRenderer()->setColor( m_realFrontColor, m_realBackColor );

    Renderer::currentRenderer()->renderMesh( m_meshType, m_attributeType, m_vertexBuffer, m_indexBuffer, m_count );
}
