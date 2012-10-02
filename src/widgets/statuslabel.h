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

#ifndef STATUSLABEL_H
#define STATUSLABEL_H

#include <QWidget>

class QLabel;
class ElidedLabel;

/**
* A label with an optional icon.
*/
class StatusLabel : public QWidget
{
    Q_OBJECT
public:
    /**
    * Constructor.
    * @param parent The parent widget.
    */
    StatusLabel( QWidget* parent );

    /**
    * Destructor.
    */
    ~StatusLabel();

public:
    void setText( const QString& text );
    QString text() const;

    void setPixmap( const QPixmap& pixmap );
    const QPixmap* pixmap() const;

private:
    QLabel* m_pixmapLabel;
    ElidedLabel* m_label;
};

#endif
