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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "xmlui/client.h"
#include "project/projectitem.h"

#include <QMainWindow>

class StatusLabel;
class Project;
class ProjectItemModel;

class QSortFilterProxyModel;

class MainWindow : public QMainWindow, public XmlUi::Client
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

public:
    QString glVersion() const;

    void openFile( const QString& path );

protected: // overrides
    void closeEvent( QCloseEvent* e );

private slots:
    void updateActions();

    void showContextMenu( const QPoint& pos );

    void doubleClicked( const QModelIndex& index );

    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

    void insertItem();
    void insertFolder();
    void insertGroup();
    void insertCurve();
    void insertSurface();

    void cloneItem();
    void renameItem();
    void deleteItem();
    void editProperties();

    void drawScene();
    void closeScene();

    void colorSettings();
    void tessellationSettings();

    void setModified();

private:
    void initializeProject();

    void newFile( const QString& path );
    void openFile( bool confirm );
    void saveFile( const QString& path );

    void insertItem( ProjectItem::Type type, const QString& name );

    bool confirmClose();

    void showStatus( const QPixmap& pixmap, const QString& text, int icon = 0 );

private:
    Ui::MainWindow m_ui;

    StatusLabel* m_statusLabel;

    Project* m_project;

    ProjectItemModel* m_model;
    QSortFilterProxyModel* m_proxyModel;

    QString m_path;

    bool m_modified;
};

#endif
