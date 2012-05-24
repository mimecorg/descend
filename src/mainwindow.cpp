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
#include "utils/iconloader.h"
#include "xmlui/builder.h"
#include "xmlui/toolstrip.h"

#include <QElapsedTimer>

MainWindow::MainWindow() : QMainWindow()
{
    setWindowTitle( tr( "Descend" ) );

    QAction* action;
    XmlUi::ToolStripAction* insertAction;

    action = new QAction( IconLoader::icon( "file-new" ), tr( "New" ), this );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_N ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( newFile() ) );
    setAction( "newFile", action );

    action = new QAction( IconLoader::icon( "file-open" ), tr( "Open..." ), this );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_O ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( openFile() ) );
    setAction( "openFile", action );

    action = new QAction( IconLoader::icon( "file-save" ), tr( "Save" ), this );
    setAction( "popupSaveFile", action );

    action = new QAction( IconLoader::icon( "file-save" ), tr( "&Save" ), this );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_S ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( saveFile() ) );
    setAction( "saveFile", action );

    action = new QAction( IconLoader::icon( "file-save-as" ), tr( "Save &As..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( saveFileAs() ) );
    setAction( "saveFileAs", action );

    insertAction = new XmlUi::ToolStripAction( IconLoader::icon( "edit-insert" ), tr( "Insert Node" ), this );
    insertAction->setShortcut( QKeySequence( Qt::Key_Insert ) );
    insertAction->setPopupMode( QToolButton::InstantPopup );
    connect( insertAction, SIGNAL( triggered() ), this, SLOT( insertNode() ) );
    setAction( "insertNode", insertAction );

    action = new QAction( IconLoader::icon( "node-definition" ), tr( "Insert &Definition..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( insertDefinition() ) );
    setAction( "insertDefinition", action );

    action = new QAction( IconLoader::icon( "node-scene" ), tr( "Insert &Scene..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( insertScene() ) );
    setAction( "insertScene", action );

    action = new QAction( IconLoader::icon( "node-curve" ), tr( "Insert &Curve..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( insertCurve() ) );
    setAction( "insertCurve", action );

    action = new QAction( IconLoader::icon( "node-surface" ), tr( "Insert &Surface..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( insertSurface() ) );
    setAction( "insertSurface", action );

    action = new QAction( IconLoader::icon( "node-group" ), tr( "Insert &Group..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( insertGroup() ) );
    setAction( "insertGroup", action );

    action = new QAction( IconLoader::icon( "edit-clone" ), tr( "Duplicate" ), this );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_D ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( cloneNode() ) );
    setAction( "cloneNode", action );

    action = new QAction( IconLoader::icon( "edit-rename" ), tr( "Rename" ), this );
    action->setShortcut( QKeySequence( Qt::Key_F2 ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( renameNode() ) );
    setAction( "renameNode", action );

    action = new QAction( IconLoader::icon( "edit-delete" ), tr( "Delete" ), this );
    action->setShortcut( QKeySequence( Qt::Key_Delete ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( deleteNode() ) );
    setAction( "deleteNode", action );

    action = new QAction( IconLoader::icon( "edit-up" ), tr( "Up" ), this );
    action->setToolTip( tr( "Move Up" ) );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Up ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( moveNodeUp() ) );
    setAction( "moveNodeUp", action );

    action = new QAction( IconLoader::icon( "edit-down" ), tr( "Down" ), this );
    action->setToolTip( tr( "Move Down" ) );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Down ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( moveNodeDown() ) );
    setAction( "moveNodeDown", action );

    action = new QAction( IconLoader::icon( "edit-properties" ), tr( "Properties" ), this );
    action->setShortcut( QKeySequence( Qt::Key_F4 ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( editProperties() ) );
    setAction( "editProperties", action );

    action = new QAction( IconLoader::icon( "descend" ), tr( "Draw Scene" ), this );
    action->setShortcut( QKeySequence( Qt::Key_F5 ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( drawScene() ) );
    setAction( "drawScene", action );

    action = new QAction( IconLoader::icon( "close" ), tr( "Close Scene" ), this );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_W ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( closeScene() ) );
    setAction( "closeScene", action );

    action = new QAction( IconLoader::icon( "camera" ), tr( "Camera Settings" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( cameraSettings() ) );
    setAction( "cameraSettings", action );

    action = new QAction( IconLoader::icon( "view-default" ), tr( "Default View" ), this );
    setAction( "popupDefaultView", action );

    action = new QAction( tr( "&Default" ), this );
    action->setShortcut( QKeySequence( Qt::ALT + Qt::Key_Home ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( defaultView() ) );
    setAction( "defaultView", action );

    action = new QAction( tr( "&Top" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( topView() ) );
    setAction( "topView", action );

    action = new QAction( tr( "&Front" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( frontView() ) );
    setAction( "frontView", action );

    action = new QAction( tr( "Top-&Left" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( topLeftView() ) );
    setAction( "topLeftView", action );

    action = new QAction( tr( "Top-&Right" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( topRightView() ) );
    setAction( "topRightView", action );

    action = new QAction( IconLoader::icon( "view-previous" ), tr( "Previous View" ), this );
    action->setShortcut( QKeySequence( Qt::ALT + Qt::Key_Left ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( previousView() ) );
    setAction( "previousView", action );

    action = new QAction( IconLoader::icon( "view-next" ), tr( "Next View" ), this );
    action->setShortcut( QKeySequence( Qt::ALT + Qt::Key_Right ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( nextView() ) );
    setAction( "nextView", action );

    action = new QAction( IconLoader::icon( "colors" ), tr( "Color Settings" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( colorSettings() ) );
    setAction( "colorSettings", action );

    action = new QAction( IconLoader::icon( "lighting" ), tr( "Lighting Settings" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( lightingSettings() ) );
    setAction( "lightingSettings", action );

    action = new QAction( IconLoader::icon( "tessellation" ), tr( "Tessellation Settings" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( tessellationSettings() ) );
    setAction( "tessellationSettings", action );

    setTitle( "sectionFile", tr( "File" ) );
    setTitle( "sectionEdit", tr( "Edit" ) );
    setTitle( "sectionScene", tr( "Scene" ) );
    setTitle( "sectionView", tr( "View" ) );
    setTitle( "sectionSettings", tr( "Settings" ) );

    setPopupMenu( "popupSaveFile", "menuSaveFile", "saveFile" );
    setPopupMenu( "popupDefaultView", "menuDefaultView", "defaultView" );

    loadXmlUiFile( ":/resources/mainwindow.xml" );

    XmlUi::ToolStrip* strip = new XmlUi::ToolStrip( this );
    setMenuWidget( strip );

    XmlUi::Builder* builder = new XmlUi::Builder( this );
    builder->registerToolStrip( "stripMain", strip );
    builder->addClient( this );

    insertAction->setMenu( builder->contextMenu( "menuInsert" ) );

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

void MainWindow::insertNode()
{
    builder()->toolStrip( "stripMain" )->execMenu( action( "insertNode" ) );
}
