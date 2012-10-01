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

#ifndef SCENENODECOLOR_H
#define SCENENODECOLOR_H

class SceneNodeColor
{
public:
    enum ColorFlag
    {
        DualColors = 1,
        SwapColors = 2
    };

    Q_DECLARE_FLAGS( ColorFlags, ColorFlag )

    enum ColorType
    {
        InheritPrimary,
        InheritSecondary,
        Custom,
        Calculated
    };

public:
    explicit SceneNodeColor( ColorFlags flags = 0 );
    SceneNodeColor( const SceneNodeColor& other );
    ~SceneNodeColor();

public:
    SceneNodeColor& operator =( const SceneNodeColor& other );

public:
    void setFlags( ColorFlags flags );
    ColorFlags flags() const { return m_flags; }

    void setType( int index, ColorType type );
    ColorType type( int index ) const;

    void setColor( int index, const QColor& color );
    QColor color( int index ) const;

private:
    ColorFlags m_flags;
    ColorType m_type[ 2 ];
    QColor m_color[ 2 ];
};

Q_DECLARE_OPERATORS_FOR_FLAGS( SceneNodeColor::ColorFlags )

#endif
