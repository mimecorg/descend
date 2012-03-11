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

#include "miscidentifier.h"
#include "miscidentifier_p.h"

#include "miscengine_p.h"

MiscIdentifier::MiscIdentifier() :
    d( NULL )
{
}

MiscIdentifier::MiscIdentifier( const MiscIdentifier& other ) :
    d( other.d )
{
}

MiscIdentifier::MiscIdentifier( const QString& name, MiscEngine* engine ) :
    d( MiscEnginePrivate::get( engine )->lookupIdentifier( name ) )
{
}

MiscIdentifier::MiscIdentifier( const QStringRef& name, MiscEngine* engine ) :
    d( MiscEnginePrivate::get( engine )->lookupIdentifier( name ) )
{
}

MiscIdentifier::MiscIdentifier( MiscIdentifierPrivate* data ) :
    d( data )
{
}

MiscIdentifier::~MiscIdentifier()
{
}

QString MiscIdentifier::name() const
{
    return d ? d->name() : QString();
}

QString MiscIdentifierPrivate::name() const
{
    return m_engine->identifierAt( m_index );
}

MiscIdentifier& MiscIdentifier::operator =( const MiscIdentifier& other )
{
    d = other.d;
    return *this;
}

bool operator ==( const MiscIdentifier& lhs, const MiscIdentifier& rhs )
{
    return lhs.d == rhs.d;
}

bool operator !=( const MiscIdentifier& lhs, const MiscIdentifier& rhs )
{
    return lhs.d != rhs.d;
}
