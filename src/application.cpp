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

#include "application.h"

#include "mainwindow.h"
#include "utils/localsettings.h"
#include "utils/iconloader.h"

#if defined( Q_WS_WIN )
#include <shlobj.h>
#endif

Application* application = NULL;

Application::Application( int& argc, char** argv ) : QApplication( argc, argv )
{
    Q_INIT_RESOURCE( icons );
    Q_INIT_RESOURCE( resources );
    Q_INIT_RESOURCE( shaders );

    initializeDefaultPaths();

    application = this;

    m_settings = new LocalSettings( locateDataFile( "settings.dat" ), this );

#if defined( Q_WS_WIN )
    setStyle( "XmlUi::WindowsStyle" );
#elif defined( Q_WS_MAC )
    setStyle( "XmlUi::MacStyle" );
#endif

    setWindowIcon( IconLoader::icon( "descend" ) );

    m_mainWindow = new MainWindow();
    m_mainWindow->showMaximized();
}

Application::~Application()
{
    delete m_mainWindow;
    m_mainWindow = NULL;

    delete m_settings;
    m_settings = NULL;
}

void Application::initializeDefaultPaths()
{
    wchar_t appDataPath[ MAX_PATH ];
    if ( SHGetSpecialFolderPath( 0, appDataPath, CSIDL_APPDATA, FALSE ) )
        m_dataPath = QDir::fromNativeSeparators( QString::fromWCharArray( appDataPath ) );
    else
        m_dataPath = QDir::homePath();

    m_dataPath += QLatin1String( "/Descend" );
}

QString Application::locateDataFile( const QString& name )
{
    QString path = m_dataPath + '/' + name;

    checkAccess( path );

    return path;
}

bool Application::checkAccess( const QString& path )
{
    QFileInfo fileInfo( path );
    if ( fileInfo.exists() )
        return fileInfo.isReadable();

    QDir dir = fileInfo.absoluteDir();
    if ( dir.exists() )
        return dir.isReadable();

    return dir.mkpath( dir.absolutePath() );
}
