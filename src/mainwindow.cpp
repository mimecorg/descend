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
#include "surfacetessellator.h"

#include "misc/miscengine.h"
#include "misc/miscunit.h"
#include "misc/misccode.h"
#include "misc/miscvalue.h"
#include "misc/miscidentifier.h"
#include "misc/miscsymbol.h"

#include <QVector2D>
#include <QVector3D>
#include <QElapsedTimer>

MainWindow::MainWindow() : QMainWindow()
{
    m_ui.setupUi( this );

    m_ui.minLodEdit->setText( "8" );
    m_ui.maxLodEdit->setText( "16" );
    m_ui.alphaEdit->setText( "5" );
}

MainWindow::~MainWindow()
{
}

class ParametricSurface : public AbstractSurface
{
public:
    ParametricSurface( const QString& text ) :
        m_unit( &m_engine ),
        m_p( "p", &m_engine ),
        m_q( "q", &m_engine ),
        m_vPos( "v_pos", &m_engine ),
        m_samples( 0 )
    {
        m_unit.addVariable( MiscSymbol( Misc::FloatType, m_p ) );
        m_unit.addVariable( MiscSymbol( Misc::FloatType, m_q ) );
        m_unit.addVariable( MiscSymbol( Misc::VectorType, m_vPos ) );
        m_code = m_unit.compileUnit( text );
    }

public:
    int samples() const { return m_samples; }

    bool isValid() const { return m_code.isValid(); }

    QString errorMessage() const { return m_engine.errorMessage(); }
    int errorLineNumber() const { return m_engine.errorLineNumber(); }

    QVector3D vertexFromParameter( const QVector2D& parameter )
    {
        m_samples++;

        m_unit.setVariable( m_p, MiscValue( (float)parameter.x(), &m_engine ) );
        m_unit.setVariable( m_q, MiscValue( (float)parameter.y(), &m_engine ) );
        m_unit.setVariable( m_vPos, MiscValue( Misc::VectorType, &m_engine ) );

        m_engine.execute( m_code );

        MiscValue pos = m_unit.variable( m_vPos );

        return pos.toVector().toVector3DAffine();
    }

private:
    MiscEngine m_engine;

    MiscUnit m_unit;
    MiscCode m_code;

    MiscIdentifier m_p;
    MiscIdentifier m_q;
    MiscIdentifier m_vPos;

    int m_samples;
};

void MainWindow::on_executeButton_clicked()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    m_ui.outputEdit->clear();

    QElapsedTimer timer;
    timer.start();

    ParametricSurface surface( m_ui.scriptEdit->toPlainText() );

    if ( surface.isValid() ) {
        SurfaceTessellator tessellator;

        int minLod = qBound( 0, m_ui.minLodEdit->text().toInt(), 20 );
        int maxLod = qBound( 0, m_ui.maxLodEdit->text().toInt(), 20 );
        float alpha = qBound( 0.0f, m_ui.alphaEdit->text().toFloat(), 45.0f );

        tessellator.tessellate( &surface, minLod, maxLod, alpha );

        m_ui.sceneWidget->setSurface( tessellator.vertices(), tessellator.normals(), tessellator.indices() );

        quint64 elapsed = timer.elapsed();

        m_ui.outputEdit->setPlainText( QString( "Vertices: %1\nTriangles: %2\nSamples: %3\nElapsed time: %4 ms" )
            .arg( tessellator.vertices().count() )
            .arg( tessellator.indices().count() )
            .arg( surface.samples() )
            .arg( elapsed ) );
    } else {
        m_ui.outputEdit->setPlainText( QString( "Error: %1 in line %2" )
            .arg( surface.errorMessage() )
            .arg( surface.errorLineNumber() ) );
    }

    QApplication::restoreOverrideCursor();
}

void MainWindow::on_edgesCheckBox_toggled( bool on )
{
    m_ui.sceneWidget->setEdges( on );
}
