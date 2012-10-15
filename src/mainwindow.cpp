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

#include "application.h"
#include "dialogs/newprojectdialog.h"
#include "dialogs/propertiesdialog.h"
#include "dialogs/colorsettingsdialog.h"
#include "dialogs/tessellationdialog.h"
#include "misc/miscengine.h"
#include "project/project.h"
#include "project/projectitemmodel.h"
#include "scene/parametricmeshnode.h"
#include "scene/scene.h"
#include "scene/tessellator.h"
#include "utils/iconloader.h"
#include "utils/localsettings.h"
#include "widgets/statuslabel.h"
#include "xmlui/builder.h"
#include "xmlui/toolstrip.h"

#include <QElapsedTimer>
#include <QStatusBar>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QCloseEvent>
#include <QAccessible>

MainWindow::MainWindow() : QMainWindow(),
    m_project( NULL ),
    m_model( NULL ),
    m_modified( false )
{
    setWindowTitle( tr( "Descend" ) );

    QAction* action;
    XmlUi::ToolStripAction* insertAction;

    action = new QAction( IconLoader::icon( "about" ), tr( "About WebIssues" ), this );
    action->setShortcut( QKeySequence( Qt::Key_F1 ) );
    connect( action, SIGNAL( triggered() ), application, SLOT( about() ) );
    setAction( "about", action );

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

    /*
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
    */

    action = new QAction( IconLoader::icon( "colors" ), tr( "Color Settings" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( colorSettings() ) );
    setAction( "colorSettings", action );

    /*
    action = new QAction( IconLoader::icon( "lighting" ), tr( "Lighting Settings" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( lightingSettings() ) );
    setAction( "lightingSettings", action );
    */

    action = new QAction( IconLoader::icon( "tessellation" ), tr( "Tessellation Settings" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( tessellationSettings() ) );
    setAction( "tessellationSettings", action );

    setTitle( "sectionFile", tr( "File" ) );
    setTitle( "sectionEdit", tr( "Edit" ) );
    setTitle( "sectionScene", tr( "Scene" ) );
    //setTitle( "sectionView", tr( "View" ) );
    setTitle( "sectionSettings", tr( "Settings" ) );

    setPopupMenu( "popupSaveFile", "menuSaveFile", "saveFile" );
    //setPopupMenu( "popupDefaultView", "menuDefaultView", "defaultView" );

    loadXmlUiFile( ":/resources/mainwindow.xml" );

    XmlUi::ToolStrip* strip = new XmlUi::ToolStrip( this );
    strip->addAuxiliaryAction( this->action( "about" ) );
    setMenuWidget( strip );

    XmlUi::Builder* builder = new XmlUi::Builder( this );
    builder->registerToolStrip( "stripMain", strip );
    builder->addClient( this );

    insertAction->setMenu( builder->contextMenu( "menuInsert" ) );

    m_ui.setupUi( this );

    QStatusBar* bar = statusBar();

    m_statusLabel = new StatusLabel( bar );
    bar->addWidget( m_statusLabel, 1 );

    m_proxyModel = new QSortFilterProxyModel( this );
    m_proxyModel->setSortLocaleAware( true );
    m_proxyModel->sort( 0, Qt::AscendingOrder );
    m_proxyModel->setDynamicSortFilter( true );

    m_ui.treeView->setModel( m_proxyModel );

    connect( m_ui.treeView->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( updateActions() ) );
    connect( m_ui.treeView, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( showContextMenu( const QPoint& ) ) );
    connect( m_ui.treeView, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( doubleClicked( const QModelIndex& ) ) );

    newFile( QString() );

    LocalSettings* settings = application->applicationSettings();

    if ( settings->contains( "MainWindowGeometry" ) )
        restoreGeometry( settings->value( "MainWindowGeometry" ).toByteArray() );
    else
        setWindowState( Qt::WindowMaximized );

    if ( settings->contains( "SplitterState" ) )
        m_ui.splitter->restoreState( settings->value( "SplitterState" ).toByteArray() );
}

MainWindow::~MainWindow()
{
    delete m_project;

    LocalSettings* settings = application->applicationSettings();

    settings->setValue( "MainWindowGeometry", saveGeometry() );
    settings->setValue( "SplitterState", m_ui.splitter->saveState() );
}

void MainWindow::updateActions()
{
    QModelIndex index = m_proxyModel->mapToSource( m_ui.treeView->currentIndex() );
    ProjectItem* item = m_model->itemFromIndex( index );

    bool isEditable = item != NULL && item->type() != ProjectItem::Project;
    bool canInsertFolder = item != NULL && ( item->type() == ProjectItem::Project || item->type() == ProjectItem::Folder );
    bool canInsertGeometry = item != NULL && ( item->type() == ProjectItem::Project || item->type() == ProjectItem::Folder || item->type() == ProjectItem::Group );
    bool canDrawScene = item != NULL && ( item->type() == ProjectItem::Group || item->type() == ProjectItem::Curve || item->type() == ProjectItem::Surface );

    action( "insertItem" )->setEnabled( canInsertFolder | canInsertGeometry );
    action( "insertFolder" )->setEnabled( canInsertFolder );
    action( "insertGroup" )->setEnabled( canInsertGeometry );
    action( "insertCurve" )->setEnabled( canInsertGeometry );
    action( "insertSurface" )->setEnabled( canInsertGeometry );

    action( "cloneItem" )->setEnabled( isEditable );
    action( "renameItem" )->setEnabled( isEditable );
    action( "deleteItem" )->setEnabled( isEditable );
    action( "editProperties" )->setEnabled( item != NULL );
    action( "drawScene" )->setEnabled( canDrawScene );
    action( "closeScene" )->setEnabled( m_ui.sceneWidget->scene() != NULL );
}

void MainWindow::showContextMenu( const QPoint& pos )
{
    QModelIndex index = m_ui.treeView->indexAt( pos );

    if ( index.isValid() ) {
        m_ui.treeView->selectionModel()->setCurrentIndex( index, QItemSelectionModel::Current );
        m_ui.treeView->selectionModel()->select( index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );

        QMenu* menu = builder()->contextMenu( "menuItem" );
        if ( menu )
            menu->popup( m_ui.treeView->viewport()->mapToGlobal( pos ) );
    }
}

void MainWindow::doubleClicked( const QModelIndex& index )
{
    if ( index.isValid() && action( "drawScene" )->isEnabled() )
        drawScene();
}

void MainWindow::newFile()
{
    if ( !confirmClose() )
        return;

    NewProjectDialog dialog( this );

    int result = dialog.exec();

    if ( result == QDialogButtonBox::Ok )
        newFile( dialog.templatePath() );
    else if ( result == QDialogButtonBox::Open )
        openFile( false );
}

void MainWindow::openFile()
{
    openFile( true );
}

void MainWindow::openFile( bool confirm )
{
    if ( confirm && !confirmClose() )
        return;

    LocalSettings* settings = application->applicationSettings();

    QString myDocuments = QDesktopServices::storageLocation( QDesktopServices::DocumentsLocation );
    QString path = settings->value( "LastPath", myDocuments ).toString();

    path = QFileDialog::getOpenFileName( this, tr( "Open File" ), path, tr( "Descend Projects (*.dscn)" ) );

    if ( !path.isEmpty() ) {
        QFileInfo fileInfo( path );
        settings->setValue( "LastPath", fileInfo.absoluteDir().path() );

        openFile( path );
    }
}

void MainWindow::saveFile()
{
    if ( m_path.isEmpty() )
        saveFileAs();
    else
        saveFile( m_path );
}

void MainWindow::saveFileAs()
{
    LocalSettings* settings = application->applicationSettings();

    QString path;
    if ( m_path.isEmpty() ) {
        QString myDocuments = QDesktopServices::storageLocation( QDesktopServices::DocumentsLocation );
        path = settings->value( "LastPath", myDocuments ).toString();
    } else {
        path = m_path;
    }

    path = QFileDialog::getSaveFileName( this, tr( "Save File As" ), path, tr( "Descend Projects (*.dscn)" ) );

    if ( !path.isEmpty() ) {
        QFileInfo fileInfo( path );
        settings->setValue( "LastPath", fileInfo.absoluteDir().path() );

        if ( fileInfo.suffix().isEmpty() )
            path += ".dscn";

        saveFile( path );
    }
}

void MainWindow::newFile( const QString& path )
{
    Project* project = new Project();

    if ( path.isEmpty() ) {
        project->setName( "Project" );
    } else {
        if ( !project->load( path ) ) {
            delete project;

            QMessageBox::warning( this, tr( "Warning" ), tr( "The selected template could not be opened." ) );

            return;
        }
    }

    delete m_project;
    m_project = project;

    m_path.clear();
    m_modified = false;

    initializeProject();
}

void MainWindow::initializeProject()
{
    delete m_model;
    m_model = new ProjectItemModel( m_project, this );

    connect( m_model, SIGNAL( projectModified() ), this, SLOT( setModified() ) );

    m_proxyModel->setSourceModel( m_model );

    m_ui.treeView->expandToDepth( 1 );
    m_ui.treeView->setCurrentIndex( m_proxyModel->index( 0, 0 ) );

    closeScene();

    m_ui.sceneWidget->setProject( m_project );

    if ( m_path.isEmpty() )
        setWindowTitle( tr( "Descend" ) );
    else
        setWindowTitle( tr( "%1 - Descend" ).arg( QDir::toNativeSeparators( m_path ) ) );
}

void MainWindow::openFile( const QString& path )
{
    Project* project = new Project();

    if ( project->load( path ) ) {
        delete m_project;
        m_project = project;

        m_path = path;
        m_modified = false;

        initializeProject();
    } else {
        delete project;

        QMessageBox::warning( this, tr( "Warning" ), tr( "The selected file could not be opened." ) );
    }
}

void MainWindow::saveFile( const QString& path )
{
    if ( m_project->save( path ) ) {
        m_path = path;
        m_modified = false;

        setWindowTitle( tr( "%1 - Descend" ).arg( QDir::toNativeSeparators( m_path ) ) );
    } else {
        QMessageBox::warning( this, tr( "Warning" ), tr( "The selected file could not be saved." ) );
    }
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

void MainWindow::cloneItem()
{
    QModelIndex index = m_proxyModel->mapToSource( m_ui.treeView->currentIndex() );
    ProjectItem* item = m_model->itemFromIndex( index );

    if ( item != NULL ) {
        QString name = tr( "Copy of %1" ).arg( item->name() );

        QModelIndex cloned = m_model->cloneItem( index, name );

        QModelIndex current = m_proxyModel->mapFromSource( cloned );

        if ( m_ui.treeView->isExpanded( m_ui.treeView->currentIndex() ) )
            m_ui.treeView->expand( current );

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
    ProjectItem* item = m_model->itemFromIndex( index );

    if ( item != NULL ) {
        if ( QMessageBox::warning( this, tr( "Warning" ), tr( "Are you sure you want to delete item <b>%1</b>?" ).arg( item->name() ),
             QMessageBox::Ok | QMessageBox::Cancel ) == QMessageBox::Ok )
            m_model->deleteItem( index );
    }
}

void MainWindow::editProperties()
{
    QModelIndex index = m_proxyModel->mapToSource( m_ui.treeView->currentIndex() );
    ProjectItem* item = m_model->itemFromIndex( index );

    if ( item != NULL ) {
        PropertiesDialog dialog( item, this );
        
        if ( dialog.exec() == QDialog::Accepted )
            m_modified = true;
    }
}

void MainWindow::drawScene()
{
    QModelIndex index = m_proxyModel->mapToSource( m_ui.treeView->currentIndex() );
    ProjectItem* item = m_model->itemFromIndex( index );

    if ( item == NULL )
        return;

    m_model->setMarkedItem( NULL );

    m_ui.sceneWidget->setScene( NULL );

    m_ui.sceneWidget->updateSettings();

    QApplication::setOverrideCursor( Qt::WaitCursor );

    bool ok = false;
    qint64 elapsed = 0;

    Scene* scene = new Scene();

    if ( m_project->initializeScene( scene, item ) ) {
        QColor color = m_project->setting( "Color" ).value<QColor>();
        QColor color2 = m_project->setting( "Color2" ).value<QColor>();

        SceneNodeContext context;
        context.setColor( 0, color );
        context.setColor( 1, color2.isValid() ? color2 : color );

        QElapsedTimer timer;
        timer.start();

        ok = scene->calculate( context );

        elapsed = timer.elapsed();
    }

    if ( ok ) {
        QString info = tr( "Elements: %1" ).arg( scene->elementsCount() );
        info += QLatin1String( "        " );
        info += tr( "Elapsed time: %1 ms" ).arg( elapsed );

        showStatus( IconLoader::pixmap( "status-info" ), info );

        m_model->setMarkedItem( item );

        m_ui.sceneWidget->setScene( scene );
    } else {
        QString error;

        switch ( scene->engine()->errorCode() ) {
            case Misc::SyntaxError:
                error = tr( "Syntax error" );
                break;
            case Misc::TypeError:
                error = tr( "Type mismatch error" );
                break;
            case Misc::RuntimeError:
                error = tr( "Runtime error" );
                break;
            default:
                error = tr( "Error" );
                break;
        }

        error += QLatin1String( ": " );
        error += scene->engine()->errorMessage();

        if ( m_project->errorItem() != NULL ) {
            error += QLatin1Char( ' ' );

            switch ( m_project->errorContext() ) {
                case Project::InitContext:
                    error += tr( "in initialization of" );
                    break;
                case Project::CalcContext:
                    error += tr( "in calculation of" );
                    break;
                default:
                    error += tr( "in" );
                    break;
            }

            error += QLatin1Char( ' ' );
            error += m_project->errorItem()->name();

            error += QLatin1String( ", " );
            error += tr( "line %1" ).arg( scene->engine()->errorLineNumber() );
        }

        showStatus( IconLoader::pixmap( "status-error" ), error, QMessageBox::Warning );

        delete scene;
    }

    QApplication::restoreOverrideCursor();

    updateActions();
}

void MainWindow::closeScene()
{
    m_ui.sceneWidget->setScene( NULL );

    showStatus( QPixmap(), QString() );

    m_model->setMarkedItem( NULL );

    updateActions();
}

void MainWindow::colorSettings()
{
    ColorSettingsDialog dialog( m_project, this );

    if ( dialog.exec() == QDialog::Accepted )
        m_modified = true;
}

void MainWindow::tessellationSettings()
{
    TessellationDialog dialog( m_project, this );

    if ( dialog.exec() == QDialog::Accepted )
        m_modified = true;
}

void MainWindow::setModified()
{
    m_modified = true;
}

void MainWindow::closeEvent( QCloseEvent* e )
{
    if ( confirmClose() )
        e->accept();
    else
        e->ignore();
}

bool MainWindow::confirmClose()
{
    if ( !m_modified )
        return true;

    QMessageBox::StandardButton button = QMessageBox::warning( this, tr( "Warning" ), tr( "Do you want to save current project?" ),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

    if ( button == QMessageBox::No )
        return true;

    if ( button == QMessageBox::Yes ) {
        saveFile();

        if ( !m_modified )
            return true;
    }

    return false;
}

void MainWindow::showStatus( const QPixmap& pixmap, const QString& text, int icon /*= 0*/ )
{
    m_statusLabel->setPixmap( pixmap );
    m_statusLabel->setText( text );

    if ( icon != 0 && topLevelWidget()->isActiveWindow() ) {
        QMessageBox box;
        box.setIcon( (QMessageBox::Icon)icon );
        QAccessible::updateAccessibility( &box, 0, QAccessible::Alert );
    }
}

QString MainWindow::glVersion() const
{
    QGLFormat format = m_ui.sceneWidget->format();

    return QString( "%1.%2" ).arg( format.majorVersion() ).arg( format.minorVersion() );
}
