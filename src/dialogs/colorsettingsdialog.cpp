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

#include "dialogs/colorsettingsdialog.h"

#include "project/project.h"
#include "utils/iconloader.h"

#include <QPushButton>

ColorSettingsDialog::ColorSettingsDialog( Project* project, QWidget* parent ) : QDialog( parent ),
    m_project( project )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "colors", 22 ) );
    m_ui.promptLabel->setText( tr( "Edit color settings for the project:" ) );

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );

    QLabel* labels[ 6 ] = { m_ui.modeLabel, m_ui.primaryLabel, m_ui.secondaryLabel, m_ui.bgModeLabel, m_ui.topLabel, m_ui.bottomLabel };

    int maxWidth = 0;
    for ( int i = 0; i < 6; i++ )
        maxWidth = qMax( maxWidth, labels[ i ]->sizeHint().width() );

    for ( int i = 0; i < 6; i++ )
        labels[ i ]->setFixedWidth( maxWidth );

    maxWidth = qMax( m_ui.modeComboBox->sizeHint().width(), m_ui.bgModeComboBox->sizeHint().width() );
    m_ui.modeComboBox->setFixedWidth( maxWidth );
    m_ui.bgModeComboBox->setFixedWidth( maxWidth );

    m_ui.buttonBox->button( QDialogButtonBox::Ok )->setText( tr( "&OK" ) );
    m_ui.buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "&Cancel" ) );

    setFixedHeight( sizeHint().height() );

    QColor color = m_project->setting( "Color" ).value<QColor>();
    QColor color2 = m_project->setting( "Color2" ).value<QColor>();
    QColor bgColor = m_project->setting( "BgColor" ).value<QColor>();
    QColor bgColor2 = m_project->setting( "BgColor2" ).value<QColor>();

    m_ui.primaryButton->setColor( color );
    if ( color2.isValid() ) {
        m_ui.modeComboBox->setCurrentIndex( 1 );
        m_ui.secondaryButton->setColor( color2 );
    } else {
        m_ui.modeComboBox->setCurrentIndex( 0 );
        m_ui.secondaryButton->setColor( QColor( 255, 255, 255 ) );
    }

    m_ui.topButton->setColor( bgColor );
    if ( bgColor2.isValid() ) {
        m_ui.bgModeComboBox->setCurrentIndex( 1 );
        m_ui.bottomButton->setColor( bgColor2 );
    } else {
        m_ui.bgModeComboBox->setCurrentIndex( 0 );
        m_ui.bottomButton->setColor( QColor( 255, 255, 255 ) );
    }

    connect( m_ui.modeComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( updateWidgets() ) );
    connect( m_ui.bgModeComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( updateWidgets() ) );

    updateWidgets();
}

ColorSettingsDialog::~ColorSettingsDialog()
{
}

void ColorSettingsDialog::accept()
{
    m_project->setSetting( "Color", m_ui.primaryButton->color() );
    m_project->setSetting( "Color2", m_ui.modeComboBox->currentIndex() == 1 ? m_ui.secondaryButton->color() : QColor() );
    m_project->setSetting( "BgColor", m_ui.topButton->color() );
    m_project->setSetting( "BgColor2", m_ui.bgModeComboBox->currentIndex() == 1 ? m_ui.bottomButton->color() : QColor() );

    QDialog::accept();
}

void ColorSettingsDialog::updateWidgets()
{
    m_ui.secondaryButton->setEnabled( m_ui.modeComboBox->currentIndex() == 1 );
    m_ui.bottomButton->setEnabled( m_ui.bgModeComboBox->currentIndex() == 1 );
}
