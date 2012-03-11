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

#include "miscfunction.h"
#include "miscfunction_p.h"

MiscFunction::MiscFunction() :
    d( NULL )
{
}

MiscFunction::MiscFunction( const MiscFunction& other ) :
    d( other.d )
{
}

MiscFunction::MiscFunction( MiscFunctionPrivate* data ) :
    d( data )
{
}

MiscFunction::~MiscFunction()
{
}

MiscFunction& MiscFunction::operator =( const MiscFunction& other )
{
    d = other.d;
    return *this;
}

bool operator ==( const MiscFunction& lhs, const MiscFunction& rhs )
{
    return lhs.d == rhs.d;
}

bool operator !=( const MiscFunction& lhs, const MiscFunction& rhs )
{
    return lhs.d != rhs.d;
}
