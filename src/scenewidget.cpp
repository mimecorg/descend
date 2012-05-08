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

#include "scenewidget.h"

#include "scene/glcore.h"

#include <QGLShaderProgram>
#include <QMouseEvent>

#include <math.h>

SceneWidget::SceneWidget( QWidget* parent ) : QGLWidget( parent ),
    m_vertexBuffer( QGLBuffer::VertexBuffer ),
    m_indexBuffer( QGLBuffer::IndexBuffer ),
    m_triangles( 0 ),
    m_edges( false ),
    m_rotation( 0.0 ),
    m_angle( -45.0 ),
    m_tracking( NoTracking )
{
}

SceneWidget::~SceneWidget()
{
}

void SceneWidget::setSurface( const QVector<QVector3D>& vertices, const QVector<QVector3D>& normals, const QVector<Triangle>& indices )
{
    makeCurrent();

    int vcount = 3 * vertices.count() * sizeof( float );

    m_vertexBuffer.bind();
    m_vertexBuffer.allocate( 2 * vcount );
    m_vertexBuffer.write( 0, vertices.constData(), vcount );
    m_vertexBuffer.write( vcount, normals.constData(), vcount );

    int icount = 3 * indices.count() * sizeof( int );

    m_indexBuffer.bind();
    m_indexBuffer.allocate( indices.constData(), icount );

    m_program.bind();
    m_program.setAttributeBuffer( "inPosition", GL_FLOAT, 0, 3 );
    m_program.enableAttributeArray( "inPosition" );
    m_program.setAttributeBuffer( "inNormal", GL_FLOAT, vcount, 3 );
    m_program.enableAttributeArray( "inNormal" );

    m_program.release();
    m_indexBuffer.release();
    m_vertexBuffer.release();

    m_triangles = indices.count();

    updateGL();
}

void SceneWidget::setEdges( bool on )
{
    if ( m_edges != on ) {
        m_edges = on;
        updateGL();
    }
}

void SceneWidget::initializeGL()
{
    m_program.addShaderFromSourceFile( QGLShader::Vertex, ":/shaders/lit.vert" );
    m_program.addShaderFromSourceFile( QGLShader::Fragment, ":/shaders/lit.frag" );
    m_program.link();

    m_vertexBuffer.create();
    m_indexBuffer.create();

    glEnable( GL_FRAMEBUFFER_SRGB );

    glPolygonOffset( 1.5f, 0.0f );
}

void SceneWidget::resizeGL( int width, int height )
{
    glViewport( 0, 0, width, height );
}

static const double CameraDistance = 40.0;
static const double CameraZoom = 24.0;

static const double NearClipping = 10.0;
static const double FarClipping = 70.0;

static const double LightRotation = -0.3491;
static const double LightAngle = 0.4363;

void SceneWidget::paintGL()
{
    glClear( GL_DEPTH_BUFFER_BIT );

    glBegin( GL_QUADS );

    glColor3f( 0.0f, 0.1f, 0.7f );
    glVertex2f( 1.0f, 1.0f );
    glVertex2f( -1.0f, 1.0f );

    glColor3f( 0.2f, 0.5f, 1.0f );
    glVertex2f( -1.0f, -1.0f );
    glVertex2f( 1.0f, -1.0f );

    glEnd();

    if ( m_triangles == 0 )
        return;

    GLint viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );

    double vw = viewport[ 2 ];
    double vh = viewport[ 3 ];

    double fy = tan( 0.5 * CameraZoom * M_PI / 180.0 ) * NearClipping;
    double fx = fy * vw / vh;

    QMatrix4x4 projection;
    projection.frustum( -fx, fx, -fy, fy, NearClipping, FarClipping );

    QMatrix4x4 view;
    view.translate( 0.0, 0.0, -CameraDistance );
    view.rotate( m_angle, 1.0, 0.0, 0.0 );
    view.rotate( m_rotation, 0.0, 0.0, 1.0 );

    QMatrix3x3 normal = view.normalMatrix();

    glEnable( GL_DEPTH_TEST );

    m_vertexBuffer.bind();
    m_indexBuffer.bind();

    m_program.bind();

    m_program.setUniformValue( "ProjectionMatrix", projection );
    m_program.setUniformValue( "ViewMatrix", view );
    m_program.setUniformValue( "NormalMatrix", normal );

    m_program.setUniformValue( "FrontColor", QColor( 255, 64, 144 ) );
    m_program.setUniformValue( "BackColor", QColor( 255, 144, 64 ) );

    m_program.setUniformValue( "LightDirection", QVector3D( (float)( sin( LightRotation ) * cos( LightAngle ) ),
		(float)( sin( LightAngle ) ), (float)( cos( LightRotation ) * cos( LightAngle ) ) ) );

    m_program.setUniformValue( "AmbientIntensity", 0.1f );
    m_program.setUniformValue( "DiffuseIntensity", 0.6f );
    m_program.setUniformValue( "SpecularIntensity", 0.4f );
    m_program.setUniformValue( "Shininess", 40.0f );

    if ( m_edges )
        glEnable( GL_POLYGON_OFFSET_FILL );

    glDrawElements( GL_TRIANGLES, 3 * m_triangles, GL_UNSIGNED_INT, 0 );

    if ( m_edges )
        glDisable( GL_POLYGON_OFFSET_FILL );

    if ( m_edges ) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        m_program.setUniformValue( "FrontColor", QColor( 0, 0, 0 ) );
        m_program.setUniformValue( "BackColor", QColor( 0, 0, 0 ) );
        m_program.setUniformValue( "SpecularIntensity", 0.0f );

        glDrawElements( GL_TRIANGLES, 3 * m_triangles, GL_UNSIGNED_INT, 0 );

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    m_program.release();

    m_vertexBuffer.release();
    m_indexBuffer.release();

    glDisable( GL_DEPTH_TEST );
}

void SceneWidget::mousePressEvent( QMouseEvent* e )
{
    if ( m_triangles == 0 )
        return;

    if ( m_tracking != NoTracking ) {
        m_tracking = NoTracking;
        return;
    }

    if ( e->button() == Qt::LeftButton )
        m_tracking = RotateXY;

    m_lastPos = e->pos();
}

void SceneWidget::mouseMoveEvent( QMouseEvent* e )
{
    if ( m_tracking == NoTracking )
        return;

    switch ( m_tracking ) {
        case RotateXY:
            m_rotation += 0.4 * ( e->pos().x() - m_lastPos.x() );
            m_angle += 0.4 * ( e->pos().y() - m_lastPos.y() );
            updateGL();
            break;

        default:
            break;
    }

    m_lastPos = e->pos();
}

void SceneWidget::mouseReleaseEvent( QMouseEvent* /*e*/ )
{
    m_tracking = NoTracking;
}
