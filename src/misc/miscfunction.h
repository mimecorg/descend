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

#ifndef MISCFUNCTION_H
#define MISCFUNCTION_H

#include "misc.h"

class MiscFunctionPrivate;

class MiscFunction
{
public:
    MiscFunction();
    MiscFunction( const MiscFunction& other );
    ~MiscFunction();

public:
    bool isValid() const { return d != NULL; }

public:
    MiscFunction& operator =( const MiscFunction& other );

    friend bool operator ==( const MiscFunction& lhs, const MiscFunction& rhs );
    friend bool operator !=( const MiscFunction& lhs, const MiscFunction& rhs );

private:
    explicit MiscFunction( MiscFunctionPrivate* data );

private:
    MiscFunctionPrivate* d;

    friend class MiscFunctionPrivate;
};

#endif
