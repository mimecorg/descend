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

#ifndef MISCENGINE_P_H
#define MISCENGINE_P_H

#include "miscengine.h"
#include "miscpool_p.h"
#include "miscidentifier_p.h"
#include "miscvalue_p.h"

#include <QStringList>

class MiscCodePrivate;
class MiscFunctionPrivate;

class MiscEnginePrivate
{
public:
    MiscEnginePrivate();
    ~MiscEnginePrivate();

public:
    static MiscEnginePrivate* get( MiscEngine* engine ) { return engine->d; }

public:
    void setError( Misc::ErrorCode code, const QString& message = QString(), int lineNumber = 0 );

    Misc::ErrorCode errorCode() const { return m_errorCode; }

    MiscPool* pool() { return &m_pool; }

    const QString& identifierAt( int index ) const
    {
        return m_ids.at( index );
    }

    MiscIdentifierPrivate* lookupIdentifier( const QString& name );
    MiscIdentifierPrivate* lookupIdentifier( const QStringRef& name );

    MISC_FORCEINLINE MiscValuePrivate* zeroValue( Misc::Type type ) const
    {
        return m_zeroValues[ type - 1 ];
    }

    MISC_FORCEINLINE MiscValuePrivate* boolValue( bool b ) const
    {
        return m_boolValues[ (int)b ];
    }

    MISC_FORCEINLINE MiscValuePrivate* newValue( Misc::Type type )
    {
        if ( m_freeValues[ type - 1 ] == NULL )
            return allocValue( type );

        MiscValuePrivate* data = m_freeValues[ type - 1 ];
        m_freeValues[ type - 1 ] = data->m_next;

        Q_ASSERT( data->m_ref == 0 );
        Q_ASSERT( data->m_type == type );

        return data;
    }

    MISC_FORCEINLINE void freeValue( MiscValuePrivate* data )
    {
        Q_ASSERT( data->m_ref == 0 );

        data->m_next = m_freeValues[ data->m_type - 1 ];
        m_freeValues[ data->m_type - 1 ] = data;
    }

    MISC_FORCEINLINE void freeValue( MiscValuePrivate* data, Misc::Type type )
    {
        Q_ASSERT( data->m_ref == 0 );
        Q_ASSERT( data->m_type == type );

        data->m_next = m_freeValues[ type - 1 ];
        m_freeValues[ type - 1 ] = data;
    }

    int allocVariable( Misc::Type type );

    void setVariable( int index, MiscValuePrivate* value );

    MiscValuePrivate* variable( int index ) const;

    int allocFunction();

    void setFunction( int index, MiscFunctionPrivate* function );

    MiscFunctionPrivate* function( int index ) const
    {
        Q_ASSERT( index >= 0 && index < m_functionsCount );

        return m_functionsTable[ index ];
    }

    MiscCodePrivate* newCode( size_t opsSize );

    MiscFunctionPrivate* newFunction( Misc::Type returnType, int paramsCount, size_t opsSize );

public:
    enum RegisteredIdentifiers
    {
        Keywords = 16,
        ConstantFirst = Keywords,
        Constants = 2,
        ConstantLast = ConstantFirst + Constants - 1,
        FunctionFirst,
        Functions = 39,
        FunctionLast = FunctionFirst + Functions - 1,
        Reserved,
        SubscriptFirst = Reserved,
        Subscripts = 20,
        SubscriptLast = SubscriptFirst + Subscripts - 1
    };

private:
    void registerIdentifier( const char* id );

    MiscIdentifierPrivate* newIdentifier( int index, uint hash );

    void reallocIdHashTable();

    MiscValuePrivate* allocValue( Misc::Type type );

    bool execute( char* ops, int sp, int fp );

    MiscValuePrivate* detachVector( MiscValuePrivate* value );
    MiscValuePrivate* detachMatrix( MiscValuePrivate* value );

    int unwind( int sp, int count );

private:
    Misc::ErrorCode m_errorCode;
    QString m_errorMessage;
    int m_errorLineNumber;

    MiscPool m_pool;

    int m_idHashSize;
    int m_idHashCount;
    MiscIdentifierPrivate** m_idHashTable;

    QStringList m_ids;

    MiscValuePrivate* m_zeroValues[ Misc::LastType ];
    MiscValuePrivate* m_boolValues[ 2 ];

    MiscValuePrivate* m_freeValues[ Misc::LastType ];

    int m_variablesSize;
    int m_variablesCount;
    MiscSlot* m_variablesTable;

    int m_functionsSize;
    int m_functionsCount;
    MiscFunctionPrivate** m_functionsTable;

    int m_stackSize;
    MiscSlot* m_stack;

    friend class MiscEngine;
};

#endif
