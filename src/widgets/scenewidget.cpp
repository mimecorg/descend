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

#include "widgets/scenewidget.h"

#include "project/project.h"
#include "scene/glloader.h"
#include "scene/scene.h"
#include "scene/renderer.h"
#include "scene/tessellator.h"

#include <QMouseEvent>
#include <qmath.h>

SceneWidget::SceneWidget( QWidget* parent ) : QGLWidget( parent ),
    m_renderer( NULL ),
    m_project( NULL ),
    m_scene( NULL ),
    m_rotation( 0.0 ),
    m_angle( 0.0 ),
    m_tracking( NoTracking )
{
}

SceneWidget::~SceneWidget()
{
    delete m_renderer;
    delete m_scene;
}

void SceneWidget::setProject( Project* project )
{
    m_project = project;

    m_rotation = 0.0;
    m_angle = -45.0;

    updateSettings();
}

void SceneWidget::setScene( Scene* scene )
{
    if ( m_scene != scene ) {
        delete m_scene;
        m_scene = scene;
        update();
    }
}

static const double LightRotation = -0.3491;
static const double LightAngle = 0.4363;

void SceneWidget::initializeGL()
{
    GLLoader::initialize( context() );

    m_renderer = new Renderer();
    m_renderer->makeCurrent();

    m_renderer->setLightDirection( QVector3D( (float)( sin( LightRotation ) * cos( LightAngle ) ),
		(float)( sin( LightAngle ) ), (float)( cos( LightRotation ) * cos( LightAngle ) ) ) );
    m_renderer->setLightIntensity( 0.1f, 0.6f, 0.4f );
    m_renderer->setShininess( 40.0f );

    updateSettings();
}

void SceneWidget::updateSettings()
{
    if ( m_renderer != NULL && m_project != NULL ) {
        QColor color = m_project->setting( "BgColor" ).value<QColor>();
        QColor color2 = m_project->setting( "BgColor2" ).value<QColor>();

        m_renderer->setBackgroundColor( color, color2.isValid() ? color2 : color );

        int minLod = m_project->setting( "MinLod" ).toInt();
        int maxLod = m_project->setting( "MaxLod" ).toInt();
        float geometryThreshold = m_project->setting( "GeometryThreshold" ).toFloat();
        float attributeThreshold = m_project->setting( "AttributeThreshold" ).toFloat();

        Tessellator* tessellator = m_renderer->tessellator( Renderer::SurfaceMesh );
        tessellator->setLodRange( minLod, maxLod );
        tessellator->setGeometryThreshold( geometryThreshold );
        tessellator->setAttributeThreshold( attributeThreshold );

        Tessellator* tessellator2 = m_renderer->tessellator( Renderer::CurveMesh );
        tessellator2->setLodRange( minLod, maxLod );
        tessellator2->setGeometryThreshold( geometryThreshold );
        tessellator2->setAttributeThreshold( attributeThreshold );

        m_renderer->setEdgesVisible( m_project->setting( "DrawEdges" ).toBool() );

        update();
    }
}

void SceneWidget::resizeGL( int width, int height )
{
    glViewport( 0, 0, width, height );
}

static const double CameraDistance = 40.0;
static const double CameraZoom = 24.0;

static const double NearClipping = 10.0;
static const double FarClipping = 70.0;

void SceneWidget::paintGL()
{
    m_renderer->makeCurrent();

    m_renderer->clear();

    if ( m_scene != NULL ) {
        GLint viewport[ 4 ];
        glGetIntegerv( GL_VIEWPORT, viewport );

        double vw = viewport[ 2 ];
        double vh = viewport[ 3 ];

        double fy = tan( 0.5 * CameraZoom * M_PI / 180.0 ) * NearClipping;
        double fx = fy * vw / vh;

        QMatrix4x4 projection;
        projection.frustum( -fx, fx, -fy, fy, NearClipping, FarClipping );

        m_renderer->setProjectionMatrix( projection );

        QMatrix4x4 view;
        view.translate( 0.0, 0.0, -CameraDistance );
        view.rotate( m_angle, 1.0, 0.0, 0.0 );
        view.rotate( m_rotation, 0.0, 0.0, 1.0 );

        m_renderer->setViewMatrix( view );

        m_scene->render();
    }
}

void SceneWidget::mousePressEvent( QMouseEvent* e )
{
    if ( m_scene == NULL )
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
