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

#include "misccompiler_p.h"

#include "miscengine_p.h"
#include "miscunit_p.h"
#include "miscscope_p.h"
#include "miscast_p.h"
#include "misccode_p.h"
#include "miscfunction_p.h"

MiscCompiler::MiscCompiler( MiscUnitPrivate* unit ) :
    m_unit( unit ),
    m_scope( NULL ),
    m_engine( unit->engine() ),
    m_opsCapacity( 0 ),
    m_opsSize( 0 ),
    m_opsTable( NULL ),
    m_stackCapacity( 0 ),
    m_stackSize( 0 ),
    m_stackTop( 0 ),
    m_stack( NULL )
{
}

MiscCompiler::~MiscCompiler()
{
    free( m_opsTable );
    free( m_stack );
}

MiscCodePrivate* MiscCompiler::compileUnit( Misc::AST::UnitNode* node )
{
    m_scope = NULL;
    m_opsSize = 0;
    m_stackSize = 0;
    m_stackTop = 0;

    node->accept( this );

    if ( !isValid() )
        return NULL;

    Q_ASSERT( m_stackTop == 0 );

    append( Misc::OP::exit );

    return createCode();
}

MiscFunctionPrivate* MiscCompiler::compileFunction( const MiscSymbol& symbol, Misc::AST::Node* node )
{
    MiscScope scope( m_engine );
    scope.setFunction( symbol );

    m_scope = &scope;
    m_opsSize = 0;
    m_stackSize = 0;
    m_stackTop = 0;

    int params = symbol.parameters().count();

    for ( int i = 0; i < params; i++ )
        push( symbol.parameters().at( i ).type() );

    node->accept( this );

    if ( !isValid() )
        return NULL;

    if ( node->m_kind == Misc::AST::Block ) {
        Q_ASSERT( m_stackTop == params );

        appendPushDefault( symbol.type() );
    } else {
        Q_ASSERT( m_stackTop == params + 1 );

        if ( m_stack[ params ] != symbol.type() ) {
            m_engine->setError( Misc::TypeError, "invalid return type", node->m_line );
            return NULL;
        }
    }

    if ( params == 0 )
        append( Misc::OP::ret );
    else
        append( Misc::OP::ret_n, params );

    return createFunction( symbol );
}

MiscCodePrivate* MiscCompiler::compileInitializer( int slot, Misc::Type type, Misc::AST::Node* node )
{
    m_scope = NULL;
    m_opsSize = 0;
    m_stackSize = 0;
    m_stackTop = 0;

    node->accept( this );

    if ( !isValid() )
        return NULL;

    Q_ASSERT( m_stackTop == 1 );

    if ( m_stack[ 0 ] != type ) {
        m_engine->setError( Misc::TypeError, "invalid initializer type", node->m_line );
        return NULL;
    }

    appendStoreGlobal( type, slot );
    append( Misc::OP::exit );

    return createCode();
}

MiscCodePrivate* MiscCompiler::createCode()
{
    MiscCodePrivate* code = m_engine->newCode( m_opsSize );

    code->m_stackSize = m_stackSize;

    memcpy( code->m_opsTable, m_opsTable, m_opsSize );

    return code;
}

MiscFunctionPrivate* MiscCompiler::createFunction( const MiscSymbol& symbol )
{
    int params = symbol.parameters().count();

    MiscFunctionPrivate* function = m_engine->newFunction( symbol.type(), params, m_opsSize );

    for ( int i = 0; i < params; i++ )
        function->m_paramTypes[ i ] = symbol.parameters().at( i ).type();

    function->m_stackSize = m_stackSize;

    memcpy( function->m_opsTable, m_opsTable, m_opsSize );

    return function;
}

bool MiscCompiler::isValid()
{
    return m_engine->errorCode() == Misc::NoError;
}

static const float const_values[ MiscEnginePrivate::Constants ] =
{
    3.141592f, // PI
    2.718281f  // E
};

void MiscCompiler::visit( Misc::AST::IdNode* node )
{
    int id = node->m_id->m_index;

    if ( id >= MiscEnginePrivate::ConstantFirst && id <= MiscEnginePrivate::ConstantLast ) {
        append( Misc::OP::push_float, const_values[ id - MiscEnginePrivate::ConstantFirst ] );
        push( Misc::FloatType );
        return;
    }

    if ( id < MiscEnginePrivate::Reserved ) {
        m_engine->setError( Misc::SyntaxError, QString( "illegal use of reserved identifier '%1'" ).arg( node->m_id->name() ), node->m_line );
        return;
    }

    for ( MiscScope* scope = m_scope; scope != NULL; scope = scope->parent() ) {
        int index = scope->indexOf( node->m_id );

        if ( index >= 0 ) {
            MiscSymbol symbol = scope->symbolAt( index );

            Q_ASSERT( symbol.type() == m_stack[ index ] );

            appendLoadLocal( symbol.type(), index );
            return;
        }
    }

    for ( MiscUnitPrivate* unit = m_unit; unit != NULL; unit = unit->parent() ) {
        int index = unit->indexOf( node->m_id );

        if ( index >= 0 ) {
            MiscSymbol symbol = unit->symbolAt( index );

            if ( symbol.flags() & Misc::FunctionSymbol ) {
                m_engine->setError( Misc::SyntaxError, QString( "illegal use of function '%1'" ).arg( node->m_id->name() ), node->m_line );
                return;
            }

            int slot = unit->slotAt( index );

            appendLoadGlobal( symbol.type(), slot );
            return;
        }
    }

    m_engine->setError( Misc::SyntaxError, QString( "unknown identifier '%1'" ).arg( node->m_id->name() ), node->m_line );
}

void MiscCompiler::visit( Misc::AST::SubscriptNode* node )
{
    int id = node->m_id->m_index;

    if ( id < MiscEnginePrivate::Reserved ) {
        m_engine->setError( Misc::SyntaxError, QString( "illegal use of reserved identifier '%1'" ).arg( node->m_id->name() ), node->m_line );
        return;
    }

    for ( MiscScope* scope = m_scope; scope != NULL; scope = scope->parent() ) {
        int index = scope->indexOf( node->m_id );

        if ( index >= 0 ) {
            MiscSymbol symbol = scope->symbolAt( index );

            Q_ASSERT( symbol.type() == m_stack[ index ] );

            int comp = componentIndex( symbol.type(), node->m_component, node->m_line );

            if ( comp < 0 )
                return;

            appendLoadLocal( symbol.type(), index, comp );
            return;
        }
    }

    for ( MiscUnitPrivate* unit = m_unit; unit != NULL; unit = unit->parent() ) {
        int index = unit->indexOf( node->m_id );

        if ( index >= 0 ) {
            MiscSymbol symbol = unit->symbolAt( index );

            if ( symbol.flags() & Misc::FunctionSymbol ) {
                m_engine->setError( Misc::SyntaxError, QString( "illegal use of function '%1'" ).arg( node->m_id->name() ), node->m_line );
                return;
            }

            int comp = componentIndex( symbol.type(), node->m_component, node->m_line );

            if ( comp < 0 )
                return;

            int slot = unit->slotAt( index );

            appendLoadGlobal( symbol.type(), slot, comp );
            return;
        }
    }

    m_engine->setError( Misc::SyntaxError, QString( "unknown identifier '%1'" ).arg( node->m_id->name() ), node->m_line );
}

int MiscCompiler::componentIndex( Misc::Type type, MiscIdentifierPrivate* component, int lineNumber /*= 0*/ )
{
    int index = component->m_index;

    if ( type == Misc::VectorType && index >= MiscEnginePrivate::SubscriptFirst && index < MiscEnginePrivate::SubscriptFirst + 4 )
        return index - MiscEnginePrivate::SubscriptFirst;

    if ( type == Misc::MatrixType && index >= MiscEnginePrivate::SubscriptFirst + 4 && index < MiscEnginePrivate::SubscriptFirst + 20 )
        return index - MiscEnginePrivate::SubscriptFirst - 4;

    m_engine->setError( Misc::SyntaxError, QString( "invalid component '%1'" ).arg( component->name() ), lineNumber );
    return -1;
}

void MiscCompiler::visit( Misc::AST::BooleanNode* node )
{
    append( node->m_bool ? Misc::OP::push_true : Misc::OP::push_false );
    push( Misc::BoolType );
}

void MiscCompiler::visit( Misc::AST::FloatNode* node )
{
    if ( node->m_float == 0.0f )
        append( Misc::OP::push_zerof );
    else
        append( Misc::OP::push_float, node->m_float );
    push( Misc::FloatType );
}

bool MiscCompiler::enter( Misc::AST::VectorNode* node )
{
    int count = node->m_list->m_count ;

    if ( count < 2 || count > 4 ) {
        m_engine->setError( Misc::TypeError, "invalid number of vector initializers" );
        return false;
    }

    return true;
}

void MiscCompiler::leave( Misc::AST::VectorNode* node )
{
    int count = node->m_list->m_count ;

    for ( int i = 0; i < count; i++ ) {
        if ( m_stack[ m_stackTop + i - count ] != Misc::FloatType ) {
            m_engine->setError( Misc::TypeError, "invalid initializer type", node->m_line );
            return;
        }
    }

    switch ( count ) {
        case 2:
            append( Misc::OP::make_vec2 );
            break;
        case 3:
            append( Misc::OP::make_vec3 );
            break;
        case 4:
            append( Misc::OP::make_vec4 );
            break;
        default:
            Q_ASSERT( false );
            break;
    }

    pop( count );
    push( Misc::VectorType );
}

bool MiscCompiler::enter( Misc::AST::MatrixNode* node )
{
    int count = node->m_list->m_count ;

    if ( count < 2 || count > 4 ) {
        m_engine->setError( Misc::TypeError, "invalid number of matrix initializers" );
        return false;
    }

    Misc::AST::ListNode* listNode = node->m_list;

    for ( Misc::AST::Node* childNode = listNode->m_head; childNode != NULL; childNode = childNode->m_next ) {
        Misc::AST::ListNode* rowNode = static_cast<Misc::AST::ListNode*>( childNode );

        if ( rowNode->m_count != count ) {
            m_engine->setError( Misc::TypeError, "invalid number of matrix initializers" );
            return false;
        }
    }

    return true;
}

void MiscCompiler::leave( Misc::AST::MatrixNode* node )
{
    int count = node->m_list->m_count * node->m_list->m_count;

    for ( int i = 0; i < count; i++ ) {
        if ( m_stack[ m_stackTop + i - count ] != Misc::FloatType ) {
            m_engine->setError( Misc::TypeError, "invalid initializer type", node->m_line );
            return;
        }
    }

    switch ( count ) {
        case 4:
            append( Misc::OP::make_mat2 );
            break;
        case 9:
            append( Misc::OP::make_mat3 );
            break;
        case 16:
            append( Misc::OP::make_mat4 );
            break;
        default:
            Q_ASSERT( false );
            break;
    }

    pop( count );
    push( Misc::MatrixType );
}

static const char* const operator_names[ Misc::AST::OperatorsCount ] =
{
    "&", "&&", "/", "=", "==", ">", ">=", "<", "<=",
    "-", "%", "*", "!", "!=", "|", "||", "+", "^"
};

const char* Misc::AST::operatorName( Operator op )
{
    return operator_names[ op ];
}

struct UnaryOp
{
    Misc::OP::Code m_code;
    Misc::AST::Operator m_op;
    Misc::Type m_type;
};

#define UNARY_OP( code, op, type ) \
    { Misc::OP::op_##code, Misc::AST::op, Misc::type##Type }

static const UnaryOp unary_ops[] =
{
    UNARY_OP( nop,  Plus,  Float  ),
    UNARY_OP( nop,  Plus,  Vector ),
    UNARY_OP( nop,  Plus,  Matrix ),
    UNARY_OP( negf, Minus, Float  ),
    UNARY_OP( negv, Minus, Vector ),
    UNARY_OP( negm, Minus, Matrix ),
    UNARY_OP( notb, Not,   Bool   )
};

bool MiscCompiler::enter( Misc::AST::UnaryNode* node )
{
    // optimize handling negative numbers
    if ( node->m_op == Misc::AST::Minus && node->m_arg->m_kind == Misc::AST::Float ) {
        Misc::AST::FloatNode* floatNode = static_cast<Misc::AST::FloatNode*>( node->m_arg );
        append( Misc::OP::push_float, -floatNode->m_float );
        push( Misc::FloatType );
        return false;
    }

    return true;
}

void MiscCompiler::leave( Misc::AST::UnaryNode* node )
{
    for ( int i = 0; i < (int)( sizeof( unary_ops ) / sizeof( unary_ops[ 0 ] ) ); i++ ) {
        if ( unary_ops[ i ].m_op == node->m_op && unary_ops[ i ].m_type == m_stack[ m_stackTop - 1 ] ) {
            if ( unary_ops[ i ].m_code != Misc::OP::op_nop )
                append( unary_ops[ i ].m_code );
            return;
        }
    }

    m_engine->setError( Misc::TypeError, QString( "invalid operand types for operator '%1'" ).arg( operator_names[ node->m_op ] ), node->m_line );
}

struct BinaryOp
{
    Misc::OP::Code m_code;
    Misc::AST::Operator m_op;
    Misc::Type m_arg1;
    Misc::Type m_arg2;
    Misc::Type m_result;
};

#define BINARY_OP( code, op, arg1, arg2, result ) \
    { Misc::OP::op_##code, Misc::AST::op, Misc::arg1##Type, Misc::arg2##Type, Misc::result##Type }

static const BinaryOp binary_ops[] =
{
    BINARY_OP( addff, Plus,     Float,  Float,  Float  ),
    BINARY_OP( addvv, Plus,     Vector, Vector, Vector ),
    BINARY_OP( addmm, Plus,     Matrix, Matrix, Matrix ),
    BINARY_OP( subff, Minus,    Float,  Float,  Float  ),
    BINARY_OP( subvv, Minus,    Vector, Vector, Vector ),
    BINARY_OP( submm, Minus,    Matrix, Matrix, Matrix ),
    BINARY_OP( mulff, Multiply, Float,  Float,  Float  ),
    BINARY_OP( mulfv, Multiply, Float,  Vector, Vector ),
    BINARY_OP( mulvf, Multiply, Vector, Float,  Vector ),
    BINARY_OP( mulvv, Multiply, Vector, Vector, Vector ),
    BINARY_OP( mulfm, Multiply, Float,  Matrix, Matrix ),
    BINARY_OP( mulmf, Multiply, Matrix, Float,  Matrix ),
    BINARY_OP( mulvm, Multiply, Vector, Matrix, Vector ),
    BINARY_OP( mulmv, Multiply, Matrix, Vector, Vector ),
    BINARY_OP( mulmm, Multiply, Matrix, Matrix, Matrix ),
    BINARY_OP( divff, Divide,   Float,  Float,  Float  ),
    BINARY_OP( divvf, Divide,   Vector, Float,  Vector ),
    BINARY_OP( divmf, Divide,   Matrix, Float,  Matrix ),
    BINARY_OP( modff, Modulo,   Float,  Float,  Float  ),
    BINARY_OP( powff, Power,    Float,  Float,  Float  ),
    BINARY_OP( eqbb,  EqEq,     Bool,   Bool,   Bool   ),
    BINARY_OP( eqff,  EqEq,     Float,  Float,  Bool   ),
    BINARY_OP( eqvv,  EqEq,     Vector, Vector, Bool   ),
    BINARY_OP( eqmm,  EqEq,     Matrix, Matrix, Bool   ),
    BINARY_OP( neqbb, NotEq,    Bool,   Bool,   Bool   ),
    BINARY_OP( neqff, NotEq,    Float,  Float,  Bool   ),
    BINARY_OP( neqvv, NotEq,    Vector, Vector, Bool   ),
    BINARY_OP( neqmm, NotEq,    Matrix, Matrix, Bool   ),
    BINARY_OP( ltff,  Lt,       Float,  Float,  Bool   ),
    BINARY_OP( lteff, LtEq,     Float,  Float,  Bool   ),
    BINARY_OP( gtff,  Gt,       Float,  Float,  Bool   ),
    BINARY_OP( gteff, GtEq,     Float,  Float,  Bool   ),
    BINARY_OP( andbb, And,      Bool,   Bool,   Bool   ),
    BINARY_OP( orbb,  Or,       Bool,   Bool,   Bool   )
};

bool MiscCompiler::enter( Misc::AST::BinaryNode* /*node*/ )
{
    return true;
}

void MiscCompiler::leave( Misc::AST::BinaryNode* node )
{
    binaryOperator( node->m_op, node->m_line );
}

bool MiscCompiler::binaryOperator( Misc::AST::Operator op, int lineNumber /*= 0*/ )
{
    Misc::Type arg1 = m_stack[ m_stackTop - 2 ];
    Misc::Type arg2 = m_stack[ m_stackTop - 1 ];

    for ( int i = 0; i < (int)( sizeof( binary_ops ) / sizeof( binary_ops[ 0 ] ) ); i++ ) {
        if ( binary_ops[ i ].m_op == op && binary_ops[ i ].m_arg1 == arg1 && binary_ops[ i ].m_arg2 == arg2 ) {
            append( binary_ops[ i ].m_code );
            pop( 2 );
            push( binary_ops[ i ].m_result );
            return true;
        }
    }

    m_engine->setError( Misc::TypeError, QString( "invalid operand types for operator '%1'" ).arg( operator_names[ op ] ), lineNumber );
    return false;
}

bool MiscCompiler::enter( Misc::AST::LogicalNode* node )
{
    node->m_arg1->accept( this );

    if ( !isValid() )
        return false;

    if ( m_stack[ m_stackTop - 1 ] != Misc::BoolType ) {
        m_engine->setError( Misc::TypeError, QString( "invalid operand types for operator '%1'" ).arg( operator_names[ node->m_op ] ), node->m_line );
        return false;
    }

    size_t jmpPos = m_opsSize;

    switch ( node->m_op ) {
        case Misc::AST::AndAnd:
            append( Misc::OP::jmp_and, 0 );
            break;
        case Misc::AST::OrOr:
            append( Misc::OP::jmp_or, 0 );
            break;
        default:
            Q_ASSERT( false );
            break;
    }

    pop();

    node->m_arg2->accept( this );

    if ( !isValid() )
        return false;

    if ( m_stack[ m_stackTop - 1 ] != Misc::BoolType ) {
        m_engine->setError( Misc::TypeError, QString( "invalid operand types for operator '%1'" ).arg( operator_names[ node->m_op ] ), node->m_line );
        return false;
    }

    adjustJump( jmpPos, m_opsSize );

    return false;
}

bool MiscCompiler::enter( Misc::AST::ConditionalNode* node )
{
    node->m_condition->accept( this );

    if ( !isValid() )
        return false;

    if ( m_stack[ m_stackTop - 1 ] != Misc::BoolType ) {
        m_engine->setError( Misc::TypeError, QString( "invalid condition type" ), node->m_line );
        return false;
    }

    size_t jmpElsePos = m_opsSize;

    append( Misc::OP::jmp_false, 0 );
    pop();

    node->m_exp1->accept( this );

    if ( !isValid() )
        return false;

    Misc::Type type = m_stack[ m_stackTop - 1 ];

    size_t jmpEndPos = m_opsSize;

    append( Misc::OP::jmp, 0 );
    pop();

    adjustJump( jmpElsePos, m_opsSize );

    node->m_exp2->accept( this );

    if ( !isValid() )
        return false;

    if ( m_stack[ m_stackTop - 1 ] != type ) {
        m_engine->setError( Misc::TypeError, QString( "invalid operand types for conditional operator" ), node->m_line );
        return false;
    }

    adjustJump( jmpEndPos, m_opsSize );

    return false;
}

struct BuiltInFn
{
    Misc::OP::Code m_code;
    int m_argsCnt;
    Misc::Type m_args[ 3 ];
    Misc::Type m_result;
};

#define BUILTIN_FN0( code, result ) \
    { Misc::OP::fn_##code, 0, { Misc::InvalidType, Misc::InvalidType, Misc::InvalidType }, Misc::result##Type }
#define BUILTIN_FN1( code, arg1, result ) \
    { Misc::OP::fn_##code, 1, { Misc::arg1##Type, Misc::InvalidType, Misc::InvalidType }, Misc::result##Type }
#define BUILTIN_FN2( code, arg1, arg2, result ) \
    { Misc::OP::fn_##code, 2, { Misc::arg1##Type, Misc::arg2##Type, Misc::InvalidType }, Misc::result##Type }
#define BUILTIN_FN3( code, arg1, arg2, arg3, result ) \
    { Misc::OP::fn_##code, 3, { Misc::arg1##Type, Misc::arg2##Type, Misc::arg3##Type }, Misc::result##Type }

static const BuiltInFn builtin_fns[ MiscEnginePrivate::Functions ] =
{
    BUILTIN_FN1( sin,         Float,  Float  ),
    BUILTIN_FN1( cos,         Float,  Float  ),
    BUILTIN_FN1( tan,         Float,  Float  ),
    BUILTIN_FN1( asin,        Float,  Float  ),
    BUILTIN_FN1( acos,        Float,  Float  ),
    BUILTIN_FN1( atan,        Float,  Float  ),
    BUILTIN_FN2( atan2,       Float,  Float,  Float  ),
    BUILTIN_FN1( sinh,        Float,  Float  ),
    BUILTIN_FN1( cosh,        Float,  Float  ),
    BUILTIN_FN1( tanh,        Float,  Float  ),
    BUILTIN_FN1( asinh,       Float,  Float  ),
    BUILTIN_FN1( acosh,       Float,  Float  ),
    BUILTIN_FN1( atanh,       Float,  Float  ),
    BUILTIN_FN1( exp,         Float,  Float  ),
    BUILTIN_FN1( log,         Float,  Float  ),
    BUILTIN_FN1( sqrt,        Float,  Float  ),
    BUILTIN_FN1( abs,         Float,  Float  ),
    BUILTIN_FN1( sign,        Float,  Float  ),
    BUILTIN_FN1( floor,       Float,  Float  ),
    BUILTIN_FN1( round,       Float,  Float  ),
    BUILTIN_FN1( ceil,        Float,  Float  ),
    BUILTIN_FN2( min,         Float,  Float,  Float  ),
    BUILTIN_FN2( max,         Float,  Float,  Float  ),
    BUILTIN_FN3( clamp,       Float,  Float,  Float,  Float  ),
    BUILTIN_FN3( lerp,        Float,  Float,  Float,  Float  ),
    BUILTIN_FN1( isnan,       Float,  Bool   ),
    BUILTIN_FN1( isinf,       Float,  Bool   ),
    BUILTIN_FN1( length,      Vector, Float  ),
    BUILTIN_FN2( distance,    Vector, Vector, Float  ),
    BUILTIN_FN2( dot,         Vector, Vector, Float  ),
    BUILTIN_FN2( cross,       Vector, Vector, Vector ),
    BUILTIN_FN1( normalize,   Vector, Vector ),
    BUILTIN_FN3( lerpv,       Vector, Vector, Float,  Vector ),
    BUILTIN_FN0( identity,    Matrix ),
    BUILTIN_FN1( translate,   Vector, Matrix ),
    BUILTIN_FN1( scale,       Vector, Matrix ),
    BUILTIN_FN2( rotate,      Float,  Vector, Matrix ),
    BUILTIN_FN1( transpose,   Matrix, Matrix ),
    BUILTIN_FN1( determinant, Matrix, Matrix ),
    BUILTIN_FN1( inverse,     Matrix, Matrix )
};

bool MiscCompiler::enter( Misc::AST::CallNode* /*node*/ )
{
    return true;
}

void MiscCompiler::leave( Misc::AST::CallNode* node )
{
    int paramsCount = node->m_args ? node->m_args->m_count : 0;

    int id = node->m_id->m_index;

    if ( id >= MiscEnginePrivate::FunctionFirst && id <= MiscEnginePrivate::FunctionLast ) {
        int index = id - MiscEnginePrivate::FunctionFirst;

        if ( builtin_fns[ index ].m_argsCnt != paramsCount ) {
            m_engine->setError( Misc::SyntaxError, QString( "invalid number of parameters for function '%1'" ).arg( node->m_id->name() ), node->m_line );
            return;
        }

        for ( int i = 0; i < paramsCount; i++ ) {
            if ( m_stack[ m_stackTop + i - paramsCount ] != builtin_fns[ index ].m_args[ i ] ) {
                m_engine->setError( Misc::TypeError, QString( "invalid parameter types for function '%1'" ).arg( node->m_id->name() ), node->m_line );
                return;
            }
        }

        append( builtin_fns[ index ].m_code );

        pop( paramsCount );
        push( builtin_fns[ index ].m_result );

        return;
    }

    if ( id < MiscEnginePrivate::Reserved ) {
        m_engine->setError( Misc::SyntaxError, QString( "illegal use of reserved identifier '%1'" ).arg( node->m_id->name() ), node->m_line );
        return;
    }

    for ( MiscScope* scope = m_scope; scope != NULL; scope = scope->parent() ) {
        int index = scope->indexOf( node->m_id );

        if ( index >= 0 ) {
            m_engine->setError( Misc::SyntaxError, QString( "illegal use of variable '%1'" ).arg( node->m_id->name() ), node->m_line );
            return;
        }
    }

    for ( MiscUnitPrivate* unit = m_unit; unit != NULL; unit = unit->parent() ) {
        int index = unit->indexOf( node->m_id );

        if ( index >= 0 ) {
            MiscSymbol symbol = unit->symbolAt( index );

            if ( !( symbol.flags() & Misc::FunctionSymbol ) ) {
                m_engine->setError( Misc::SyntaxError, QString( "illegal use of variable '%1'" ).arg( node->m_id->name() ), node->m_line );
                return;
            }

            if ( symbol.parameters().count() != paramsCount ) {
                m_engine->setError( Misc::SyntaxError, QString( "invalid number of parameters for function '%1'" ).arg( node->m_id->name() ), node->m_line );
                return;
            }

            for ( int i = 0; i < paramsCount; i++ ) {
                if ( m_stack[ m_stackTop + i - paramsCount ] != symbol.parameters().at( i ).type() ) {
                    m_engine->setError( Misc::TypeError, QString( "invalid parameter types for function '%1'" ).arg( node->m_id->name() ), node->m_line );
                    return;
                }
            }

            int slot = unit->slotAt( index );

            append( Misc::OP::call, slot );

            pop( paramsCount );
            push( symbol.type() );

            return;
        }
    }

    m_engine->setError( Misc::SyntaxError, QString( "unknown identifier '%1'" ).arg( node->m_id->name() ), node->m_line );
}

bool MiscCompiler::enter( Misc::AST::AssignmentNode* node )
{
    MiscIdentifierPrivate* identifier = NULL;
    MiscIdentifierPrivate* component = NULL;

    if ( node->m_lvalue->m_kind == Misc::AST::Id ) {
        Misc::AST::IdNode* idNode = static_cast<Misc::AST::IdNode*>( node->m_lvalue );
        identifier = idNode->m_id;
    } else if ( node->m_lvalue->m_kind == Misc::AST::Subscript ) {
        Misc::AST::SubscriptNode* subscriptNode = static_cast<Misc::AST::SubscriptNode*>( node->m_lvalue );
        identifier = subscriptNode->m_id;
        component = subscriptNode->m_component;
    } else {
        Q_ASSERT( false );
    }

    if ( identifier->m_index < MiscEnginePrivate::Reserved ) {
        m_engine->setError( Misc::SyntaxError, QString( "illegal use of reserved identifier '%1'" ).arg( identifier->name() ), node->m_line );
        return false;
    }

    for ( MiscScope* scope = m_scope; scope != NULL; scope = scope->parent() ) {
        int index = scope->indexOf( identifier );

        if ( index >= 0 ) {
            MiscSymbol symbol = scope->symbolAt( index );

            if ( symbol.flags() & Misc::ConstantSymbol ) {
                m_engine->setError( Misc::SyntaxError, QString( "illegal assignment to constant '%1'" ).arg( identifier->name() ), node->m_line );
                return false;
            }

            Q_ASSERT( symbol.type() == m_stack[ index ] );

            int comp = -1;

            if ( component != NULL ) {
                comp = componentIndex( symbol.type(), component, node->m_lvalue->m_line );

                if ( comp < 0 )
                    return false;
            }

            if ( node->m_op != Misc::AST::Eq ) {
                if ( component == NULL )
                    appendLoadLocal( symbol.type(), index );
                else
                    appendLoadLocal( symbol.type(), index, comp );
            }

            node->m_exp->accept( this );

            if ( !isValid() )
                return false;

            if ( node->m_op != Misc::AST::Eq ) {
                if ( !binaryOperator( node->m_op, node->m_line ) )
                    return false;
            }

            if ( component == NULL ) {
                if ( m_stack[ m_stackTop - 1 ] != symbol.type() ) {
                    m_engine->setError( Misc::TypeError, "type mismatch", node->m_line );
                    return false;
                }

                appendStoreLocal( symbol.type(), index );
            } else {
                if ( m_stack[ m_stackTop - 1 ] != Misc::FloatType ) {
                    m_engine->setError( Misc::TypeError, "type mismatch", node->m_line );
                    return false;
                }

                appendStoreLocal( symbol.type(), index, comp );
            }

            return false;
        }
    }

    for ( MiscUnitPrivate* unit = m_unit; unit != NULL; unit = unit->parent() ) {
        int index = unit->indexOf( identifier );

        if ( index >= 0 ) {
            if ( unit != m_unit ) {
                m_engine->setError( Misc::SyntaxError, QString( "illegal assignment to variable '%1' declared in parent unit" ).arg( identifier->name() ), node->m_line );
                return false;
            }

            MiscSymbol symbol = unit->symbolAt( index );

            if ( symbol.flags() & Misc::FunctionSymbol ) {
                m_engine->setError( Misc::SyntaxError, QString( "illegal use of function '%1'" ).arg( identifier->name() ), node->m_line );
                return false;
            }

            if ( symbol.flags() & Misc::ConstantSymbol ) {
                m_engine->setError( Misc::SyntaxError, QString( "illegal assignment to constant '%1'" ).arg( identifier->name() ), node->m_line );
                return false;
            }

            int comp = -1;

            if ( component != NULL ) {
                comp = componentIndex( symbol.type(), component, node->m_lvalue->m_line );

                if ( comp < 0 )
                    return false;
            }

            int slot = unit->slotAt( index );

            if ( node->m_op != Misc::AST::Eq ) {
                if ( component == NULL )
                    appendLoadGlobal( symbol.type(), slot );
                else
                    appendLoadGlobal( symbol.type(), slot, comp );
            }

            node->m_exp->accept( this );

            if ( !isValid() )
                return false;

            if ( node->m_op != Misc::AST::Eq ) {
                if ( !binaryOperator( node->m_op, node->m_line ) )
                    return false;
            }

            if ( component == NULL ) {
                if ( m_stack[ m_stackTop - 1 ] != symbol.type() ) {
                    m_engine->setError( Misc::TypeError, "type mismatch", node->m_line );
                    return false;
                }

                appendStoreGlobal( symbol.type(), slot );
            } else {
                if ( m_stack[ m_stackTop - 1 ] != Misc::FloatType ) {
                    m_engine->setError( Misc::TypeError, "type mismatch", node->m_line );
                    return false;
                }

                appendStoreGlobal( symbol.type(), slot, comp );
            }

            return false;
        }
    }

    m_engine->setError( Misc::SyntaxError, QString( "unknown identifier '%1'" ).arg( identifier->name() ), node->m_line );
    return false;
}

bool MiscCompiler::enter( Misc::AST::VoidNode* /*node*/ )
{
    return true;
}

void MiscCompiler::leave( Misc::AST::VoidNode* /*node*/ )
{
    append( Misc::OP::pop );
    pop();
}

bool MiscCompiler::enter( Misc::AST::IfNode* node )
{
    node->m_condition->accept( this );

    if ( !isValid() )
        return false;

    if ( m_stack[ m_stackTop - 1 ] != Misc::BoolType ) {
        m_engine->setError( Misc::TypeError, QString( "invalid condition type" ), node->m_line );
        return false;
    }

    size_t jmpElsePos = m_opsSize;

    append( Misc::OP::jmp_false, 0 );
    pop();

    node->m_stat1->accept( this );

    if ( !isValid() )
        return false;

    if ( node->m_stat2 != NULL ) {
        size_t jmpEndPos = m_opsSize;

        append( Misc::OP::jmp, 0 );

        adjustJump( jmpElsePos, m_opsSize );

        node->m_stat2->accept( this );

        if ( !isValid() )
            return false;

        adjustJump( jmpEndPos, m_opsSize );
    } else {
        adjustJump( jmpElsePos, m_opsSize );
    }

    return false;
}

bool MiscCompiler::enter( Misc::AST::WhileNode* node )
{
    size_t continuePos = m_opsSize;

    node->m_condition->accept( this );

    if ( !isValid() )
        return false;

    if ( m_stack[ m_stackTop - 1 ] != Misc::BoolType ) {
        m_engine->setError( Misc::TypeError, QString( "invalid condition type" ), node->m_line );
        return false;
    }

    size_t jmpBreakPos = m_opsSize;

    append( Misc::OP::jmp_false, 0 );
    pop();

    MiscScope scope( m_engine, m_scope );
    m_scope = &scope;

    scope.setLoopMode( true );

    node->m_stat->accept( this );

    if ( !isValid() )
        return false;

    m_scope = scope.parent();

    append( Misc::OP::jmp, (int)continuePos );

    adjustJump( jmpBreakPos, m_opsSize );

    adjustLoop( &scope, continuePos, m_opsSize );

    return false;
}

bool MiscCompiler::enter( Misc::AST::DoWhileNode* node )
{
    size_t startPos = m_opsSize;

    MiscScope scope( m_engine, m_scope );
    m_scope = &scope;

    scope.setLoopMode( true );

    node->m_stat->accept( this );

    if ( !isValid() )
        return false;

    m_scope = scope.parent();

    size_t continuePos = m_opsSize;

    node->m_condition->accept( this );

    if ( !isValid() )
        return false;

    if ( m_stack[ m_stackTop - 1 ] != Misc::BoolType ) {
        m_engine->setError( Misc::TypeError, QString( "invalid condition type" ), node->m_line );
        return false;
    }

    append( Misc::OP::jmp_true, (int)startPos );
    pop();

    adjustLoop( &scope, continuePos, m_opsSize );

    return false;
}

bool MiscCompiler::enter( Misc::AST::ForNode* node )
{
    MiscScope scope( m_engine, m_scope );
    m_scope = &scope;

    scope.setLoopMode( true );

    node->m_stat1->accept( this );

    if ( !isValid() )
        return false;

    size_t startPos = m_opsSize;

    size_t jmpBreakPos = -1;

    if ( node->m_condition != NULL ) {
        node->m_condition->accept( this );

        if ( !isValid() )
            return false;

        if ( m_stack[ m_stackTop - 1 ] != Misc::BoolType ) {
            m_engine->setError( Misc::TypeError, QString( "invalid condition type" ), node->m_line );
            return false;
        }

        jmpBreakPos = m_opsSize;

        append( Misc::OP::jmp_false, 0 );
        pop();
    }

    node->m_stat3->accept( this );

    if ( !isValid() )
        return false;

    size_t continuePos = m_opsSize;

    node->m_stat2->accept( this );

    if ( !isValid() )
        return false;

    append( Misc::OP::jmp, (int)startPos );

    if ( node->m_condition != NULL )
        adjustJump( jmpBreakPos, m_opsSize );

    adjustLoop( &scope, continuePos, m_opsSize );

    appendPop( scope.count() );

    Q_ASSERT( m_stackTop == scope.offset() );

    m_scope = scope.parent();

    return false;
}

bool MiscCompiler::enter( Misc::AST::ReturnNode* node )
{
    int count = m_scope->count() + m_scope->offset();

    Q_ASSERT( m_stackTop == count );

    for ( MiscScope* scope = m_scope; scope != NULL; scope = scope->parent() ) {
        if ( scope->function().isValid() ) {
            Misc::Type type = scope->function().type();

            if ( node->m_exp != NULL ) {
                if ( type == Misc::VoidType ) {
                    m_engine->setError( Misc::SyntaxError, "unexpected return value", node->m_exp->m_line );
                    return false;
                }

                node->m_exp->accept( this );

                if ( !isValid() )
                    return false;

                if ( m_stack[ m_stackTop - 1 ] != type ) {
                    m_engine->setError( Misc::TypeError, "invalid return type", node->m_line );
                    return false;
                }
            } else {
                if ( type != Misc::VoidType ) {
                    m_engine->setError( Misc::SyntaxError, "missing return value", node->m_line );
                    return false;
                }

                append( Misc::OP::push_void );
                push( Misc::VoidType );
            }

            if ( count == 0 )
                append( Misc::OP::ret );
            else
                append( Misc::OP::ret_n, count );
            pop();

            return false;
        }
    }

    m_engine->setError( Misc::SyntaxError, "unexpected 'return'", node->m_line );
    return false;
}

void MiscCompiler::visit( Misc::AST::BreakNode* node )
{
    int count = 0;

    for ( MiscScope* scope = m_scope; scope != NULL; scope = scope->parent() ) {
        if ( scope->isLoopMode() ) {
            appendPop( count );

            scope->addBreak( m_opsSize );

            append( Misc::OP::jmp, 0 );

            return;
        } else {
            count += scope->count();
        }
    }

    m_engine->setError( Misc::SyntaxError, "unexpected 'break'", node->m_line );
}

void MiscCompiler::visit( Misc::AST::ContinueNode* node )
{
    int count = 0;

    for ( MiscScope* scope = m_scope; scope != NULL; scope = scope->parent() ) {
        if ( scope->isLoopMode() ) {
            appendPop( count );

            scope->addContinue( m_opsSize );

            append( Misc::OP::jmp, 0 );

            return;
        } else {
            count += scope->count();
        }
    }

    m_engine->setError( Misc::SyntaxError, "unexpected 'continue'", node->m_line );
}

bool MiscCompiler::enter( Misc::AST::BlockNode* node )
{
    if ( node->m_list == NULL )
        return false;

    MiscScope scope( m_engine, m_scope );
    m_scope = &scope;

    node->m_list->accept( this );

    if ( !isValid() )
        return false;

    appendPop( scope.count() );

    Q_ASSERT( m_stackTop == scope.offset() );

    m_scope = scope.parent();

    return false;
}

bool MiscCompiler::enter( Misc::AST::DeclarationNode* node )
{
    Misc::AST::ListNode* listNode = node->m_vars;

    for ( Misc::AST::Node* childNode = listNode->m_head; childNode != NULL; childNode = childNode->m_next ) {
        Misc::AST::VariableNode* variableNode = static_cast<Misc::AST::VariableNode*>( childNode );

        if ( variableNode->m_exp != NULL ) {
            variableNode->m_exp->accept( this );

            if ( !isValid() )
                return false;

            if ( m_stack[ m_stackTop - 1 ] != node->m_type ) {
                m_engine->setError( Misc::TypeError, "invalid initializer type", variableNode->m_exp->m_line );
                return false;
            }
        } else {
            appendPushDefault( node->m_type );
        }

        MiscSymbol symbol( node->m_type, MiscIdentifierPrivate::toPublic( variableNode->m_id ) );
        symbol.setFlags( node->m_flags );

        if ( m_scope == NULL ) {
            // global variable
            int slot = m_unit->addVariable( symbol, variableNode->m_line );

            if ( slot < 0 )
                return false;

            appendStoreGlobal( symbol.type(), slot );
        } else {
            // local variable
            int index = m_scope->addVariable( symbol, variableNode->m_line );

            if ( index < 0 )
                return false;

            Q_ASSERT( index == m_stackTop - 1 );
        }
    }

    return false;
}

bool MiscCompiler::enter( Misc::AST::FunctionNode* /*node*/ )
{
    // skip functions - they are compiled separately by MiscUnit
    return false;
}

void MiscCompiler::appendPushDefault( Misc::Type type )
{
    switch ( type ) {
        case Misc::VoidType:
            append( Misc::OP::push_void );
            break;
        case Misc::BoolType:
            append( Misc::OP::push_false );
            break;
        case Misc::FloatType:
            append( Misc::OP::push_zerof );
            break;
        case Misc::VectorType:
            append( Misc::OP::push_zerov );
            break;
        case Misc::MatrixType:
            append( Misc::OP::push_zerom );
            break;
        default:
            Q_ASSERT( false );
            break;
    }

    push( type );
}

void MiscCompiler::appendPop( int count )
{
    if ( count == 1 )
        append( Misc::OP::pop );
    else if ( count > 1 )
        append( Misc::OP::pop_n, count );

    pop( count );
}

void MiscCompiler::appendLoadLocal( Misc::Type type, int index )
{
    switch ( type ) {
        case Misc::BoolType:
            append( Misc::OP::load_lvarb, index );
            break;
        case Misc::FloatType:
            append( Misc::OP::load_lvarf, index );
            break;
        case Misc::VectorType:
            append( Misc::OP::load_lvarv, index );
            break;
        case Misc::MatrixType:
            append( Misc::OP::load_lvarm, index );
            break;
        default:
            Q_ASSERT( false );
    }

    push( type );
}

void MiscCompiler::appendLoadGlobal( Misc::Type type, int slot )
{
    switch ( type ) {
        case Misc::BoolType:
            append( Misc::OP::load_gvarb, slot );
            break;
        case Misc::FloatType:
            append( Misc::OP::load_gvarf, slot );
            break;
        case Misc::VectorType:
            append( Misc::OP::load_gvarv, slot );
            break;
        case Misc::MatrixType:
            append( Misc::OP::load_gvarm, slot );
            break;
        default:
            Q_ASSERT( false );
    }

    push( type );
}

void MiscCompiler::appendLoadLocal( Misc::Type type, int index, int comp )
{
    switch ( type ) {
        case Misc::VectorType:
            append( Misc::OP::load_lcompv, index, comp );
            break;
        case Misc::MatrixType:
            append( Misc::OP::load_lcompm, index, comp );
            break;
        default:
            Q_ASSERT( false );
    }

    push( Misc::FloatType );
}

void MiscCompiler::appendLoadGlobal( Misc::Type type, int slot, int comp )
{
    switch ( type ) {
        case Misc::VectorType:
            append( Misc::OP::load_gcompv, slot, comp );
            break;
        case Misc::MatrixType:
            append( Misc::OP::load_gcompm, slot, comp );
            break;
        default:
            Q_ASSERT( false );
    }

    push( Misc::FloatType );
}

void MiscCompiler::appendStoreLocal( Misc::Type type, int index )
{
    switch ( type ) {
        case Misc::BoolType:
            append( Misc::OP::store_lvarb, index );
            break;
        case Misc::FloatType:
            append( Misc::OP::store_lvarf, index );
            break;
        case Misc::VectorType:
            append( Misc::OP::store_lvarv, index );
            break;
        case Misc::MatrixType:
            append( Misc::OP::store_lvarm, index );
            break;
        default:
            Q_ASSERT( false );
    }

    pop();
}

void MiscCompiler::appendStoreGlobal( Misc::Type type, int slot )
{
    switch ( type ) {
        case Misc::BoolType:
            append( Misc::OP::store_gvarb, slot );
            break;
        case Misc::FloatType:
            append( Misc::OP::store_gvarf, slot );
            break;
        case Misc::VectorType:
            append( Misc::OP::store_gvarv, slot );
            break;
        case Misc::MatrixType:
            append( Misc::OP::store_gvarm, slot );
            break;
        default:
            Q_ASSERT( false );
    }

    pop();
}

void MiscCompiler::appendStoreLocal( Misc::Type type, int index, int comp )
{
    switch ( type ) {
        case Misc::VectorType:
            append( Misc::OP::store_lcompv, index, comp );
            break;
        case Misc::MatrixType:
            append( Misc::OP::store_lcompm, index, comp );
            break;
        default:
            Q_ASSERT( false );
    }

    pop();
}

void MiscCompiler::appendStoreGlobal( Misc::Type type, int slot, int comp )
{
    switch ( type ) {
        case Misc::VectorType:
            append( Misc::OP::store_gcompv, slot, comp );
            break;
        case Misc::MatrixType:
            append( Misc::OP::store_gcompm, slot, comp );
            break;
        default:
            Q_ASSERT( false );
    }

    pop();
}


void MiscCompiler::append( Misc::OP::Code code )
{
    Misc::OP::Instruction* instr = growOps( Misc::OP::InstrSize );
    instr->m_code = code;
}

void MiscCompiler::append( Misc::OP::Code code, int arg )
{
    Misc::OP::Instruction* instr = growOps( Misc::OP::InstrArg1Size );
    instr->m_code = code;
    instr->m_arg[ 0 ] = arg;
}

void MiscCompiler::append( Misc::OP::Code code, int arg1, int arg2 )
{
    Misc::OP::Instruction* instr = growOps( Misc::OP::InstrArg2Size );
    instr->m_code = code;
    instr->m_arg[ 0 ] = arg1;
    instr->m_arg[ 1 ] = arg2;
}

void MiscCompiler::append( Misc::OP::Code code, float arg )
{
    Misc::OP::Instruction* instr = growOps( Misc::OP::InstrFloatSize );
    instr->m_code = code;
    instr->m_float = arg;
}

void MiscCompiler::reallocOps()
{
    if ( m_opsCapacity == 0 )
        m_opsCapacity = 4096;
    else
        m_opsCapacity <<= 2;

    m_opsTable = static_cast<char*>( realloc( m_opsTable, m_opsCapacity ) );
}

void MiscCompiler::adjustLoop( MiscScope* scope, size_t continuePos, size_t breakPos )
{
    for ( int i = 0; i < scope->continues().count(); i++ )
        adjustJump( scope->continues().at( i ), continuePos );

    for ( int i = 0; i < scope->breaks().count(); i++ )
        adjustJump( scope->breaks().at( i ), breakPos );
}

void MiscCompiler::adjustJump( size_t pos, size_t target )
{
    Misc::OP::Instruction* instr = reinterpret_cast<Misc::OP::Instruction*>( m_opsTable + pos );
    instr->m_arg[ 0 ] = (int)target;
}

void MiscCompiler::push( Misc::Type type )
{
    if ( m_stackTop + 1 > m_stackCapacity ) {
        if ( m_stackCapacity == 0 )
            m_stackCapacity = 128;
        else
            m_stackCapacity <<= 2;

        m_stack = static_cast<Misc::Type*>( realloc( m_stack, m_stackCapacity * sizeof( Misc::Type ) ) );
    }

    m_stack[ m_stackTop++ ] = type;

    if ( m_stackSize < m_stackTop )
        m_stackSize = m_stackTop;
}
