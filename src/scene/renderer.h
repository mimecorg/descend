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

#ifndef RENDERER_H
#define RENDERER_H

class Tessellator;

class Renderer : public QObject
{
    Q_OBJECT
public:
    enum MeshType
    {
        CurveMesh,
        SurfaceMesh
    };

    enum AttributeType
    {
        NoAttribute,
        RgbAttribute
    };

public:
    Renderer();
    ~Renderer();

public:
    void makeCurrent();
    void doneCurrent();

    static Renderer* currentRenderer() { return m_current; }

    Tessellator* tessellator( MeshType type ) const;

    void setBackgroundColor( const QColor& top, const QColor& bottom );

    void setViewMatrix( const QMatrix4x4& matrix );
    void setProjectionMatrix( const QMatrix4x4& matrix );

    void setLightDirection( const QVector3D& direction );
    void setLightIntensity( float ambient, float diffuse, float specular );
    void setShininess( float shininess );

    void setEdgesVisible( bool on );

    void setColor( const QColor& frontColor, const QColor& backColor );

    void clear();

    void renderMesh( MeshType type, AttributeType attr, QGLBuffer& vertexBuffer, QGLBuffer& indexBuffer, int count );

private:
    QMap<QString, QGLShaderProgram*> m_programs;

    Tessellator* m_curveTessellator;
    Tessellator* m_surfaceTessellator;

    QColor m_topColor;
    QColor m_bottomColor;

    QMatrix4x4 m_viewMatrix;
    QMatrix3x3 m_normalMatrix;
    QMatrix4x4 m_projectionMatrix;

    QVector3D m_lightDirection;
    float m_ambientIntensity;
    float m_diffuseIntensity;
    float m_specularIntensity;
    float m_shininess;

    bool m_edgesVisible;

    QColor m_frontColor;
    QColor m_backColor;

    static Renderer* m_current;
};

#endif
