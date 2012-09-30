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

void ColorButton::setColor( const QColor& color )
{
    if ( m_color != color ) {
        m_color = color;

        if ( color.isValid() ) {
            QPixmap pixmap( iconSize() );
            pixmap.fill( Qt::transparent );

            QPainter painter( &pixmap );
            painter.fillRect( pixmap.rect().adjusted( 2, 2, -1, -2 ), m_color );

            setIcon( QIcon( pixmap ) );
        } else {
            setIcon( QIcon() );
        }

        emit colorChanged( color );
    }
}

void ColorButton::selectColor()
{
    QColor color = QColorDialog::getColor( m_color, this, tr( "Select Color" ) );
    if ( color.isValid() )
        setColor( color );
}
