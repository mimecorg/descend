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

#ifndef MISCFunction_P_H
#define MISCFunction_P_H

#include "miscfunction.h"

class MiscEnginePrivate;

class MiscFunctionPrivate
{
public:
    MiscFunctionPrivate( Misc::Type returnType, MiscEnginePrivate* engine ) :
        m_returnType( returnType ),
        m_paramsCount( 0 ),
        m_paramTypes( NULL ),
        m_stackSize( 0 ),
        m_opsSize( 0 ),
        m_opsTable( NULL ),
        m_engine( engine )
    {
    }

    ~MiscFunctionPrivate()
    {
    }

public:
    static MiscFunctionPrivate* get( const MiscFunction& function ) { return function.d; }
    static MiscFunction toPublic( MiscFunctionPrivate* data ) { return MiscFunction( data ); }

public:
    Misc::Type m_returnType;

    int m_paramsCount;
    Misc::Type* m_paramTypes;

    int m_stackSize;

    size_t m_opsSize;
    char* m_opsTable;

    MiscEnginePrivate* m_engine;
};

#endif
