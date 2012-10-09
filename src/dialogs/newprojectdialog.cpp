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

#include "dialogs/newprojectdialog.h"

#include "application.h"
#include "utils/iconloader.h"
#include "utils/localsettings.h"

NewProjectDialog::NewProjectDialog( QWidget* parent ) : QDialog( parent )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "file-new", 22 ) );
    m_ui.promptLabel->setText( tr( "Select a template for the new project:" ) );

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );

    m_ui.buttonBox->button( QDialogButtonBox::Ok )->setText( tr( "C&reate" ) );
    m_ui.buttonBox->button( QDialogButtonBox::Open )->setText( tr( "&Open..." ) );
    m_ui.buttonBox->button( QDialogButtonBox::Close )->setText( tr( "&Close" ) );

    QListWidgetItem* blankItem = new QListWidgetItem( IconLoader::icon( "descend" ), tr( "Blank Project" ), m_ui.listTemplates );

    QSettings templates( application->templatesPath() + "/templates.ini", QSettings::IniFormat );
    templates.setIniCodec( "UTF8" );

    templates.beginGroup( "templates" );
    QStringList keys = templates.allKeys();

    foreach ( QString key, keys ) {
        QString name = templates.value( key ).toString();

        QListWidgetItem* item = new QListWidgetItem( IconLoader::icon( "descend" ), name, m_ui.listTemplates );
        item->setData( Qt::UserRole, application->templatesPath() + "/" + key );
    }

    LocalSettings* settings = application->applicationSettings();

    m_ui.checkShowOnStartup->setChecked( settings->value( "NewProjectOnStartup", true ).toBool() );

    blankItem->setSelected( true );
}

NewProjectDialog::~NewProjectDialog()
{
}

void NewProjectDialog::on_buttonBox_clicked( QAbstractButton* button )
{
    LocalSettings* settings = application->applicationSettings();

    settings->setValue( "NewProjectOnStartup", m_ui.checkShowOnStartup->isChecked() );

    QDialog::done( m_ui.buttonBox->standardButton( button ) );
}

QString NewProjectDialog::templatePath() const
{
    QList<QListWidgetItem*> items = m_ui.listTemplates->selectedItems();

    if ( items.count() == 0 )
        return QString();

    return items.first()->data( Qt::UserRole ).toString();
}

void NewProjectDialog::on_listTemplates_itemSelectionChanged()
{
    m_ui.buttonBox->button( QDialogButtonBox::Ok )->setEnabled( m_ui.listTemplates->selectedItems().count() > 0 );
}

void NewProjectDialog::on_listTemplates_itemDoubleClicked( QListWidgetItem* /*item*/ )
{
    on_buttonBox_clicked( m_ui.buttonBox->button( QDialogButtonBox::Ok ) );
}
