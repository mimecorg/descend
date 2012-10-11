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

#include "miscsymbol.h"
#include "miscsymbol_p.h"

#include "miscidentifier_p.h"
#include "miscengine_p.h"

MiscSymbol::MiscSymbol() :
    d( new MiscSymbolPrivate() )
{
}

MiscSymbol::MiscSymbol( Misc::Type type, const MiscIdentifier& id, Misc::SymbolFlags flags /*= 0*/ ) :
    d( new MiscSymbolPrivate( type, id, flags ) )
{
}

MiscSymbol::MiscSymbol( const MiscSymbol& other ) :
    d( other.d )
{
}

MiscSymbol::~MiscSymbol()
{
}

MiscSymbolPrivate::MiscSymbolPrivate() :
    m_type( Misc::InvalidType ),
    m_flags( 0 )
{
}

MiscSymbolPrivate::MiscSymbolPrivate( Misc::Type type, const MiscIdentifier& id, Misc::SymbolFlags flags /*= 0*/ ) :
    m_type( type ),
    m_id( id ),
    m_flags( flags )
{
}

MiscSymbolPrivate::MiscSymbolPrivate( const MiscSymbolPrivate& other ) : QSharedData( other ),
    m_type( other.m_type ),
    m_id( other.m_id ),
    m_flags( other.m_flags ),
    m_params( other.m_params )
{
}

MiscSymbolPrivate::~MiscSymbolPrivate()
{
}

bool MiscSymbol::isValid() const
{
    return d->m_type != Misc::InvalidType;
}

void MiscSymbol::setType( Misc::Type type )
{
    d->m_type = type;
}

Misc::Type MiscSymbol::type() const
{
    return d->m_type;
}

void MiscSymbol::setIdentifier( const MiscIdentifier& id )
{
    d->m_id = id;
}

MiscIdentifier MiscSymbol::identifier() const
{
    return d->m_id;
}

void MiscSymbol::setParameters( const QList<Parameter>& params )
{
    d->m_params = params;
}

QList<MiscSymbol::Parameter> MiscSymbol::parameters() const
{
    return d->m_params;
}

void MiscSymbol::setFlags( Misc::SymbolFlags flags )
{
    d->m_flags = flags;
}

Misc::SymbolFlags MiscSymbol::flags() const
{
    return d ? d->m_flags : Misc::SymbolFlags( 0 );
}

MiscSymbol& MiscSymbol::operator =( const MiscSymbol& other )
{
    d = other.d;
    return *this;
}

bool operator ==( const MiscSymbol& lhs, const MiscSymbol& rhs )
{
    if ( lhs.d->m_type != rhs.d->m_type )
        return false;

    if ( lhs.d->m_flags != rhs.d->m_flags )
        return false;

    if ( lhs.d->m_params != rhs.d->m_params )
        return false;

    return true;
}

bool operator !=( const MiscSymbol& lhs, const MiscSymbol& rhs )
{
    return !( lhs == rhs );
}

MiscSymbol::Parameter::Parameter() :
    m_type( Misc::InvalidType )
{
}

MiscSymbol::Parameter::Parameter( Misc::Type type, const MiscIdentifier& id ) :
    m_type( type ),
    m_id( id )
{
}

MiscSymbol::Parameter::Parameter( const MiscSymbol::Parameter& other ) :
    m_type( other.m_type ),
    m_id( other.m_id )
{
}

MiscSymbol::Parameter::~Parameter()
{
}

MiscSymbol::Parameter& MiscSymbol::Parameter::operator =( const MiscSymbol::Parameter& other )
{
    m_type = other.m_type;
    m_id = other.m_id;
    return *this;
}
