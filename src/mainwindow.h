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

#include <QMainWindow>

#include "ui_mainwindow.h"
#include "xmlui/client.h"
#include "project/projectitem.h"

class Project;
class ProjectItemModel;

class MainWindow : public QMainWindow, public XmlUi::Client
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

private slots:
    void on_executeButton_clicked();
    void on_edgesCheckBox_toggled( bool on );

    void updateActions();

    void insertItem();
    void insertFolder();
    void insertGroup();
    void insertCurve();
    void insertSurface();

    void renameItem();
    void deleteItem();

private:
    void insertItem( ProjectItem::Type type, const QString& name );

private:
    Ui::MainWindow m_ui;

    Project* m_project;

    ProjectItemModel* m_model;
    QSortFilterProxyModel* m_proxyModel;
};

#endif
