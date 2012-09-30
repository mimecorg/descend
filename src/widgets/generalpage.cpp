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

#include "widgets/generalpage.h"

#include "adapters/generaladapter.h"

GeneralPage::GeneralPage( GeneralAdapter* adapter, QWidget* parent ) : QWidget( parent ),
    m_adapter( adapter ),
    m_updating( false )
{
    m_ui.setupUi( this );

    SceneNodeColor::ColorFlags flags = adapter->hasColorFlags();

    m_ui.modeComboBox->addItem( tr( "Single Color" ), SingleColor );
    if ( flags & SceneNodeColor::DualColors )
        m_ui.modeComboBox->addItem( tr( "Dual Colors" ), DualColors );
    if ( adapter->hasAttributeType() )
        m_ui.modeComboBox->addItem( tr( "Vertex Color" ), VertexColor );

    m_ui.primaryComboBox->addItem( tr( "Inherit Primary" ), SceneNodeColor::InheritPrimary );
    m_ui.primaryComboBox->addItem( tr( "Inherit Secondary" ), SceneNodeColor::InheritSecondary );
    m_ui.primaryComboBox->addItem( tr( "Custom" ), SceneNodeColor::Custom );
    m_ui.primaryComboBox->addItem( tr( "Calculated" ), SceneNodeColor::Calculated );

    if ( !( flags & SceneNodeColor::DualColors ) ) {
        m_ui.secondaryLabel->hide();
        m_ui.secondaryComboBox->hide();
        m_ui.secondaryButton->hide();
    }

    if ( !( flags & SceneNodeColor::SwapColors ) )
        m_ui.swapCheckBox->hide();

    ColorMode mode = SingleColor;
    int type = SceneNodeColor::InheritPrimary;
    int type2 = SceneNodeColor::InheritSecondary;
    QColor color = Qt::white;
    QColor color2 = Qt::white;

    SceneNodeColor nodeColor = adapter->color();

    if ( adapter->hasAttributeType() && adapter->attributeType() == Renderer::RgbAttribute )
        mode = VertexColor;
    else if ( ( flags & SceneNodeColor::DualColors ) && ( nodeColor.flags() & SceneNodeColor::DualColors ) )
        mode = DualColors;

    if ( mode == SingleColor || mode == DualColors ) {
        type = nodeColor.type( 0 );
        if ( type == SceneNodeColor::Custom )
            color = nodeColor.color( 0 );
    }

    if ( mode == DualColors ) {
        type2 = nodeColor.type( 1 );
        if ( type2 == SceneNodeColor::Custom )
            color2 = nodeColor.color( 1 );
    }

    m_ui.modeComboBox->setCurrentIndex( m_ui.modeComboBox->findData( mode ) );

    m_ui.primaryComboBox->setCurrentIndex( m_ui.primaryComboBox->findData( type ) );
    m_ui.primaryButton->setColor( color );

    if ( flags & SceneNodeColor::DualColors ) {
        populateSecondaryComboBox( type, type2 );
        m_ui.secondaryButton->setColor( color2 );
    }

    if ( ( flags & SceneNodeColor::SwapColors ) && ( nodeColor.flags() & SceneNodeColor::SwapColors ) )
        m_ui.swapCheckBox->setChecked( true );

    connect( m_ui.modeComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( updateWidgets() ) );
    connect( m_ui.primaryComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( updateWidgets() ) );
    connect( m_ui.secondaryComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( updateWidgets() ) );

    updateWidgets();
}

GeneralPage::~GeneralPage()
{
    delete m_adapter;
}

void GeneralPage::apply()
{
    if ( m_adapter->hasAttributeType() )
        m_adapter->setAttributeType( attributeType() );

    m_adapter->setColor( color() );
}

Renderer::AttributeType GeneralPage::attributeType() const
{
    int mode = m_ui.modeComboBox->itemData( m_ui.modeComboBox->currentIndex() ).toInt();

    if ( mode == VertexColor )
        return Renderer::RgbAttribute;
    else
        return Renderer::NoAttribute;
}

SceneNodeColor GeneralPage::color() const
{
    SceneNodeColor color;

    int mode = m_ui.modeComboBox->itemData( m_ui.modeComboBox->currentIndex() ).toInt();

    if ( mode == SingleColor || mode == DualColors ) {
        int type = m_ui.primaryComboBox->itemData( m_ui.primaryComboBox->currentIndex() ).toInt();

        color.setType( 0, static_cast<SceneNodeColor::ColorType>( type ) );
        if ( type == SceneNodeColor::Custom )
            color.setColor( 0, m_ui.primaryButton->color() );

        if ( mode == DualColors ) {
            color.setFlags( color.flags() | SceneNodeColor::DualColors );

            int type2 = m_ui.secondaryComboBox->itemData( m_ui.secondaryComboBox->currentIndex() ).toInt();

            color.setType( 1, static_cast<SceneNodeColor::ColorType>( type2 ) );
            if ( type2 == SceneNodeColor::Custom )
                color.setColor( 1, m_ui.secondaryButton->color() );

            SceneNodeColor::ColorFlags flags = m_adapter->hasColorFlags();
        
            if ( ( flags & SceneNodeColor::SwapColors ) && m_ui.swapCheckBox->isChecked() )
                color.setFlags( color.flags() | SceneNodeColor::SwapColors );
        }
    }

    return color;
}

void GeneralPage::updateWidgets()
{
    if ( m_updating )
        return;

    m_updating = true;

    SceneNodeColor::ColorFlags flags = m_adapter->hasColorFlags();

    int mode = m_ui.modeComboBox->itemData( m_ui.modeComboBox->currentIndex() ).toInt();
    int type = m_ui.primaryComboBox->itemData( m_ui.primaryComboBox->currentIndex() ).toInt();

    m_ui.primaryComboBox->setEnabled( mode == SingleColor || mode == DualColors );
    m_ui.primaryButton->setEnabled( ( mode == SingleColor || mode == DualColors ) && type == SceneNodeColor::Custom );

    if ( flags & SceneNodeColor::DualColors ) {
        int type2 = m_ui.secondaryComboBox->itemData( m_ui.secondaryComboBox->currentIndex() ).toInt();

        m_ui.secondaryComboBox->setEnabled( mode == DualColors );
        populateSecondaryComboBox( type, type2 );

        m_ui.secondaryButton->setEnabled( mode == DualColors && type2 == SceneNodeColor::Custom );
    }

    if ( flags & SceneNodeColor::SwapColors )
        m_ui.swapCheckBox->setEnabled( mode == DualColors );

    emit colorChanged();

    m_updating = false;
}

void GeneralPage::populateSecondaryComboBox( int type, int& type2 )
{
    m_ui.secondaryComboBox->clear();

    if ( type != SceneNodeColor::Calculated ) {
        m_ui.secondaryComboBox->addItem( tr( "Inherit Primary" ), SceneNodeColor::InheritPrimary );
        m_ui.secondaryComboBox->addItem( tr( "Inherit Secondary" ), SceneNodeColor::InheritSecondary );
        m_ui.secondaryComboBox->addItem( tr( "Custom" ), SceneNodeColor::Custom );

        if ( type2 == SceneNodeColor::Calculated )
            type2 = SceneNodeColor::InheritSecondary;
    } else {
        m_ui.secondaryComboBox->addItem( tr( "Calculated" ), SceneNodeColor::Calculated );

        type2 = SceneNodeColor::Calculated;
    }

    m_ui.secondaryComboBox->setCurrentIndex( m_ui.secondaryComboBox->findData( type2 ) );
}
