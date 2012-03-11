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

#ifndef MISCIDENTIFIER_P_H
#define MISCIDENTIFIER_P_H

#include "miscidentifier.h"

class MiscEnginePrivate;

class MiscIdentifierPrivate
{
public:
    MiscIdentifierPrivate( int index, uint hash, MiscEnginePrivate* engine ) :
        m_index( index ),
        m_hash( hash ),
        m_next( NULL ),
        m_engine( engine )
    {
    }

    ~MiscIdentifierPrivate()
    {
    }

public:
    static MiscIdentifierPrivate* get( const MiscIdentifier& identifier ) { return identifier.d; }
    static MiscIdentifier toPublic( MiscIdentifierPrivate* data ) { return MiscIdentifier( data ); }

public:
    QString name() const;

public:
    int m_index;
    uint m_hash;

    MiscIdentifierPrivate* m_next;

    MiscEnginePrivate* m_engine;
};

#endif
