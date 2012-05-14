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

#include "mainwindow.h"

#include "scenewidget.h"
#include "misc/miscengine.h"
#include "scene/parametricmeshnode.h"
#include "scene/scene.h"
#include "scene/tessellator.h"

#include <QElapsedTimer>

MainWindow::MainWindow() : QMainWindow()
{
    m_ui.setupUi( this );

    m_ui.minLodEdit->setText( "8" );
    m_ui.maxLodEdit->setText( "16" );
    m_ui.alphaEdit->setText( "0.05" );
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_executeButton_clicked()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    m_ui.outputEdit->clear();

    int minLod = qBound( 0, m_ui.minLodEdit->text().toInt(), 20 );
    int maxLod = qBound( 0, m_ui.maxLodEdit->text().toInt(), 20 );
    float threshold = qBound( 0.0f, m_ui.alphaEdit->text().toFloat(), 1.0f );

    Renderer::currentRenderer()->tessellator( Renderer::SurfaceMesh )->setLodRange( minLod, maxLod );
    Renderer::currentRenderer()->tessellator( Renderer::SurfaceMesh )->setGeometryThreshold( threshold );
    Renderer::currentRenderer()->tessellator( Renderer::SurfaceMesh )->setAttributeThreshold( threshold );

    Scene* scene = new Scene();

    ParametricMeshNode* node = new ParametricMeshNode( Renderer::SurfaceMesh, Renderer::NoAttribute, scene );

    node->setColor( QColor( 255, 64, 144 ), QColor( 255, 144, 64 ) );

    bool ok = false;

    if ( node->addCalcCode( m_ui.scriptEdit->toPlainText() ) ) {
        QElapsedTimer timer;
        timer.start();

        if ( scene->calculate() ) {
            m_ui.outputEdit->setPlainText( QString( "Triangles: %1\nElapsed time: %2 ms" )
                .arg( node->elementsCount() )
                .arg( timer.elapsed() ) );

            m_ui.sceneWidget->setScene( scene );

            ok = true;
        }
    }

    if ( !ok ) {
        m_ui.outputEdit->setPlainText( QString( "Error: %1 in line %2" )
            .arg( scene->engine()->errorMessage() )
            .arg( scene->engine()->errorLineNumber() ) );

        delete scene;
    }

    QApplication::restoreOverrideCursor();
}

void MainWindow::on_edgesCheckBox_toggled( bool on )
{
    m_ui.sceneWidget->setEdges( on );
}
