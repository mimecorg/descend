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

#include "scene/renderer.h"

#include "scene/curvetessellator.h"
#include "scene/glloader.h"
#include "scene/surfacetessellator.h"

Renderer* Renderer::m_current = NULL;

static const char* const shaders[] = { "lit", "lit-rgb", "plain", "plain-rgb" };

Renderer::Renderer() :
    m_ambientIntensity( 0 ),
    m_diffuseIntensity( 0 ),
    m_specularIntensity( 0 ),
    m_shininess( 0 ),
    m_edgesVisible( false )
{
    for ( int i = 0; i < sizeof( shaders ) / sizeof( shaders[ 0 ] ); i++ ) {
        QGLShaderProgram* program = new QGLShaderProgram( this );
        program->addShaderFromSourceFile( QGLShader::Vertex, QString( ":/shaders/%1.vert" ).arg( shaders[ i ] ) );
        program->addShaderFromSourceFile( QGLShader::Fragment, QString( ":/shaders/%1.frag" ).arg( shaders[ i ] ) );
        program->link();
        m_programs.insert( shaders[ i ], program );
    }

    m_curveTessellator = new CurveTessellator();
    m_surfaceTessellator = new SurfaceTessellator();

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_FRAMEBUFFER_SRGB );

    glPolygonOffset( 1.5f, 0.0f );
}

Renderer::~Renderer()
{
    doneCurrent();

    delete m_curveTessellator;
    delete m_surfaceTessellator;
}

void Renderer::makeCurrent()
{
    m_current = this;
}

void Renderer::doneCurrent()
{
    if ( m_current == this )
        m_current = NULL;
}

Tessellator* Renderer::tessellator( MeshType type ) const
{
    switch ( type ) {
        case CurveMesh:
            return m_curveTessellator;
        case SurfaceMesh:
            return m_surfaceTessellator;
        default:
            Q_ASSERT( false );
            return NULL;
    }
}

void Renderer::setBackgroundColor( const QColor& top, const QColor& bottom )
{
    m_topColor = top;
    m_bottomColor = bottom;
}

void Renderer::setViewMatrix( const QMatrix4x4& matrix )
{
    m_viewMatrix = matrix;
    m_normalMatrix = matrix.normalMatrix();
}

void Renderer::setProjectionMatrix( const QMatrix4x4& matrix )
{
    m_projectionMatrix = matrix;
}

void Renderer::setLightDirection( const QVector3D& direction )
{
    m_lightDirection = direction;
}

void Renderer::setLightIntensity( float ambient, float diffuse, float specular )
{
    m_ambientIntensity = ambient;
    m_diffuseIntensity = diffuse;
    m_specularIntensity = specular;
}

void Renderer::setShininess( float shininess )
{
    m_shininess = shininess;
}

void Renderer::setEdgesVisible( bool on )
{
    m_edgesVisible = on;
}

void Renderer::setColor( const QColor& frontColor, const QColor& backColor )
{
    m_frontColor = frontColor;
    m_backColor = backColor;
}

void Renderer::clear()
{
    glClear( GL_DEPTH_BUFFER_BIT );

    QGLBuffer::release( QGLBuffer::VertexBuffer );
    QGLBuffer::release( QGLBuffer::IndexBuffer );

    const float data[ 6 * 4 ] =
    {
        1.0f, 1.0f, 0.0f,
        m_topColor.redF(), m_topColor.greenF(), m_topColor.blueF(),
        -1.0f, 1.0f, 0.0f,
        m_topColor.redF(), m_topColor.greenF(), m_topColor.blueF(),
        -1.0f, -1.0f, 0.0f,
        m_bottomColor.redF(), m_bottomColor.greenF(), m_bottomColor.blueF(),
        1.0f, -1.0f, 0.0f,
        m_bottomColor.redF(), m_bottomColor.greenF(), m_bottomColor.blueF()
    };

    QGLShaderProgram* program = m_programs[ "plain-rgb" ];
    program->bind();

    program->setUniformValue( "ViewMatrix", QMatrix4x4() );
    program->setUniformValue( "ProjectionMatrix", QMatrix4x4() );

    program->setAttributeArray( "inPosition", GL_FLOAT, data, 3, 6 * sizeof( float ) );
    program->enableAttributeArray( "inPosition" );

    program->setAttributeArray( "inColor", GL_FLOAT, data + 3, 3, 6 * sizeof( float ) );
    program->enableAttributeArray( "inColor" );

    glDisable( GL_DEPTH_TEST );

    glDrawArrays( GL_QUADS, 0, 4 );

    glEnable( GL_DEPTH_TEST );

    program->disableAttributeArray( "inPosition" );
    program->disableAttributeArray( "inColor" );

    program->release();
}

static QString programName( Renderer::MeshType type, Renderer::AttributeType attr )
{
    QString name;

    switch ( type ) {
        case Renderer::CurveMesh:
            name = "plain";
            break;
        case Renderer::SurfaceMesh:
            name = "lit";
            break;
        default:
            Q_ASSERT( false );
            break;
    }

    if ( attr == Renderer::RgbAttribute )
        name += QLatin1String( "-rgb" );

    return name;
}

static int vertexSize( Renderer::MeshType type, Renderer::AttributeType attr )
{
    int size = 3 * sizeof( float );

    if ( type == Renderer::SurfaceMesh )
        size += 3 * sizeof( float );

    if ( attr == Renderer::RgbAttribute )
        size += 3 * sizeof( float );

    return size;
}

void Renderer::renderMesh( MeshType type, AttributeType attr, QGLBuffer& vertexBuffer, QGLBuffer& indexBuffer, int count )
{
    vertexBuffer.bind();
    indexBuffer.bind();

    QGLShaderProgram* program = m_programs[ programName( type, attr ) ];

    program->bind();

    program->setUniformValue( "ViewMatrix", m_viewMatrix );
    program->setUniformValue( "ProjectionMatrix", m_projectionMatrix );

    if ( type == SurfaceMesh ) {
        program->setUniformValue( "NormalMatrix", m_normalMatrix );
        program->setUniformValue( "LightDirection", m_lightDirection );
        program->setUniformValue( "AmbientIntensity", m_ambientIntensity );
        program->setUniformValue( "DiffuseIntensity", m_diffuseIntensity );
        program->setUniformValue( "SpecularIntensity", m_specularIntensity );
        program->setUniformValue( "Shininess", m_shininess );
    }

    if ( attr == NoAttribute ) {
        program->setUniformValue( "FrontColor", m_frontColor );
        program->setUniformValue( "BackColor", m_backColor );
    }

    int size = vertexSize( type, attr );
    int offset = 0;

    program->setAttributeBuffer( "inPosition", GL_FLOAT, offset, 3, size );
    program->enableAttributeArray( "inPosition" );
    offset += 3 * sizeof( float );

    if ( type == SurfaceMesh ) {
        program->setAttributeBuffer( "inNormal", GL_FLOAT, offset, 3, size );
        program->enableAttributeArray( "inNormal" );
        offset += 3 * sizeof( float );
    }

    if ( attr == RgbAttribute ) {
        program->setAttributeBuffer( "inColor", GL_FLOAT, offset, 3, size );
        program->enableAttributeArray( "inColor" );
        offset += 3 * sizeof( float );
    }

    Q_ASSERT( offset == size );

    if ( m_edgesVisible && type == SurfaceMesh )
        glEnable( GL_POLYGON_OFFSET_FILL );

    switch ( type ) {
        case CurveMesh:
            glDrawElements( GL_LINES, 2 * count, GL_UNSIGNED_INT, 0 );
            break;
        case SurfaceMesh:
            glDrawElements( GL_TRIANGLES, 3 * count, GL_UNSIGNED_INT, 0 );
            break;
        default:
            Q_ASSERT( false );
            break;
    }

    program->disableAttributeArray( "inPosition" );
    program->disableAttributeArray( "inNormal" );
    if ( attr == RgbAttribute )
        program->disableAttributeArray( "inColor" );

    program->release();

    if ( m_edgesVisible && type == SurfaceMesh ) {
        glDisable( GL_POLYGON_OFFSET_FILL );

        program = m_programs[ "plain" ];
        program->bind();

        program->setUniformValue( "ViewMatrix", m_viewMatrix );
        program->setUniformValue( "ProjectionMatrix", m_projectionMatrix );

        program->setUniformValue( "FrontColor", QColor( 0, 0, 0 ) );
        program->setUniformValue( "BackColor", QColor( 0, 0, 0 ) );

        program->setAttributeBuffer( "inPosition", GL_FLOAT, 0, 3, size );
        program->enableAttributeArray( "inPosition" );

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        glDrawElements( GL_TRIANGLES, 3 * count, GL_UNSIGNED_INT, 0 );

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        program->disableAttributeArray( "inPosition" );

        program->release();
    }

    vertexBuffer.release();
    indexBuffer.release();
}
