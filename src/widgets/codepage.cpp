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

#include "widgets/codepage.h"

#include "adapters/codeadapter.h"
#include "widgets/elidedlabel.h"

CodePage::CodePage( CodeAdapter* adapter, const QString& label, QWidget* parent ) : QWidget( parent ),
    m_inOutLabel( NULL ),
    m_adapter( adapter ),
    m_label( label )
{
    QVBoxLayout* layout = new QVBoxLayout( this );

    QFont font( "Consolas", 10 );
    font.setStyleHint( QFont::SansSerif );

    m_edit = new QPlainTextEdit( this );
    m_edit->setFont( font );
    m_edit->setLineWrapMode( QPlainTextEdit::NoWrap );

    QFontMetrics metrics( font );
    m_edit->setTabStopWidth( 4 * metrics.width( ' ' ) );

    layout->addWidget( m_edit, 1 );

    m_edit->setPlainText( adapter->code() );

    if ( adapter->hasInputsOutputs() ) {
        m_inOutLabel = new ElidedLabel( this );
        m_inOutLabel->setFont( font );

        layout->addWidget( m_inOutLabel );
    }
}

CodePage::~CodePage()
{
    delete m_adapter;
}

void CodePage::apply()
{
    m_adapter->setCode( m_edit->toPlainText() );
}

void CodePage::updateInputsOutputs( Renderer::AttributeType attr, const SceneNodeColor& color )
{
    QString inputs = m_adapter->inputs( attr, color ).join( ", " );
    QString outputs = m_adapter->outputs( attr, color ).join( ", " );

    m_inOutLabel->setText( QString::fromUtf8( "%1 → %2" ).arg( inputs, outputs ).trimmed() );
}
