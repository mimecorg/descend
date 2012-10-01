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
#include "dialogs/propertiesdialog.h"
#include "misc/miscengine.h"
#include "project/project.h"
#include "project/projectitemmodel.h"
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

    insertAction = new XmlUi::ToolStripAction( IconLoader::icon( "edit-insert" ), tr( "Insert Item" ), this );
    insertAction->setShortcut( QKeySequence( Qt::Key_Insert ) );
    insertAction->setPopupMode( QToolButton::InstantPopup );
    connect( insertAction, SIGNAL( triggered() ), this, SLOT( insertItem() ) );
    setAction( "insertItem", insertAction );

    action = new QAction( IconLoader::icon( "item-folder" ), tr( "Insert &Folder..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( insertFolder() ) );
    setAction( "insertFolder", action );

    action = new QAction( IconLoader::icon( "item-group" ), tr( "Insert &Group..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( insertGroup() ) );
    setAction( "insertGroup", action );

    action = new QAction( IconLoader::icon( "item-curve" ), tr( "Insert &Curve..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( insertCurve() ) );
    setAction( "insertCurve", action );

    action = new QAction( IconLoader::icon( "item-surface" ), tr( "Insert &Surface..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( insertSurface() ) );
    setAction( "insertSurface", action );

    action = new QAction( IconLoader::icon( "edit-clone" ), tr( "Duplicate" ), this );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_D ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( cloneItem() ) );
    setAction( "cloneItem", action );

    action = new QAction( IconLoader::icon( "edit-rename" ), tr( "Rename" ), this );
    action->setShortcut( QKeySequence( Qt::Key_F2 ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( renameItem() ) );
    setAction( "renameItem", action );

    action = new QAction( IconLoader::icon( "edit-delete" ), tr( "Delete" ), this );
    action->setShortcut( QKeySequence( Qt::Key_Delete ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( deleteItem() ) );
    setAction( "deleteItem", action );

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

    m_project = new Project();
    m_project->setName( "Project" );

    m_model = new ProjectItemModel( m_project, this );

    m_proxyModel = new QSortFilterProxyModel( this );
    m_proxyModel->setSourceModel( m_model );
    m_proxyModel->setSortLocaleAware( true );
    m_proxyModel->sort( 0, Qt::AscendingOrder );
    m_proxyModel->setDynamicSortFilter( true );

    m_ui.treeView->setModel( m_proxyModel );
    m_ui.treeView->setEditTriggers( QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed );
    m_ui.treeView->setRootIsDecorated( false );
    m_ui.treeView->setHeaderHidden( true );

    m_ui.treeView->expandAll();

    m_ui.treeView->setCurrentIndex( m_proxyModel->index( 0, 0 ) );

    connect( m_ui.treeView->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( updateActions() ) );

    updateActions();
}

MainWindow::~MainWindow()
{
    delete m_project;
}

void MainWindow::on_edgesCheckBox_toggled( bool on )
{
    m_ui.sceneWidget->setEdges( on );
}

void MainWindow::updateActions()
{
    QModelIndex index = m_proxyModel->mapToSource( m_ui.treeView->currentIndex() );
    ProjectItem* item = m_model->itemFromIndex( index );

    bool isEditable = item != NULL && item->type() != ProjectItem::Project;
    bool canInsertFolder = item != NULL && ( item->type() == ProjectItem::Project || item->type() == ProjectItem::Folder );
    bool canInsertGeometry = item != NULL && ( item->type() == ProjectItem::Project || item->type() == ProjectItem::Folder || item->type() == ProjectItem::Group );
    bool canDrawScene = item != NULL && ( item->type() == ProjectItem::Group || item->type() == ProjectItem::Curve || item->type() == ProjectItem::Surface );

    action( "newFile" )->setEnabled( false );
    action( "openFile" )->setEnabled( false );
    action( "popupSaveFile" )->setEnabled( false );

    action( "insertItem" )->setEnabled( canInsertFolder | canInsertGeometry );
    action( "insertFolder" )->setEnabled( canInsertFolder );
    action( "insertGroup" )->setEnabled( canInsertGeometry );
    action( "insertCurve" )->setEnabled( canInsertGeometry );
    action( "insertSurface" )->setEnabled( canInsertGeometry );

    action( "cloneItem" )->setEnabled( false );
    action( "renameItem" )->setEnabled( isEditable );
    action( "deleteItem" )->setEnabled( isEditable );
    action( "editProperties" )->setEnabled( item != NULL );
    action( "drawScene" )->setEnabled( canDrawScene );
    action( "closeScene" )->setEnabled( m_ui.sceneWidget->scene() != NULL );
    action( "cameraSettings" )->setEnabled( false );
    action( "popupDefaultView" )->setEnabled( false );
    action( "previousView" )->setEnabled( false );
    action( "nextView" )->setEnabled( false );
    action( "colorSettings" )->setEnabled( false );
    action( "lightingSettings" )->setEnabled( false );
    action( "tessellationSettings" )->setEnabled( false );
}

void MainWindow::insertItem()
{
    builder()->toolStrip( "stripMain" )->execMenu( action( "insertItem" ) );
}

void MainWindow::insertFolder()
{
    insertItem( ProjectItem::Folder, tr( "Folder" ) );
}

void MainWindow::insertGroup()
{
    insertItem( ProjectItem::Group, tr( "Group" ) );
}

void MainWindow::insertCurve()
{
    insertItem( ProjectItem::Curve, tr( "Curve" ) );
}

void MainWindow::insertSurface()
{
    insertItem( ProjectItem::Surface, tr( "Surface" ) );
}

void MainWindow::insertItem( ProjectItem::Type type, const QString& name )
{
    QModelIndex parent = m_proxyModel->mapToSource( m_ui.treeView->currentIndex() );

    if ( parent.isValid() ) {
        QModelIndex index = m_model->insertItem( type, name, parent );

        QModelIndex current = m_proxyModel->mapFromSource( index );

        m_ui.treeView->setCurrentIndex( current );
        m_ui.treeView->edit( current );
    }
}

void MainWindow::renameItem()
{
    m_ui.treeView->edit( m_ui.treeView->currentIndex() );
}

void MainWindow::deleteItem()
{
    QModelIndex index = m_proxyModel->mapToSource( m_ui.treeView->currentIndex() );

    m_model->deleteItem( index );
}

void MainWindow::editProperties()
{
    QModelIndex index = m_proxyModel->mapToSource( m_ui.treeView->currentIndex() );
    ProjectItem* item = m_model->itemFromIndex( index );

    if ( item ) {
        PropertiesDialog dialog( item, this );
        dialog.exec();
    }
}

void MainWindow::drawScene()
{
    QModelIndex index = m_proxyModel->mapToSource( m_ui.treeView->currentIndex() );
    ProjectItem* item = m_model->itemFromIndex( index );

    if ( !item )
        return;

    m_ui.sceneWidget->setScene( NULL );

    QApplication::setOverrideCursor( Qt::WaitCursor );

    m_ui.outputEdit->clear();

    int minLod = qBound( 0, m_ui.minLodEdit->text().toInt(), 20 );
    int maxLod = qBound( 0, m_ui.maxLodEdit->text().toInt(), 20 );
    float threshold = qBound( 0.0f, m_ui.alphaEdit->text().toFloat(), 1.0f );

    Renderer::currentRenderer()->tessellator( Renderer::SurfaceMesh )->setLodRange( minLod, maxLod );
    Renderer::currentRenderer()->tessellator( Renderer::SurfaceMesh )->setGeometryThreshold( threshold );
    Renderer::currentRenderer()->tessellator( Renderer::SurfaceMesh )->setAttributeThreshold( threshold );

    Renderer::currentRenderer()->tessellator( Renderer::CurveMesh )->setLodRange( minLod, maxLod );
    Renderer::currentRenderer()->tessellator( Renderer::CurveMesh )->setGeometryThreshold( threshold );
    Renderer::currentRenderer()->tessellator( Renderer::CurveMesh )->setAttributeThreshold( threshold );

    bool ok = false;

    Scene* scene = new Scene();

    if ( m_project->initializeScene( scene, item ) ) {
        QElapsedTimer timer;
        timer.start();

        SceneNodeContext context;
        context.setColor( 0, QColor( 255, 64, 144 ) );
        context.setColor( 1, QColor( 255, 144, 64 ) );

        if ( scene->calculate( context ) ) {
            m_ui.outputEdit->setPlainText( QString( "Elements: %1\nElapsed time: %2 ms" )
                .arg( scene->elementsCount() )
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

    m_model->setBoldItem( item );

    updateActions();
}

void MainWindow::closeScene()
{
    m_ui.sceneWidget->setScene( NULL );

    m_ui.outputEdit->clear();

    m_model->setBoldItem( NULL );

    updateActions();
}
