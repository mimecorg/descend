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

#include <QGLShaderProgram>
#include <QMouseEvent>

#include <math.h>

SceneWidget::SceneWidget( QWidget* parent ) : QGLWidget( parent ),
    m_program( NULL ),
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
    m_vertices = vertices;
    m_normals = normals;
    m_indices = indices;

    glVertexPointer( 3, GL_FLOAT, 0, m_vertices.constData() );
    glEnableClientState( GL_VERTEX_ARRAY );

    glNormalPointer( GL_FLOAT, 0, m_normals.constData() );
    glEnableClientState( GL_NORMAL_ARRAY );

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
    glFrontFace( GL_CCW );

    glShadeModel( GL_SMOOTH );

    glPolygonOffset( 1.5f, 0.0f );

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    delete m_program;
    m_program = new QGLShaderProgram( this );

    m_program->addShaderFromSourceFile( QGLShader::Vertex, ":/shaders/vertex.glsl" );
    m_program->addShaderFromSourceFile( QGLShader::Fragment, ":/shaders/fragment.glsl" );

    m_program->link();
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

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glDisable( GL_DEPTH_TEST );

    glBegin( GL_QUADS );

    glColor3f( 0.0f, 0.1f, 0.7f );
    glVertex2f( 1.0f, 1.0f );
    glVertex2f( -1.0f, 1.0f );

    glColor3f( 0.2f, 0.5f, 1.0f );
    glVertex2f( -1.0f, -1.0f );
    glVertex2f( 1.0f, -1.0f );

    glEnd();

    if ( m_indices.isEmpty() )
        return;

    GLint viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );

    double vw = viewport[ 2 ];
    double vh = viewport[ 3 ];

    double fy = tan( 0.5 * CameraZoom * M_PI / 180.0 ) * NearClipping;
    double fx = fy * vw / vh;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -fx, fx, -fy, fy, NearClipping, FarClipping );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslated( 0.0, 0.0, -CameraDistance );
    glRotated( m_angle, 1.0, 0.0, 0.0 );
    glRotated( m_rotation, 0.0, 0.0, 1.0 );

    glEnable( GL_DEPTH_TEST );

    m_program->bind();

    m_program->setUniformValue( "Color", QColor( 255, 128, 200 ) );

    m_program->setUniformValue( "LightDirection", QVector3D( (float)( sin( LightRotation ) * cos( LightAngle ) ),
		(float)( sin( LightAngle ) ), (float)( cos( LightRotation ) * cos( LightAngle ) ) ) );

    m_program->setUniformValue( "AmbientIntensity", 0.3f );
    m_program->setUniformValue( "DiffuseIntensity", 0.65f );
    m_program->setUniformValue( "SpecularIntensity", 0.4f );
    m_program->setUniformValue( "Shininess", 40.0f );

    if ( m_edges )
        glEnable( GL_POLYGON_OFFSET_FILL );

    glDrawElements( GL_TRIANGLES, 3 * m_indices.count(), GL_UNSIGNED_INT, m_indices.constData() );

    if ( m_edges )
        glDisable( GL_POLYGON_OFFSET_FILL );

    if ( m_edges ) {
	    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );

        m_program->release();

        glDrawElements( GL_TRIANGLES, 3 * m_indices.count(), GL_UNSIGNED_INT, m_indices.constData() );
    }

    m_program->release();
}

void SceneWidget::mousePressEvent( QMouseEvent* e )
{
    if ( m_indices.isEmpty() )
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
