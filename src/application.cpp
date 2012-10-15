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
#include "dialogs/aboutbox.h"
#include "dialogs/guidedialog.h"

#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QPushButton>
#include <QDir>

#if defined( Q_WS_WIN )
#include <shlobj.h>
#endif

Application* application = NULL;

Application::Application( int& argc, char** argv ) : QApplication( argc, argv ),
    m_mainWindow( NULL )
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

    QGLFormat::OpenGLVersionFlags flags = QGLFormat::openGLVersionFlags();

    if ( ( flags & QGLFormat::OpenGL_Version_3_3 ) == 0 ) {
        QMessageBox::critical( NULL, tr( "Error" ), tr( "Descend requires OpenGL version %1.%2 or higher." ).arg( 3 ).arg( 3 ) );
        return;
    }

    m_mainWindow = new MainWindow();
    m_mainWindow->show();

    if ( argc > 1 )
        m_mainWindow->openFile( QString( argv[ 1 ] ) );
    else if ( m_settings->value( "NewProjectOnStartup", true ).toBool() )
        QTimer::singleShot( 100, m_mainWindow, SLOT( newFile() ) );
}

Application::~Application()
{
    delete m_mainWindow;
    m_mainWindow = NULL;

    delete m_settings;
    m_settings = NULL;
}

int Application::exec()
{
    if ( application->m_mainWindow == NULL )
        return 1;

    return QApplication::exec();
}

#if defined( Q_OS_WIN )
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif

QString Application::technicalInformation()
{
#if defined( Q_OS_WIN )
#if defined( Q_OS_WIN64 )
    QString configBits = "64";
#else
    QString configBits = "32";
#endif
#if defined ( QT_DEBUG )
    QString configMode = "debug";
#else
    QString configMode = "release";
#endif
#endif
#if defined( QT_DLL )
    QString configLink = "dynamic";
#else
    QString configLink = "static";
#endif

    QString qtVersion = qVersion();

    QString glVersion = m_mainWindow->glVersion();

    QString infoMessage;
    infoMessage += "<h4>" + tr( "Technical Information" ) + "</h4>";
    infoMessage += "<p>";

#if defined( Q_OS_WIN )
    const IMAGE_NT_HEADERS* header = (const IMAGE_NT_HEADERS*)( (char*)&__ImageBase + __ImageBase.e_lfanew );
    QDateTime compiled = QDateTime::fromTime_t( header->FileHeader.TimeDateStamp );

    infoMessage += tr( "Built on %1 in %2-bit %3 mode." ).arg( compiled.toString( "yyyy-MM-dd HH:mm" ), configBits, configMode ) + " ";
#endif

    infoMessage += " " + tr( "Using Qt %1 (%2 linking) and OpenGL %3." ).arg( qtVersion, configLink, glVersion ) + "</p>";

    return infoMessage;
}

void Application::about()
{
    if ( !m_aboutBox ) {
        QString message;
        message += "<h3>" + tr( "Descend %1" ).arg( "0.2" ) + "</h3>";
        message += "<p>" + tr( "Program for drawing 3D surfaces from parametric equations." ) + "</p>";
        message += "<p>" + tr( "This program is free software: you can redistribute it and/or modify"
            " it under the terms of the GNU General Public License as published by"
            " the Free Software Foundation, either version 3 of the License, or"
            " (at your option) any later version." ) + "</p>";
        message += "<p>" + trUtf8( "Copyright &copy; 2012 Michał Męciński" ) + "</p>";

        QString link = "<a href=\"http://descend.mimec.org\">descend.mimec.org</a>";

        QString helpMessage;
        helpMessage += "<h4>" + tr( "Help" ) + "</h4>";
        helpMessage += "<p>" + tr( "Open the Descend Quick Guide for help." ) + "</p>";

        QString webMessage;
        webMessage += "<h4>" + tr( "Website" ) + "</h4>";
        webMessage += "<p>" + tr( "Visit %1 for more information about Descend." ).arg( link ) + "</p>";

        QString infoMessage = technicalInformation();

        m_aboutBox = new AboutBox( tr( "About Descend" ), message, m_mainWindow );

        AboutBoxSection* helpSection = m_aboutBox->addSection( IconLoader::pixmap( "help" ), helpMessage );

        QPushButton* helpButton = helpSection->addButton( tr( "&Quick Guide" ) );
        connect( helpButton, SIGNAL( clicked() ), this, SLOT( showQuickGuide() ) );

        m_aboutBox->addSection( IconLoader::pixmap( "web" ), webMessage );

        m_aboutBox->addSection( IconLoader::pixmap( "status-info" ), infoMessage );
    }

    m_aboutBox->show();
    m_aboutBox->activateWindow();
}

void Application::showQuickGuide()
{
    m_aboutBox->close();

    if ( !m_guideDialog ) {
        m_guideDialog = new GuideDialog( m_mainWindow );

        m_guideDialog->resize( m_mainWindow->width() / 2, m_mainWindow->height() - 100 );
        m_guideDialog->move( m_mainWindow->pos().x() + m_mainWindow->width() / 2 - 50, m_mainWindow->pos().y() + 50 );
    }

    m_guideDialog->show();
    m_guideDialog->activateWindow();
}

void Application::initializeDefaultPaths()
{
    QString appPath = applicationDirPath();

#if defined( Q_WS_WIN )
    m_templatesPath = QDir::cleanPath( appPath + "/../templates" );
#else
    m_templatesPath = QDir::cleanPath( appPath + "/../share/descend/templates" );
#endif

#if defined( Q_WS_WIN )
    wchar_t appDataPath[ MAX_PATH ];
    if ( SHGetSpecialFolderPath( 0, appDataPath, CSIDL_APPDATA, FALSE ) )
        m_dataPath = QDir::fromNativeSeparators( QString::fromWCharArray( appDataPath ) );
    else
        m_dataPath = QDir::homePath();

    m_dataPath += QLatin1String( "/Descend" );
#else
    m_dataPath = QDesktopServices::storageLocation( QDesktopServices::DataLocation );
    m_dataPath += QLatin1String( "/descend" );
#endif
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
