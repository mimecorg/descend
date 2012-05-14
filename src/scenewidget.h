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

#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QGLWidget>

class Scene;
class Renderer;

class SceneWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit SceneWidget( QWidget* parent );
    ~SceneWidget();

public:
    void setScene( Scene* scene );
    Scene* scene() const { return m_scene; }

    void setEdges( bool on );
    bool hasEdges() const { return m_edges; }

protected: // overrides
    void initializeGL();
    void resizeGL( int width, int height );
    void paintGL();

    void mousePressEvent( QMouseEvent* e );
    void mouseMoveEvent( QMouseEvent* e );
    void mouseReleaseEvent( QMouseEvent* e );

private:
    enum Tracking
    {
        NoTracking,
        RotateXY
    };

private:
    Renderer* m_renderer;

    Scene* m_scene;

    bool m_edges;

    float m_rotation;
    float m_angle;

    Tracking m_tracking;
    QPoint m_lastPos;
};

#endif
