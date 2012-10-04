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

#ifndef VARIANTEX_H
#define VARIANTEX_H

template<class T>
void operator <<( QVariant& data, const T& target )
{
    data = QVariant::fromValue<T>( target );
}

template<class T>
void operator >>( const QVariant& data, T& target )
{
    target = data.value<T>();
}

template<class T1, class T2>
class DeserializeCastHelper
{
public:
    DeserializeCastHelper( T2& target ) : m_target( target ) { }

    friend void operator >>( const QVariant& data, DeserializeCastHelper<T1, T2>& helper )
    {
        T1 value = data.value<T1>();
        helper.m_target = static_cast<T2>( value );
    }

private:
    T2& m_target;
};

template<class T1, class T2>
DeserializeCastHelper<T1, T2> deserialize_cast( T2& target )
{
    return DeserializeCastHelper<T1, T2>( target );
}

#endif
