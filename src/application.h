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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class MainWindow;
class LocalSettings;
class AboutBox;

class Application : public QApplication
{
    Q_OBJECT
public:
    Application( int& argc, char** argv );
    ~Application();

public:
    QString locateDataFile( const QString& name );

    LocalSettings* applicationSettings() const { return m_settings; }

public slots:
    void about();

public:
    static int exec();

private:
    void initializeDefaultPaths();

    bool checkAccess( const QString& path );

    QString technicalInformation();

private:
    QString m_dataPath;

    LocalSettings* m_settings;

    MainWindow* m_mainWindow;

    QPointer<AboutBox> m_aboutBox;
};

extern Application* application;

#endif
