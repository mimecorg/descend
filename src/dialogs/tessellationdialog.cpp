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

#include "dialogs/tessellationdialog.h"

#include "project/project.h"
#include "utils/iconloader.h"

TessellationDialog::TessellationDialog( Project* project, QWidget* parent ) : QDialog( parent ),
    m_project( project )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "tessellation", 22 ) );
    m_ui.promptLabel->setText( tr( "Edit tessellation settings for the project:" ) );

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );

    setFixedHeight( sizeHint().height() );

    m_ui.minimumSlider->setValue( project->setting( "MinLod" ).toInt() );
    m_ui.maximumSlider->setValue( project->setting( "MaxLod" ).toInt() );

    m_ui.geometrySlider->setValue( int( project->setting( "GeometryThreshold" ).toFloat() * 100.0f + 0.5f ) );
    m_ui.attributesSlider->setValue( int( project->setting( "AttributeThreshold" ).toFloat() * 100.0f + 0.5f ) );

    m_ui.edgesCheckBox->setChecked( project->setting( "DrawEdges" ).toBool() );
}

TessellationDialog::~TessellationDialog()
{
}

void TessellationDialog::accept()
{
    m_project->setSetting( "MinLod", m_ui.minimumSlider->value() );
    m_project->setSetting( "MaxLod", m_ui.maximumSlider->value() );

    m_project->setSetting( "GeometryThreshold", m_ui.geometrySlider->value() * 0.01f );
    m_project->setSetting( "AttributeThreshold", m_ui.attributesSlider->value() * 0.01f );

    m_project->setSetting( "DrawEdges", m_ui.edgesCheckBox->isChecked() );

    QDialog::accept();
}

void TessellationDialog::on_minimumSlider_valueChanged( int value )
{
    m_ui.maximumSlider->setValue( qMax( m_ui.maximumSlider->value(), value ) );
}

void TessellationDialog::on_maximumSlider_valueChanged( int value )
{
    m_ui.minimumSlider->setValue( qMin( m_ui.minimumSlider->value(), value ) );
}
