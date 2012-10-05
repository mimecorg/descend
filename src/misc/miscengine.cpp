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

#include "miscengine.h"
#include "miscengine_p.h"

#include "misccode_p.h"
#include "miscfunction_p.h"
#include "miscop_p.h"
#include "miscmath_p.h"

MiscEngine::MiscEngine( QObject* parent /*= NULL*/ ) : QObject( parent ),
    d( new MiscEnginePrivate() )
{
}

MiscEngine::~MiscEngine()
{
    delete d;
}

static const char* const reg_keywords[ MiscEnginePrivate::Keywords ] =
{
    "break", "bool", "const", "continue", "do",
    "else", "false", "float", "for", "if",
    "matrix", "return", "true", "vec", "void",
    "while"
};

static const char* const reg_constants[ MiscEnginePrivate::Constants ] =
{
    "PI", "E"
};

static const char* const reg_functions[ MiscEnginePrivate::Functions ] =
{
    "sin", "cos", "tan", "asin", "acos",
    "atan", "atan2", "sinh", "cosh", "tanh",
    "asinh", "acosh", "atanh", "exp", "log",
    "sqrt", "abs", "sign", "floor", "round",
    "ceil", "min", "max", "clamp", "lerp",
    "isnan", "isinf", "length", "distance", "dot",
    "cross", "normalize", "lerpv", "identity", "translate",
    "scale", "scalev", "rotate", "transpose", "determinant",
    "inverse"
};

static const char* const reg_subscripts[ MiscEnginePrivate::Subscripts ] =
{
    "x", "y", "z", "w",
    "xx", "xy", "xz", "xw",
    "yx", "yy", "yz", "yw",
    "zx", "zy", "zz", "zw",
    "wx", "wy", "wz", "ww"
};

MiscEnginePrivate::MiscEnginePrivate() :
    m_errorCode( Misc::NoError ),
    m_errorLineNumber( 0 ),
    m_idHashSize( 0 ),
    m_idHashCount( 0 ),
    m_idHashTable( NULL ),
    m_variablesSize( 0 ),
    m_variablesCount( 0 ),
    m_variablesTable( NULL ),
    m_functionsSize( 0 ),
    m_functionsCount( 0 ),
    m_functionsTable( NULL ),
    m_stackSize( 4096 ),
    m_stack( NULL )
{
    for ( int i = 0; i < Keywords; i++ )
        registerIdentifier( reg_keywords[ i ] );
    for ( int i = 0; i < Constants; i++ )
        registerIdentifier( reg_constants[ i ] );
    for ( int i = 0; i < Functions; i++ )
        registerIdentifier( reg_functions[ i ] );
    for ( int i = 0; i < Subscripts; i++ )
        registerIdentifier( reg_subscripts[ i ] );

    for ( int i = 0; i < Misc::LastType; i++ ) {
        m_zeroValues[ i ] = allocValue( (Misc::Type)( i + 1 ) );
        m_zeroValues[ i ]->m_ref++;
    }

    zeroValue( Misc::BoolType )->m_bool = false;
    zeroValue( Misc::FloatType )->m_float = 0.0f;
    zeroValue( Misc::VectorType )->setVector( QVector4D() );
    zeroValue( Misc::MatrixType )->setMatrix( QMatrix4x4() );

    m_boolValues[ 0 ] = zeroValue( Misc::BoolType );

    m_boolValues[ 1 ] = allocValue( Misc::BoolType );
    m_boolValues[ 1 ]->m_ref++;
    m_boolValues[ 1 ]->m_bool = true;

    for ( int i = 0; i < Misc::LastType; i++ )
        m_freeValues[ i ] = NULL;
}

MiscEnginePrivate::~MiscEnginePrivate()
{
    free( m_idHashTable );
    free( m_variablesTable );
    free( m_functionsTable );
    free( m_stack );
}

void MiscEnginePrivate::setError( Misc::ErrorCode code, const QString& message /*= QString()*/, int lineNumber /*= 0*/ )
{
    m_errorCode = code;
    m_errorMessage = message;
    m_errorLineNumber = lineNumber;
}

Misc::ErrorCode MiscEngine::errorCode() const
{
    return d->m_errorCode;
}

QString MiscEngine::errorMessage() const
{
    return d->m_errorMessage;
}

int MiscEngine::errorLineNumber() const
{
    return d->m_errorLineNumber;
}

MiscIdentifierPrivate* MiscEnginePrivate::lookupIdentifier( const QString& name )
{
    uint hash = qHash( name );

    MiscIdentifierPrivate* data = m_idHashTable[ hash % m_idHashSize ];

    while ( data != NULL ) {
        if ( data->m_hash == hash && m_ids[ data->m_index ] == name )
            return data;
        data = data->m_next;
    }

    int index = m_ids.count();
    m_ids.append( name );

    return newIdentifier( index, hash );
}

MiscIdentifierPrivate* MiscEnginePrivate::lookupIdentifier( const QStringRef& name )
{
    uint hash = qHash( name );

    MiscIdentifierPrivate* data = m_idHashTable[ hash % m_idHashSize ];

    while ( data != NULL ) {
        if ( data->m_hash == hash && m_ids[ data->m_index ] == name )
            return data;
        data = data->m_next;
    }

    int index = m_ids.count();
    m_ids.append( name.toString() );

    return newIdentifier( index, hash );
}

void MiscEnginePrivate::registerIdentifier( const char* id )
{
    QString name = QString::fromLatin1( id );

    int index = m_ids.count();
    m_ids.append( name );

    newIdentifier( index, qHash( name ) );
}

MiscIdentifierPrivate* MiscEnginePrivate::newIdentifier( int index, uint hash )
{
    MiscIdentifierPrivate* data = new ( &m_pool ) MiscIdentifierPrivate( index, hash, this );

    m_idHashCount++;

    if ( m_idHashCount > m_idHashSize )
        reallocIdHashTable();

    int bucket = hash % m_idHashSize;

    data->m_next = m_idHashTable[ bucket ];
    m_idHashTable[ bucket ] = data;

    return data;
}

void MiscEnginePrivate::reallocIdHashTable()
{
    int size;
    if ( m_idHashSize > 0 )
        size = ( m_idHashSize << 1 ) + 1;
    else
        size = 255;

    MiscIdentifierPrivate** table = static_cast<MiscIdentifierPrivate**>( malloc( size * sizeof( MiscIdentifierPrivate* ) ) );

    for ( int i = 0; i < size; i++ )
        table[ i ] = NULL;

    if ( m_idHashTable != NULL ) {
        for ( int i = 0; i < m_idHashSize; i++ ) {
            MiscIdentifierPrivate* data = m_idHashTable[ i ];

            while ( data != NULL ) {
                MiscIdentifierPrivate* next = data->m_next;

                int bucket = data->m_hash % size;

                data->m_next = table[ bucket ];
                table[ bucket ] = data;

                data = next;
            }
        }

        free( m_idHashTable );
    }

    m_idHashSize = size;
    m_idHashTable = table;
}

#define member_size( type, member ) sizeof( ( (type*)0 )->member )

static const size_t value_size[ Misc::LastType ] =
{
    offsetof( MiscValuePrivate, m_data ),
    offsetof( MiscValuePrivate, m_data ) + member_size( MiscValuePrivate, m_bool ),
    offsetof( MiscValuePrivate, m_data ) + member_size( MiscValuePrivate, m_float ),
    offsetof( MiscValuePrivate, m_data ) + member_size( MiscValuePrivate, m_vector ),
    offsetof( MiscValuePrivate, m_data ) + member_size( MiscValuePrivate, m_matrix )
};

MiscValuePrivate* MiscEnginePrivate::allocValue( Misc::Type type )
{
    return new ( &m_pool, value_size[ type - 1 ] ) MiscValuePrivate( type, this );
}

int MiscEnginePrivate::allocVariable( Misc::Type type )
{
    int index = m_variablesCount++;

    if ( m_variablesCount > m_variablesSize ) {
        if ( m_variablesSize == 0 )
            m_variablesSize = 128;
        else
            m_variablesSize <<= 2;

        m_variablesTable = static_cast<MiscSlot*>( realloc( m_variablesTable, m_variablesSize * sizeof( MiscSlot ) ) );
    }

    m_variablesTable[ index ].m_type = type;

    switch ( type ) {
        case Misc::BoolType:
            m_variablesTable[ index ].m_bool = false;
            break;

        case Misc::FloatType:
            m_variablesTable[ index ].m_float = 0.0f;
            break;

        case Misc::VectorType:
        case Misc::MatrixType:
            m_variablesTable[ index ].m_value = zeroValue( type );
            m_variablesTable[ index ].m_value->m_ref++;
            break;

	default:
	    Q_ASSERT( false );
	    break;
    }

    return index;
}

void MiscEnginePrivate::setVariable( int index, MiscValuePrivate* value )
{
    Q_ASSERT( index >= 0 && index < m_variablesCount );
    Q_ASSERT( value != NULL );

    Q_ASSERT( value->m_type == m_variablesTable[ index ].m_type );

    switch ( value->m_type ) {
        case Misc::BoolType:
            m_variablesTable[ index ].m_bool = value->m_bool;
            break;

        case Misc::FloatType:
            m_variablesTable[ index ].m_float = value->m_float;
            break;

        case Misc::VectorType:
        case Misc::MatrixType:
            value->m_ref++;
            if ( --m_variablesTable[ index ].m_value->m_ref == 0 )
                freeValue( m_variablesTable[ index ].m_value );
            m_variablesTable[ index ].m_value = value;
            break;

	default:
	    Q_ASSERT( false );
	    break;
    }
}

MiscValuePrivate* MiscEnginePrivate::variable( int index ) const
{
    Q_ASSERT( index >= 0 && index < m_variablesCount );

    switch ( m_variablesTable[ index ].m_type ) {
        case Misc::BoolType:
            return boolValue( m_variablesTable[ index ].m_bool );

        case Misc::FloatType: {
            MiscValuePrivate* value = const_cast<MiscEnginePrivate*>( this )->newValue( Misc::FloatType );
            value->m_float = m_variablesTable[ index ].m_float;
            return value;
        }

        case Misc::VectorType:
        case Misc::MatrixType:
            return m_variablesTable[ index ].m_value;

	default:
	    Q_ASSERT( false );
	    return NULL;
    }
}

int MiscEnginePrivate::allocFunction()
{
    int index = m_functionsCount++;

    if ( m_functionsCount > m_functionsSize ) {
        if ( m_functionsSize == 0 )
            m_functionsSize = 128;
        else
            m_functionsSize <<= 2;

        m_functionsTable = static_cast<MiscFunctionPrivate**>( realloc( m_functionsTable, m_functionsSize * sizeof( MiscFunctionPrivate* ) ) );
    }

    m_functionsTable[ index ] = NULL;

    return index;
}

void MiscEnginePrivate::setFunction( int index, MiscFunctionPrivate* function )
{
    Q_ASSERT( index >= 0 && index < m_functionsCount );

    m_functionsTable[ index ] = function;
}

MiscCodePrivate* MiscEnginePrivate::newCode( size_t opsSize )
{
    MiscCodePrivate* code = new ( &m_pool ) MiscCodePrivate( this );

    code->m_opsSize = opsSize;
    code->m_opsTable = static_cast<char*>( m_pool.alloc( opsSize ) );

    return code;
}

MiscFunctionPrivate* MiscEnginePrivate::newFunction( Misc::Type returnType, int paramsCount, size_t opsSize )
{
    MiscFunctionPrivate* function = new ( &m_pool ) MiscFunctionPrivate( returnType, this );

    if ( paramsCount != 0 ) {
        function->m_paramsCount = paramsCount;
        function->m_paramTypes = static_cast<Misc::Type*>( m_pool.alloc( paramsCount * sizeof( Misc::Type ) ) );
    }

    function->m_opsSize = opsSize;
    function->m_opsTable = static_cast<char*>( m_pool.alloc( opsSize ) );

    return function;
}

bool MiscEngine::execute( const MiscCode& code )
{
    d->setError( Misc::NoError );

    if ( !code.isValid() ) {
        d->setError( Misc::ArgumentError, "invalid code" );
        return false;
    }

    MiscCodePrivate* data = MiscCodePrivate::get( code );

    if ( data->m_stackSize > d->m_stackSize ) {
        d->setError( Misc::RuntimeError, "stack overflow" );
        return false;
    }

    d->m_stack = static_cast<MiscSlot*>( realloc( d->m_stack, d->m_stackSize * sizeof( MiscSlot ) ) );

    if ( !d->execute( data->m_opsTable, 0, 0 ) )
        return false;

    return true;
}

#define PUSH( type ) \
    do { \
        m_stack[ sp ].m_type = Misc::type##Type; \
        if ( Misc::type##Type == Misc::BoolType ) { \
            m_stack[ sp ].m_bool = result.m_bool; \
        } else if ( Misc::type##Type == Misc::FloatType ) { \
            m_stack[ sp ].m_float = result.m_float; \
        } else if ( Misc::type##Type == Misc::VectorType || Misc::type##Type == Misc::MatrixType ) { \
            m_stack[ sp ].m_value = result.m_value; \
            m_stack[ sp ].m_value->m_ref++; \
        } \
        sp++; \
    } while ( false )

#define POP( type ) \
    do { \
        --sp; \
        if ( Misc::type##Type == Misc::VectorType || Misc::type##Type == Misc::MatrixType ) { \
            if ( --m_stack[ sp ].m_value->m_ref == 0 ) \
                freeValue( m_stack[ sp ].m_value, Misc::type##Type ); \
        } \
    } while ( false )

#define OP_UNARY( r, a ) \
    do { \
        POP( a ); \
        PUSH( r ); \
        cp += Misc::OP::InstrSize; \
    } while ( false )

#define OP_BINARY( r, a, b ) \
    do { \
        POP( b ); \
        POP( a ); \
        PUSH( r ); \
        cp += Misc::OP::InstrSize; \
    } while ( false )

#define OP_TERNARY( r, a, b, c ) \
    do { \
        POP( c ); \
        POP( b ); \
        POP( a ); \
        PUSH( r ); \
        cp += Misc::OP::InstrSize; \
    } while ( false )

#define OP_SIMPLE( fn ) \
    do { \
        result.m_float = fn( m_stack[ sp - 1 ].m_float ); \
        OP_UNARY( Float, Float ); \
    } while ( false )

bool MiscEnginePrivate::execute( char* ops, int sp, int fp )
{
    char* cp = ops;

    for ( ; ; ) {
        Misc::OP::Instruction* instr = reinterpret_cast<Misc::OP::Instruction*>( cp );
        MiscSlot result;

        switch ( instr->m_code ) {
            case Misc::OP::push_void:
                m_stack[ sp++ ].m_type = Misc::VoidType;
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::push_false:
                m_stack[ sp ].m_type = Misc::BoolType;
                m_stack[ sp++ ].m_bool = false;
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::push_true:
                m_stack[ sp ].m_type = Misc::BoolType;
                m_stack[ sp++ ].m_bool = true;
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::push_zerof:
                m_stack[ sp ].m_type = Misc::FloatType;
                m_stack[ sp++ ].m_float = 0.0f;
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::push_zerov:
                m_stack[ sp ].m_type = Misc::VectorType;
                m_stack[ sp ].m_value = zeroValue( Misc::VectorType );
                m_stack[ sp++ ].m_value->m_ref++;
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::push_zerom:
                m_stack[ sp ].m_type = Misc::MatrixType;
                m_stack[ sp ].m_value = zeroValue( Misc::MatrixType );
                m_stack[ sp++ ].m_value->m_ref++;
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::push_float:
                m_stack[ sp ].m_type = Misc::FloatType;
                m_stack[ sp++ ].m_float = instr->m_float;
                cp += Misc::OP::InstrFloatSize;
                break;

            case Misc::OP::make_vec2:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::makev<2>( result.m_value, m_stack + sp );
                sp -= 2;
                PUSH( Vector );
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::make_vec3:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::makev<3>( result.m_value, m_stack + sp );
                sp -= 3;
                PUSH( Vector );
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::make_vec4:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::makev<4>( result.m_value, m_stack + sp );
                sp -= 4;
                PUSH( Vector );
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::make_mat2:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::makem<2>( result.m_value, m_stack + sp );
                sp -= 4;
                PUSH( Matrix );
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::make_mat3:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::makem<3>( result.m_value, m_stack + sp );
                sp -= 9;
                PUSH( Matrix );
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::make_mat4:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::makem<4>( result.m_value, m_stack + sp );
                sp -= 16;
                PUSH( Matrix );
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::op_nop:
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::op_negf:
                result.m_float = -m_stack[ sp - 1 ].m_float;
                OP_UNARY( Float, Float );
                break;

            case Misc::OP::op_negv:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::negv( result.m_value, m_stack[ sp - 1 ].m_value );
                OP_UNARY( Vector, Vector );
                break;

            case Misc::OP::op_negm:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::negm( result.m_value, m_stack[ sp - 1 ].m_value );
                OP_UNARY( Matrix, Matrix );
                break;

            case Misc::OP::op_notb:
                result.m_bool = !m_stack[ sp - 1 ].m_bool;
                OP_UNARY( Bool, Bool );
                break;

            case Misc::OP::op_addff:
                result.m_float = m_stack[ sp - 2 ].m_float + m_stack[ sp - 1 ].m_float;
                OP_BINARY( Float, Float, Float );
                break;

            case Misc::OP::op_addvv:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::addvv( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Vector, Vector, Vector );
                break;

            case Misc::OP::op_addmm:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::addmm( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Matrix, Matrix, Matrix );
                break;

            case Misc::OP::op_subff:
                result.m_float = m_stack[ sp - 2 ].m_float - m_stack[ sp - 1 ].m_float;
                OP_BINARY( Float, Float, Float );
                break;

            case Misc::OP::op_subvv:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::subvv( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Vector, Vector, Vector );
                break;

            case Misc::OP::op_submm:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::submm( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Matrix, Matrix, Matrix );
                break;

            case Misc::OP::op_mulff:
                result.m_float = m_stack[ sp - 2 ].m_float * m_stack[ sp - 1 ].m_float;
                OP_BINARY( Float, Float, Float );
                break;

            case Misc::OP::op_mulfv:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::mulfv( result.m_value, m_stack[ sp - 2 ].m_float, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Vector, Float, Vector );
                break;

            case Misc::OP::op_mulvf:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::mulvf( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_float );
                OP_BINARY( Vector, Vector, Float );
                break;

            case Misc::OP::op_mulvv:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::mulvv( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Vector, Vector, Vector );
                break;

            case Misc::OP::op_mulfm:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::mulfm( result.m_value, m_stack[ sp - 2 ].m_float, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Matrix, Float, Matrix );
                break;

            case Misc::OP::op_mulmf:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::mulmf( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_float );
                OP_BINARY( Matrix, Matrix, Float );
                break;

            case Misc::OP::op_mulvm:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::mulvm( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Vector, Vector, Matrix );
                break;

            case Misc::OP::op_mulmv:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::mulmv( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Vector, Matrix, Vector );
                break;

            case Misc::OP::op_mulmm:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::mulmm( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Matrix, Matrix, Matrix );
                break;

            case Misc::OP::op_divff:
                result.m_float = m_stack[ sp - 2 ].m_float / m_stack[ sp - 1 ].m_float;
                OP_BINARY( Float, Float, Float );
                break;

            case Misc::OP::op_divvf:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::mulvf( result.m_value, m_stack[ sp - 2 ].m_value, 1.0f / m_stack[ sp - 1 ].m_float );
                OP_BINARY( Vector, Vector, Float );
                break;

            case Misc::OP::op_divmf:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::mulmf( result.m_value, m_stack[ sp - 2 ].m_value, 1.0f / m_stack[ sp - 1 ].m_float );
                OP_BINARY( Matrix, Matrix, Float );
                break;

            case Misc::OP::op_modff:
                result.m_float = fmod( m_stack[ sp - 2 ].m_float, m_stack[ sp - 1 ].m_float );
                OP_BINARY( Float, Float, Float );
                break;

            case Misc::OP::op_powff:
                result.m_float = pow( m_stack[ sp - 2 ].m_float, m_stack[ sp - 1 ].m_float );
                OP_BINARY( Float, Float, Float );
                break;

            case Misc::OP::op_eqbb:
                result.m_bool = ( m_stack[ sp - 2 ].m_bool == m_stack[ sp - 1 ].m_bool );
                OP_BINARY( Bool, Bool, Bool );
                break;

            case Misc::OP::op_eqff:
                result.m_bool = ( m_stack[ sp - 2 ].m_float == m_stack[ sp - 1 ].m_float );
                OP_BINARY( Bool, Float, Float );
                break;

            case Misc::OP::op_eqvv:
                result.m_bool = Misc::Math::eqvv( m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Bool, Vector, Vector );
                break;

            case Misc::OP::op_eqmm:
                result.m_bool = Misc::Math::eqmm( m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Bool, Matrix, Matrix );
                break;

            case Misc::OP::op_neqbb:
                result.m_bool = ( m_stack[ sp - 2 ].m_bool != m_stack[ sp - 1 ].m_bool );
                OP_BINARY( Bool, Bool, Bool );
                break;

            case Misc::OP::op_neqff:
                result.m_bool = ( m_stack[ sp - 2 ].m_float != m_stack[ sp - 1 ].m_float );
                OP_BINARY( Bool, Float, Float );
                break;

            case Misc::OP::op_neqvv:
                result.m_bool = !Misc::Math::eqvv( m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Bool, Vector, Vector );
                break;

            case Misc::OP::op_neqmm:
                result.m_bool =  !Misc::Math::eqmm( m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Bool, Matrix, Matrix );
                break;

            case Misc::OP::op_ltff:
                result.m_bool = ( m_stack[ sp - 2 ].m_float < m_stack[ sp - 1 ].m_float );
                OP_BINARY( Bool, Float, Float );
                break;

            case Misc::OP::op_lteff:
                result.m_bool = ( m_stack[ sp - 2 ].m_float <= m_stack[ sp - 1 ].m_float );
                OP_BINARY( Bool, Float, Float );
                break;

            case Misc::OP::op_gtff:
                result.m_bool = ( m_stack[ sp - 2 ].m_float > m_stack[ sp - 1 ].m_float );
                OP_BINARY( Bool, Float, Float );
                break;

            case Misc::OP::op_gteff:
                result.m_bool = ( m_stack[ sp - 2 ].m_float >= m_stack[ sp - 1 ].m_float );
                OP_BINARY( Bool, Float, Float );
                break;

            case Misc::OP::op_andbb:
                result.m_bool = ( m_stack[ sp - 2 ].m_bool & m_stack[ sp - 1 ].m_bool );
                OP_BINARY( Bool, Bool, Bool );
                break;

            case Misc::OP::op_orbb:
                result.m_bool = ( m_stack[ sp - 2 ].m_bool | m_stack[ sp - 1 ].m_bool );
                OP_BINARY( Bool, Bool, Bool );
                break;

            case Misc::OP::fn_sin:
                OP_SIMPLE( sinf );
                break;

            case Misc::OP::fn_cos:
                OP_SIMPLE( cosf );
                break;

            case Misc::OP::fn_tan:
                OP_SIMPLE( tanf );
                break;

            case Misc::OP::fn_asin:
                OP_SIMPLE( asinf );
                break;

            case Misc::OP::fn_acos:
                OP_SIMPLE( acosf );
                break;

            case Misc::OP::fn_atan:
                OP_SIMPLE( atanf );
                break;

            case Misc::OP::fn_atan2:
                result.m_float = atan2f( m_stack[ sp - 2 ].m_float, m_stack[ sp - 1 ].m_float );
                OP_BINARY( Float, Float, Float );
                break;

            case Misc::OP::fn_sinh:
                OP_SIMPLE( sinhf );
                break;

            case Misc::OP::fn_cosh:
                OP_SIMPLE( coshf );
                break;

            case Misc::OP::fn_tanh:
                OP_SIMPLE( tanhf );
                break;

            case Misc::OP::fn_asinh:
                OP_SIMPLE( asinhf );
                break;

            case Misc::OP::fn_acosh:
                OP_SIMPLE( acoshf );
                break;

            case Misc::OP::fn_atanh:
                OP_SIMPLE( atanhf );
                break;

            case Misc::OP::fn_exp:
                OP_SIMPLE( expf );
                break;

            case Misc::OP::fn_log:
                OP_SIMPLE( logf );
                break;

            case Misc::OP::fn_sqrt:
                OP_SIMPLE( sqrtf );
                break;

            case Misc::OP::fn_abs:
                OP_SIMPLE( fabsf );
                break;

            case Misc::OP::fn_sign:
                OP_SIMPLE( Misc::Math::signf );
                break;

            case Misc::OP::fn_floor:
                OP_SIMPLE( floorf );
                break;

            case Misc::OP::fn_round:
                OP_SIMPLE( roundf );
                break;

            case Misc::OP::fn_ceil:
                OP_SIMPLE( ceilf );
                break;

            case Misc::OP::fn_min:
                result.m_float = qMin( m_stack[ sp - 2 ].m_float, m_stack[ sp - 1 ].m_float );
                OP_BINARY( Float, Float, Float );
                break;

            case Misc::OP::fn_max:
                result.m_float = qMax( m_stack[ sp - 2 ].m_float, m_stack[ sp - 1 ].m_float );
                OP_BINARY( Float, Float, Float );
                break;

            case Misc::OP::fn_clamp:
                result.m_float = qBound( m_stack[ sp - 3 ].m_float, m_stack[ sp - 2 ].m_float, m_stack[ sp - 1 ].m_float );
                OP_TERNARY( Float, Float, Float, Float );
                break;

            case Misc::OP::fn_lerp:
                result.m_float = Misc::Math::lerpf( m_stack[ sp - 3 ].m_float, m_stack[ sp - 2 ].m_float, m_stack[ sp - 1 ].m_float );
                OP_TERNARY( Float, Float, Float, Float );
                break;

            case Misc::OP::fn_isnan:
                result.m_bool = isnanf( m_stack[ sp - 1 ].m_float );
                OP_UNARY( Bool, Float );
                break;

            case Misc::OP::fn_isinf:
                result.m_bool = isinff( m_stack[ sp - 1 ].m_float );
                OP_UNARY( Bool, Float );
                break;

            case Misc::OP::fn_length:
                result.m_float = Misc::Math::length( m_stack[ sp - 1 ].m_value );
                OP_UNARY( Float, Vector );
                break;

            case Misc::OP::fn_distance:
                result.m_float = Misc::Math::distance( m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Float, Vector, Vector );
                break;

            case Misc::OP::fn_dot:
                result.m_float = Misc::Math::dot( m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Float, Vector, Vector );
                break;

            case Misc::OP::fn_cross:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::cross( result.m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Vector, Vector, Vector );
                break;

            case Misc::OP::fn_normalize:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::normalize( result.m_value, m_stack[ sp - 1 ].m_value );
                OP_UNARY( Vector, Vector );
                break;

            case Misc::OP::fn_lerpv:
                result.m_value = newValue( Misc::VectorType );
                Misc::Math::lerpv( result.m_value, m_stack[ sp - 3 ].m_value, m_stack[ sp - 2 ].m_value, m_stack[ sp - 1 ].m_float );
                OP_TERNARY( Vector, Vector, Vector, Float );
                break;

            case Misc::OP::fn_identity:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::identity( result.m_value );
                PUSH( Matrix );
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::fn_translate:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::translate( result.m_value, m_stack[ sp - 1 ].m_value );
                OP_UNARY( Matrix, Vector );
                break;

            case Misc::OP::fn_scale:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::scale( result.m_value, m_stack[ sp - 1 ].m_float );
                OP_UNARY( Matrix, Float );
                break;

            case Misc::OP::fn_scalev:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::scalev( result.m_value, m_stack[ sp - 1 ].m_value );
                OP_UNARY( Matrix, Vector );
                break;

            case Misc::OP::fn_rotate:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::rotate( result.m_value, m_stack[ sp - 2 ].m_float, m_stack[ sp - 1 ].m_value );
                OP_BINARY( Matrix, Float, Vector );
                break;

            case Misc::OP::fn_transpose:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::transpose( result.m_value, m_stack[ sp - 1 ].m_value );
                OP_UNARY( Matrix, Matrix );
                break;

            case Misc::OP::fn_determinant:
                result.m_float = Misc::Math::determinant( m_stack[ sp - 1 ].m_value );
                OP_UNARY( Float, Matrix );
                break;

            case Misc::OP::fn_inverse:
                result.m_value = newValue( Misc::MatrixType );
                Misc::Math::inverse( result.m_value, m_stack[ sp - 1 ].m_value );
                OP_UNARY( Matrix, Matrix );
                break;

            case Misc::OP::load_gvarb:
                m_stack[ sp ].m_type = Misc::BoolType;
                m_stack[ sp++ ].m_bool = m_variablesTable[ instr->m_arg[ 0 ] ].m_bool;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::load_gvarf:
                m_stack[ sp ].m_type = Misc::FloatType;
                m_stack[ sp++ ].m_float = m_variablesTable[ instr->m_arg[ 0 ] ].m_float;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::load_gvarv:
                m_stack[ sp ].m_type = Misc::VectorType;
                m_stack[ sp ].m_value = m_variablesTable[ instr->m_arg[ 0 ] ].m_value;
                m_stack[ sp++ ].m_value->m_ref++;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::load_gvarm:
                m_stack[ sp ].m_type = Misc::MatrixType;
                m_stack[ sp ].m_value = m_variablesTable[ instr->m_arg[ 0 ] ].m_value;
                m_stack[ sp++ ].m_value->m_ref++;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::load_lvarb:
                m_stack[ sp ].m_type = Misc::BoolType;
                m_stack[ sp++ ].m_bool = m_stack[ fp + instr->m_arg[ 0 ] ].m_bool;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::load_lvarf:
                m_stack[ sp ].m_type = Misc::FloatType;
                m_stack[ sp++ ].m_float = m_stack[ fp + instr->m_arg[ 0 ] ].m_float;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::load_lvarv:
                m_stack[ sp ].m_type = Misc::VectorType;
                m_stack[ sp ].m_value = m_stack[ fp + instr->m_arg[ 0 ] ].m_value;
                m_stack[ sp++ ].m_value->m_ref++;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::load_lvarm:
                m_stack[ sp ].m_type = Misc::MatrixType;
                m_stack[ sp ].m_value = m_stack[ fp + instr->m_arg[ 0 ] ].m_value;
                m_stack[ sp++ ].m_value->m_ref++;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::load_gcompv:
            case Misc::OP::load_gcompm:
                m_stack[ sp ].m_type = Misc::FloatType;
                m_stack[ sp++ ].m_float = m_variablesTable[ instr->m_arg[ 0 ] ].m_value->m_vector[ instr->m_arg[ 1 ] ];
                cp += Misc::OP::InstrArg2Size;
                break;

            case Misc::OP::load_lcompv:
            case Misc::OP::load_lcompm:
                m_stack[ sp ].m_type = Misc::FloatType;
                m_stack[ sp++ ].m_float = m_stack[ fp + instr->m_arg[ 0 ] ].m_value->m_vector[ instr->m_arg[ 1 ] ];
                cp += Misc::OP::InstrArg2Size;
                break;

            case Misc::OP::store_gvarb:
                m_variablesTable[ instr->m_arg[ 0 ] ].m_bool = m_stack[ --sp ].m_bool;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::store_gvarf:
                m_variablesTable[ instr->m_arg[ 0 ] ].m_float = m_stack[ --sp ].m_float;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::store_gvarv:
                if ( --m_variablesTable[ instr->m_arg[ 0 ] ].m_value->m_ref == 0 )
                    freeValue( m_variablesTable[ instr->m_arg[ 0 ] ].m_value, Misc::VectorType );
                m_variablesTable[ instr->m_arg[ 0 ] ].m_value = m_stack[ --sp ].m_value;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::store_gvarm:
                if ( --m_variablesTable[ instr->m_arg[ 0 ] ].m_value->m_ref == 0 )
                    freeValue( m_variablesTable[ instr->m_arg[ 0 ] ].m_value, Misc::MatrixType );
                m_variablesTable[ instr->m_arg[ 0 ] ].m_value = m_stack[ --sp ].m_value;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::store_lvarb:
                m_stack[ fp + instr->m_arg[ 0 ] ].m_bool = m_stack[ --sp ].m_bool;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::store_lvarf:
                m_stack[ fp + instr->m_arg[ 0 ] ].m_float = m_stack[ --sp ].m_float;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::store_lvarv:
                if ( --m_stack[ fp + instr->m_arg[ 0 ] ].m_value->m_ref == 0 )
                    freeValue( m_stack[ fp + instr->m_arg[ 0 ] ].m_value, Misc::VectorType );
                m_stack[ fp + instr->m_arg[ 0 ] ].m_value = m_stack[ --sp ].m_value;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::store_lvarm:
                if ( --m_stack[ fp + instr->m_arg[ 0 ] ].m_value->m_ref == 0 )
                    freeValue( m_stack[ fp + instr->m_arg[ 0 ] ].m_value, Misc::MatrixType );
                m_stack[ fp + instr->m_arg[ 0 ] ].m_value = m_stack[ --sp ].m_value;
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::store_gcompv:
                if ( m_variablesTable[ instr->m_arg[ 0 ] ].m_value->m_ref > 1 )
                    m_variablesTable[ instr->m_arg[ 0 ] ].m_value = detachVector( m_variablesTable[ instr->m_arg[ 0 ] ].m_value );
                m_variablesTable[ instr->m_arg[ 0 ] ].m_value->m_vector[ instr->m_arg[ 1 ] ] = m_stack[ --sp ].m_float;
                cp += Misc::OP::InstrArg2Size;
                break;

            case Misc::OP::store_gcompm:
                if ( m_variablesTable[ instr->m_arg[ 0 ] ].m_value->m_ref > 1 )
                    m_variablesTable[ instr->m_arg[ 0 ] ].m_value = detachMatrix( m_variablesTable[ instr->m_arg[ 0 ] ].m_value );
                m_variablesTable[ instr->m_arg[ 0 ] ].m_value->m_vector[ instr->m_arg[ 1 ] ] = m_stack[ --sp ].m_float;
                cp += Misc::OP::InstrArg2Size;
                break;

            case Misc::OP::store_lcompv:
                if ( m_stack[ fp + instr->m_arg[ 0 ] ].m_value->m_ref > 1 )
                    m_stack[ fp + instr->m_arg[ 0 ] ].m_value = detachVector( m_stack[ fp + instr->m_arg[ 0 ] ].m_value );
                m_stack[ fp + instr->m_arg[ 0 ] ].m_value->m_vector[ instr->m_arg[ 1 ] ] = m_stack[ --sp ].m_float;
                cp += Misc::OP::InstrArg2Size;
                break;

            case Misc::OP::store_lcompm:
                if ( m_stack[ fp + instr->m_arg[ 0 ] ].m_value->m_ref > 1 )
                    m_stack[ fp + instr->m_arg[ 0 ] ].m_value = detachMatrix( m_stack[ fp + instr->m_arg[ 0 ] ].m_value );
                m_stack[ fp + instr->m_arg[ 0 ] ].m_value->m_vector[ instr->m_arg[ 1 ] ] = m_stack[ --sp ].m_float;
                cp += Misc::OP::InstrArg2Size;
                break;

            case Misc::OP::jmp:
                cp = ops + instr->m_arg[ 0 ];
                break;

            case Misc::OP::jmp_false:
                if ( m_stack[ sp - 1 ].m_bool == false )
                    cp = ops + instr->m_arg[ 0 ];
                else
                    cp += Misc::OP::InstrArg1Size;
                POP( Bool );
                break;

            case Misc::OP::jmp_true:
                if ( m_stack[ sp - 1 ].m_bool == true )
                    cp = ops + instr->m_arg[ 0 ];
                else
                    cp += Misc::OP::InstrArg1Size;
                POP( Bool );
                break;

            case Misc::OP::jmp_and:
                if ( m_stack[ sp - 1 ].m_bool == false ) {
                    cp = ops + instr->m_arg[ 0 ];
                } else {
                    cp += Misc::OP::InstrArg1Size;
                    POP( Bool );
                }
                break;

            case Misc::OP::jmp_or:
                if ( m_stack[ sp - 1 ].m_bool == true ) {
                    cp = ops + instr->m_arg[ 0 ];
                } else {
                    cp += Misc::OP::InstrArg1Size;
                    POP( Bool );
                }
                break;

            case Misc::OP::call: {
                MiscFunctionPrivate* function = m_functionsTable[ instr->m_arg[ 0 ] ];

                if ( function == NULL ) {
                    unwind( sp, sp - fp );
                    setError( Misc::RuntimeError, "invalid function call" );
                    return false;
                }

                int frame = sp - function->m_paramsCount;

                if ( function->m_stackSize > m_stackSize - frame ) {
                    unwind( sp, sp - fp );
                    setError( Misc::RuntimeError, "stack overflow" );
                    return false;
                }

                if ( !execute( function->m_opsTable, sp, frame ) ) {
                    unwind( frame, frame - fp );
                    return false;
                }

                sp = frame + 1;
                cp += Misc::OP::InstrArg1Size;
                break;
            }

            case Misc::OP::pop:
                --sp;
                if ( m_stack[ sp ].m_type == Misc::VectorType || m_stack[ sp ].m_type == Misc::MatrixType ) {
                    if ( --m_stack[ sp ].m_value->m_ref == 0 )
                        freeValue( m_stack[ sp ].m_value );
                }
                cp += Misc::OP::InstrSize;
                break;

            case Misc::OP::pop_n:
                sp = unwind( sp, instr->m_arg[ 0 ] );
                cp += Misc::OP::InstrArg1Size;
                break;

            case Misc::OP::ret:
                Q_ASSERT( sp == fp + 1 );
                return true;

            case Misc::OP::ret_n:
                Q_ASSERT( sp == fp + instr->m_arg[ 0 ] + 1 );
                sp = unwind( sp - 1, instr->m_arg[ 0 ] );
                m_stack[ sp ] = m_stack[ sp + instr->m_arg[ 0 ] ];
                return true;

            case Misc::OP::exit:
                Q_ASSERT( sp == 0 );
                return true;

            default:
#if defined( Q_CC_MSVC ) && !defined( MISC_DEBUG )
                __assume( 0 );
#endif
                qFatal( "MiscEngine: illegal instruction" );
        }
    }

    return true;
}

MiscValuePrivate* MiscEnginePrivate::detachVector( MiscValuePrivate* value )
{
    value->m_ref--;

    MiscValuePrivate* copy = newValue( Misc::VectorType );
    Misc::Math::copyv( copy, value );
    copy->m_ref++;

    return copy;
}

MiscValuePrivate* MiscEnginePrivate::detachMatrix( MiscValuePrivate* value )
{
    value->m_ref--;

    MiscValuePrivate* copy = newValue( Misc::MatrixType );
    Misc::Math::copym( copy, value );
    copy->m_ref++;

    return copy;
}

int MiscEnginePrivate::unwind( int sp, int count )
{
    for ( int i = 0; i < count; i++ ) {
        --sp;
        if ( m_stack[ sp ].m_type == Misc::VectorType || m_stack[ sp ].m_type == Misc::MatrixType ) {
            if ( --m_stack[ sp ].m_value->m_ref == 0 )
                freeValue( m_stack[ sp ].m_value );
        }
    }
    return sp;
}
