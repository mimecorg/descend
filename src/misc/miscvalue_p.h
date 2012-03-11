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

#ifndef MISCVALUE_P_H
#define MISCVALUE_P_H

#include "miscvalue.h"

#include <QVector4D>
#include <QMatrix4x4>

class MiscEnginePrivate;

class MiscValuePrivate
{
public:
    MiscValuePrivate( Misc::Type type, MiscEnginePrivate* engine );
    ~MiscValuePrivate();

public:
    static MiscValuePrivate* get( const MiscValue& value ) { return value.d; }
    static MiscValue toPublic( MiscValuePrivate* data ) { return MiscValue( data ); }

public:
    void setVector( const QVector4D& v )
    {
        m_vector[ 0 ] = v.x();
        m_vector[ 1 ] = v.y();
        m_vector[ 2 ] = v.z();
        m_vector[ 3 ] = v.w();
    }

    QVector4D vector() const
    {
        return QVector4D( m_vector[ 0 ], m_vector[ 1 ], m_vector[ 2 ], m_vector[ 3 ] );
    }

    void setMatrix( const QMatrix4x4& m )
    {
        for ( int r = 0; r < 4; r++ ) {
            for ( int c = 0; c < 4; c++ )
                m_matrix[ r ][ c ] = m( r, c );
        }
    }

    QMatrix4x4 matrix() const
    {
        return QMatrix4x4(
            m_matrix[ 0 ][ 0 ], m_matrix[ 0 ][ 1 ], m_matrix[ 0 ][ 2 ], m_matrix[ 0 ][ 3 ],
            m_matrix[ 1 ][ 0 ], m_matrix[ 1 ][ 1 ], m_matrix[ 1 ][ 2 ], m_matrix[ 1 ][ 3 ],
            m_matrix[ 2 ][ 0 ], m_matrix[ 2 ][ 1 ], m_matrix[ 2 ][ 2 ], m_matrix[ 2 ][ 3 ],
            m_matrix[ 3 ][ 0 ], m_matrix[ 3 ][ 1 ], m_matrix[ 3 ][ 2 ], m_matrix[ 3 ][ 3 ]
        );
    }

public:
    int m_ref;
    Misc::Type m_type;
    MiscEnginePrivate* m_engine;
    MiscValuePrivate* m_next;

    union {
        char m_data[ 1 ];
        bool m_bool;
        float m_float;
        float m_vector[ 4 ];
        float m_matrix[ 4 ][ 4 ];
    };
};

struct MiscSlot
{
    Misc::Type m_type;
    union
    {
        bool m_bool;
        float m_float;
        MiscValuePrivate* m_value;
    };
};

#endif
