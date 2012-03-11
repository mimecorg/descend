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

#include "miscvalue.h"
#include "miscvalue_p.h"

#include "miscengine_p.h"
#include "miscmath_p.h"

MiscValue::MiscValue() :
    d( NULL )
{
}

MiscValue::MiscValue( const MiscValue& other ) :
    d( other.d )
{
    if ( d )
        d->m_ref++;
}

MiscValue::MiscValue( Misc::Type type, MiscEngine* engine ) :
    d( MiscEnginePrivate::get( engine )->zeroValue( type ) )
{
    d->m_ref++;
}

MiscValue::MiscValue( bool b, MiscEngine* engine ) :
    d( MiscEnginePrivate::get( engine )->boolValue( b ) )
{
    d->m_ref++;
}

MiscValue::MiscValue( float f, MiscEngine* engine ) :
    d( MiscEnginePrivate::get( engine )->newValue( Misc::FloatType ) )
{
    d->m_ref++;
    d->m_float = f;
}

MiscValue::MiscValue( const QVector4D& v, MiscEngine* engine ) :
    d( MiscEnginePrivate::get( engine )->newValue( Misc::VectorType ) )
{
    d->m_ref++;
    d->setVector( v );
}

MiscValue::MiscValue( const QMatrix4x4& m, MiscEngine* engine ) :
    d( MiscEnginePrivate::get( engine )->newValue( Misc::MatrixType ) )
{
    d->m_ref++;
    d->setMatrix( m );
}

MiscValue::MiscValue( MiscValuePrivate* data ) :
    d( data )
{
    d->m_ref++;
}

MiscValue::~MiscValue()
{
    if ( d && --d->m_ref == 0 )
        d->m_engine->freeValue( d );
}

MiscValuePrivate::MiscValuePrivate( Misc::Type type, MiscEnginePrivate* engine ) :
    m_ref( 0 ),
    m_type( type ),
    m_engine( engine ),
    m_next( NULL )
{
}

MiscValuePrivate::~MiscValuePrivate()
{
}

Misc::Type MiscValue::type() const
{
    return ( d != NULL ) ? d->m_type : Misc::InvalidType;
}

bool MiscValue::toBool() const
{
    return ( d != NULL && d->m_type == Misc::BoolType ) ? d->m_bool : false;
}

float MiscValue::toFloat() const
{
    return ( d != NULL && d->m_type == Misc::FloatType ) ? d->m_float : 0.0f;
}

QVector4D MiscValue::toVector() const
{
    return ( d != NULL && d->m_type == Misc::VectorType ) ? d->vector() : QVector4D();
}

QMatrix4x4 MiscValue::toMatrix() const
{
    return ( d != NULL && d->m_type == Misc::MatrixType ) ? d->matrix() : QMatrix4x4();
}

MiscValue& MiscValue::operator =( const MiscValue& other )
{
    if ( other.d )
        other.d->m_ref++;

    if ( d && --d->m_ref == 0 )
        d->m_engine->freeValue( d );

    d = other.d;

    return *this;
}

bool operator ==( const MiscValue& lhs, const MiscValue& rhs )
{
    if ( lhs.d == NULL && rhs.d == NULL )
        return true;

    if ( lhs.d == NULL || rhs.d == NULL )
        return false;

    if ( lhs.d->m_type != rhs.d->m_type )
        return false;

    switch ( lhs.d->m_type ) {
        case Misc::VoidType:
            return true;

        case Misc::BoolType:
            return lhs.d->m_bool == rhs.d->m_bool;

        case Misc::FloatType:
            return lhs.d->m_float == rhs.d->m_float;

        case Misc::VectorType:
            return Misc::Math::eqvv( lhs.d, rhs.d );

        case Misc::MatrixType:
            return Misc::Math::eqmm( lhs.d, rhs.d );

        default:
            Q_ASSERT( false );
            return false;
    }
}

bool operator !=( const MiscValue& lhs, const MiscValue& rhs )
{
    return !( lhs == rhs );
}
