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

#include "widgets/colorbutton.h"

ColorButton::ColorButton( QWidget* parent ) : QToolButton( parent )
{
    connect( this, SIGNAL( clicked( bool ) ), this, SLOT( selectColor() ) );
}

ColorButton::~ColorButton()
{
}

static QColor colorToSrgb( const QColor& color )
{
    double r = pow( color.redF(), 1.0 / 2.2 );
    double g = pow( color.greenF(), 1.0 / 2.2 );
    double b = pow( color.blueF(), 1.0 / 2.2 );
    return QColor::fromRgbF( r, g, b );
}

static QColor colorFromSrgb( const QColor& color )
{
    double r = pow( color.redF(), 2.2 );
    double g = pow( color.greenF(), 2.2 );
    double b = pow( color.blueF(), 2.2 );
    return QColor::fromRgbF( r, g, b );
}

void ColorButton::setColor( const QColor& color )
{
    if ( m_color != color ) {
        m_color = color;
        m_colorSrgb = color.isValid() ? colorToSrgb( color ) : QColor();

        updateIcon();

        emit colorChanged( color );
    }
}

void ColorButton::selectColor()
{
    QColor color = QColorDialog::getColor( m_colorSrgb, this, tr( "Select Color" ) );

    if ( color.isValid() && color != m_colorSrgb ) {
        m_color = colorFromSrgb( color );
        m_colorSrgb = color;

        updateIcon();

        emit colorChanged( color );
    }
}

void ColorButton::updateIcon()
{
    if ( m_colorSrgb.isValid() ) {
        QPixmap pixmap( iconSize() );
        pixmap.fill( Qt::transparent );

        QPainter painter( &pixmap );
        painter.fillRect( pixmap.rect().adjusted( 2, 2, -1, -2 ), m_colorSrgb );

        setIcon( QIcon( pixmap ) );
    } else {
        setIcon( QIcon() );
    }
}
