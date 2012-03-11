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

#include "misccode.h"
#include "misccode_p.h"

#include "miscengine_p.h"

MiscCode::MiscCode() :
    d( NULL )
{
}

MiscCode::MiscCode( const MiscCode& other ) :
    d( other.d )
{
}

MiscCode::~MiscCode()
{
}

MiscCode::MiscCode( MiscCodePrivate* data ) :
    d( data )
{
}

MiscCode& MiscCode::operator =( const MiscCode& other )
{
    d = other.d;
    return *this;
}

bool operator ==( const MiscCode& lhs, const MiscCode& rhs )
{
    return lhs.d == rhs.d;
}

bool operator !=( const MiscCode& lhs, const MiscCode& rhs )
{
    return lhs.d != rhs.d;
}
