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

#ifndef MISCVALUE_H
#define MISCVALUE_H

#include "misc.h"

class MiscValuePrivate;
class MiscEngine;

class QVector4D;
class QMatrix4x4;

class MiscValue
{
public:
    MiscValue();
    MiscValue( const MiscValue& other );
    MiscValue( Misc::Type type, MiscEngine* engine );
    MiscValue( bool b, MiscEngine* engine );
    MiscValue( float f, MiscEngine* engine );
    MiscValue( const QVector4D& v, MiscEngine* engine );
    MiscValue( const QMatrix4x4& m, MiscEngine* engine );
    ~MiscValue();

public:
    bool isValid() const { return d != NULL; }

    Misc::Type type() const;

    bool toBool() const;
    float toFloat() const;
    QVector4D toVector() const;
    QMatrix4x4 toMatrix() const;

public:
    MiscValue& operator =( const MiscValue& other );

    friend bool operator ==( const MiscValue& lhs, const MiscValue& rhs );
    friend bool operator !=( const MiscValue& lhs, const MiscValue& rhs );

private:
    explicit MiscValue( MiscValuePrivate* data );

    // prevent silenty casting pointer to bool
    MiscValue( void* ptr, MiscEngine* engine );

private:
    MiscValuePrivate* d;

    friend class MiscValuePrivate;
};

#endif
