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

#ifndef PARAMETRICMESHNODE_H
#define PARAMETRICMESHNODE_H

#include "misc/misccode.h"
#include "scene/scenenode.h"
#include "scene/renderer.h"

class ParametricMeshNode : public SceneNode
{
public:
    ParametricMeshNode( Renderer::MeshType type, Renderer::AttributeType attr, SceneNode* parent );
    ~ParametricMeshNode();

public:
    Renderer::MeshType meshType() const { return m_meshType; }
    Renderer::AttributeType attributeType() const { return m_attributeType; }

    bool addInitCode( const QString& text );
    bool addCalcCode( const QString& text );

    void setColor( const QColor& front, const QColor back );
    void setReverseWinding( bool on );

    bool calculateVertex( float param, QVector3D* pos, QVector3D* attr );
    bool calculateVertex( const QVector2D& param, QVector3D* pos, QVector3D* attr );

    int elementsCount() const { return m_count; }

public:
    MiscUnit* unit() const { return NULL; }

    bool calculate( const QMatrix4x4& matrix = QMatrix4x4() );

    void render();

protected:
    bool calculateCommon( QVector3D* pos, QVector3D* attr );

protected:
    Renderer::MeshType m_meshType;
    Renderer::AttributeType m_attributeType;

    QColor m_frontColor;
    QColor m_backColor;
    bool m_reverseWinding;

    MiscUnit* m_initUnit;
    MiscUnit* m_calcUnit;

    QList<MiscCode> m_initCodes;
    QList<MiscCode> m_calcCodes;

    QColor m_realFrontColor;
    QColor m_realBackColor;

    QMatrix4x4 m_matrix;

    QGLBuffer m_vertexBuffer;
    QGLBuffer m_indexBuffer;

    int m_count;
};

#endif
