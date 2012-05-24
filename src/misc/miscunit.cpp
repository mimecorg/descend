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

#include "miscunit.h"
#include "miscunit_p.h"

#include "miscengine_p.h"
#include "misclexer_p.h"
#include "miscparser_p.h"
#include "misccode_p.h"
#include "miscfunction_p.h"
#include "miscsymbol_p.h"
#include "misccompiler_p.h"
#include "miscop_p.h"

#if defined( MISC_DEBUG )
#include "miscdisassembler_p.h"
#include "miscprintvisitor_p.h"
#endif

MiscUnit::MiscUnit( MiscEngine* engine ) : QObject( engine ),
    d( new MiscUnitPrivate( MiscEnginePrivate::get( engine ) ) )
{
}

MiscUnit::MiscUnit( MiscUnit* parent ) : QObject( parent ),
    d( new MiscUnitPrivate( parent->d->m_engine, parent->d ) )
{
}

MiscUnit::~MiscUnit()
{
    delete d;
}

MiscUnitPrivate::MiscUnitPrivate( MiscEnginePrivate* engine, MiscUnitPrivate* parent /*= NULL*/ ) :
    m_parent( parent ),
    m_engine( engine ),
    m_symbolMap( engine )
{
}

MiscUnitPrivate::~MiscUnitPrivate()
{
}

MiscCode MiscUnit::compileUnit( const QString& text )
{
    d->m_engine->setError( Misc::NoError );

    MiscPool pool;
    Misc::AST::Node* rootNode = d->parse( MiscLexer::UnitMode, text, &pool );

    if ( rootNode == NULL )
        return MiscCode();

    Q_ASSERT( rootNode->m_kind == Misc::AST::Unit );

    Misc::AST::UnitNode* unitNode = static_cast<Misc::AST::UnitNode*>( rootNode );
    Misc::AST::ListNode* listNode = unitNode->m_list;

    if ( listNode == NULL ) {
        MiscCodePrivate* code = d->m_engine->newCode( Misc::OP::InstrSize );
        Misc::OP::Instruction* instr = reinterpret_cast<Misc::OP::Instruction*>( code->m_opsTable );
        instr->m_code = Misc::OP::exit;
        return MiscCodePrivate::toPublic( code );
    }

    for ( Misc::AST::Node* node = listNode->m_head; node != NULL; node = node->m_next ) {
        if ( node->m_kind == Misc::AST::Function ) {
            Misc::AST::FunctionNode* functionNode = static_cast<Misc::AST::FunctionNode*>( node );

            MiscSymbol symbol = d->m_symbolMap.makeSymbol( functionNode );

            if ( d->addFunction( symbol, functionNode->m_line ) < 0 )
                return MiscCode();
        }
    }

    MiscCompiler compiler( d );

    MiscCodePrivate* code = compiler.compileUnit( unitNode );

    if ( code == NULL )
        return MiscCode();

#if defined( MISC_DEBUG )
    QTextStream output( stdout );
    MiscDisassembler disassembler( output );

    output << endl << "UNIT:" << endl << endl;
    disassembler.print( code->m_opsTable, code->m_opsSize );
#endif

    for ( Misc::AST::Node* node = listNode->m_head; node != NULL; node = node->m_next ) {
        if ( node->m_kind == Misc::AST::Function ) {
            Misc::AST::FunctionNode* functionNode = static_cast<Misc::AST::FunctionNode*>( node );

            int index = d->m_symbolMap.indexOf( functionNode->m_id );

            MiscFunctionPrivate* function = compiler.compileFunction( d->m_symbolMap.symbolAt( index ), functionNode->m_body );

            if ( function == NULL )
                return MiscCode();

#if defined( MISC_DEBUG )
            output << endl << "FUNCTION " << functionNode->m_id->name() << ":" << endl << endl;
            disassembler.print( function->m_opsTable, function->m_opsSize );
#endif

            d->m_engine->setFunction( d->m_slots.at( index ), function );
        }
    }

    return MiscCodePrivate::toPublic( code );
}

MiscSymbol MiscUnit::parseSymbol( const QString& text )
{
    d->m_engine->setError( Misc::NoError );

    MiscPool pool;
    Misc::AST::Node* rootNode = d->parse( MiscLexer::SymbolMode, text, &pool );

    if ( rootNode == NULL )
        return MiscSymbol();

    if ( rootNode->m_kind == Misc::AST::Declaration ) {
        Misc::AST::DeclarationNode* declarationNode = static_cast<Misc::AST::DeclarationNode*>( rootNode );
        Misc::AST::VariableNode* variableNode = static_cast<Misc::AST::VariableNode*>( declarationNode->m_vars->m_head );

        return d->m_symbolMap.makeSymbol( declarationNode, variableNode );
    }

    if ( rootNode->m_kind == Misc::AST::Function ) {
        Misc::AST::FunctionNode* functionNode = static_cast<Misc::AST::FunctionNode*>( rootNode );

        return d->m_symbolMap.makeSymbol( functionNode );
    }

    Q_ASSERT( false );

    return MiscSymbol();
}

bool MiscUnit::addVariable( const MiscSymbol& symbol )
{
    d->m_engine->setError( Misc::NoError );

    int slot = d->addVariable( symbol );

    if ( slot < 0 )
        return false;

    return true;
}

MiscCode MiscUnit::addVariable( const MiscSymbol& symbol, const QString& text )
{
    d->m_engine->setError( Misc::NoError );

    MiscPool pool;
    Misc::AST::Node* rootNode = d->parse( MiscLexer::ExpressionMode, text, &pool );

    if ( rootNode == NULL )
        return MiscCode();

    int slot = d->addVariable( symbol );

    if ( slot < 0 )
        return MiscCode();

    MiscCompiler compiler( d );

    MiscCodePrivate* code = compiler.compileInitializer( slot, symbol.type(), rootNode );

    if ( code == NULL )
        return MiscCode();

#if defined( MISC_DEBUG )
    QTextStream output( stdout );
    MiscDisassembler disassembler( output );

    output << endl << "INITIALIZER:" << endl << endl;
    disassembler.print( code->m_opsTable, code->m_opsSize );
#endif

    return MiscCodePrivate::toPublic( code );
}

bool MiscUnit::addFunction( const MiscSymbol& symbol )
{
    d->m_engine->setError( Misc::NoError );

    int slot = d->addFunction( symbol );

    if ( slot < 0 )
        return false;

    return true;
}

MiscFunction MiscUnit::addFunction( const MiscSymbol& symbol, const QString& text )
{
    d->m_engine->setError( Misc::NoError );

    MiscPool pool;
    Misc::AST::Node* rootNode = d->parse( MiscLexer::ExpressionMode, text, &pool );

    if ( rootNode == NULL )
        return MiscFunction();

    int slot = d->addFunction( symbol );

    if ( slot < 0 )
        return MiscFunction();

    MiscCompiler compiler( d );

    MiscFunctionPrivate* function = compiler.compileFunction( symbol, rootNode );

    if ( function == NULL )
        return MiscFunction();

#if defined( MISC_DEBUG )
    QTextStream output( stdout );
    MiscDisassembler disassembler( output );

    output << endl << "FUNCTION " << symbol.identifier().name() << ":" << endl << endl;
    disassembler.print( function->m_opsTable, function->m_opsSize );
#endif

    d->m_engine->setFunction( slot, function );

    return MiscFunctionPrivate::toPublic( function );
}

Misc::AST::Node* MiscUnitPrivate::parse( MiscLexer::Mode mode, const QString& text, MiscPool* pool )
{
    MiscLexer lexer( mode, text, m_engine );
    MiscParser parser( pool );

    if ( !parser.parse( &lexer ) ) {
        m_engine->setError( Misc::SyntaxError, parser.errorMessage(), parser.errorLineNumber() );
        return NULL;
    }

#if defined( MISC_DEBUG )
    QTextStream output( stdout );
    MiscPrintVisitor visitor( output );

    output << endl << "AST:" << endl << endl;
    parser.result()->accept( &visitor );
#endif

    return parser.result();
}

int MiscUnitPrivate::addVariable( const MiscSymbol& symbol, int lineNumber /*= 0*/ )
{
    int index = m_symbolMap.addSymbol( symbol, lineNumber );

    if ( index < 0 )
        return -1;

    Q_ASSERT( index == m_slots.count() );

    int slot = m_engine->allocVariable( symbol.type() );
    m_slots.append( slot );

    return slot;
}

int MiscUnitPrivate::addFunction( const MiscSymbol& symbol, int lineNumber /*= 0*/ )
{
    int index = m_symbolMap.addSymbol( symbol, lineNumber );

    if ( index < 0 )
        return -1;

    Q_ASSERT( index == m_slots.count() );

    int slot = m_engine->allocFunction();
    m_slots.append( slot );

    return slot;
}

bool MiscUnit::setVariable( const MiscIdentifier& id, const MiscValue& value )
{
    d->m_engine->setError( Misc::NoError );

    if ( !id.isValid() ) {
        d->m_engine->setError( Misc::ArgumentError, "invalid identifier" );
        return false;
    }

    if ( !value.isValid() ) {
        d->m_engine->setError( Misc::ArgumentError, "invalid value" );
        return false;
    }

    int index = d->m_symbolMap.indexOf( MiscIdentifierPrivate::get( id ) );

    if ( index < 0 ) {
        d->m_engine->setError( Misc::SyntaxError, QString( "unknown identifier '%1'" ).arg( id.name() ) );
        return false;
    }

    MiscSymbol symbol = d->m_symbolMap.symbolAt( index );

    if ( symbol.flags() & Misc::FunctionSymbol ) {
        d->m_engine->setError( Misc::SyntaxError, QString( "illegal use of function '%1'" ).arg( id.name() ) );
        return false;
    }

    if ( symbol.type() != value.type() ) {
        d->m_engine->setError( Misc::TypeError, "type mismatch" );
        return false;
    }

    int slot = d->m_slots.at( index );

    d->m_engine->setVariable( slot, MiscValuePrivate::get( value ) );

    return true;
}

MiscValue MiscUnit::variable( const MiscIdentifier& id ) const
{
    d->m_engine->setError( Misc::NoError );

    if ( !id.isValid() ) {
        d->m_engine->setError( Misc::ArgumentError, "invalid identifier" );
        return MiscValue();
    }

    int index = d->m_symbolMap.indexOf( MiscIdentifierPrivate::get( id ) );

    if ( index < 0 ) {
        d->m_engine->setError( Misc::SyntaxError, QString( "unknown identifier '%1'" ).arg( id.name() ) );
        return MiscValue();
    }

    MiscSymbol symbol = d->m_symbolMap.symbolAt( index );

    if ( symbol.flags() & Misc::FunctionSymbol ) {
        d->m_engine->setError( Misc::SyntaxError, QString( "illegal use of function '%1'" ).arg( id.name() ) );
        return MiscValue();
    }

    int slot = d->m_slots.at( index );

    MiscValuePrivate* value = d->m_engine->variable( slot );

    return MiscValuePrivate::toPublic( value );
}

bool MiscUnit::setFunction( const MiscIdentifier& id, const MiscFunction& function )
{
    d->m_engine->setError( Misc::NoError );

    if ( !id.isValid() ) {
        d->m_engine->setError( Misc::ArgumentError, "invalid identifier" );
        return false;
    }

    if ( !function.isValid() ) {
        d->m_engine->setError( Misc::ArgumentError, "invalid function" );
        return false;
    }

    int index = d->m_symbolMap.indexOf( MiscIdentifierPrivate::get( id ) );

    if ( index < 0 ) {
        d->m_engine->setError( Misc::SyntaxError, QString( "unknown identifier '%1'" ).arg( id.name() ) );
        return false;
    }

    MiscSymbol symbol = d->m_symbolMap.symbolAt( index );

    if ( !(symbol.flags() & Misc::FunctionSymbol ) ) {
        d->m_engine->setError( Misc::SyntaxError, QString( "illegal use of variable '%1'" ).arg( id.name() ) );
        return false;
    }

    MiscFunctionPrivate* data = MiscFunctionPrivate::get( function );

    if ( symbol.type() != data->m_returnType ) {
        d->m_engine->setError( Misc::TypeError, "invalid return type" );
        return false;
    }

    if ( symbol.parameters().count() != data->m_paramsCount ) {
        d->m_engine->setError( Misc::SyntaxError, QString( "invalid number of parameters for function '%1'" ).arg( id.name() ) );
        return false;
    }

    for ( int i = 0; i < data->m_paramsCount; i++ ) {
        if ( symbol.parameters().at( i ).type() != data->m_paramTypes[ i ] ) {
            d->m_engine->setError( Misc::TypeError, QString( "invalid parameter types for function '%1'" ).arg( id.name() ) );
            return false;
        }
    }

    int slot = d->m_slots.at( index );

    d->m_engine->setFunction( slot, data );

    return true;
}

MiscFunction MiscUnit::function( const MiscIdentifier& id ) const
{
    d->m_engine->setError( Misc::NoError );

    if ( !id.isValid() ) {
        d->m_engine->setError( Misc::ArgumentError, "invalid identifier" );
        return MiscFunction();
    }

    int index = d->m_symbolMap.indexOf( MiscIdentifierPrivate::get( id ) );

    if ( index < 0 ) {
        d->m_engine->setError( Misc::SyntaxError, QString( "unknown identifier '%1'" ).arg( id.name() ) );
        return MiscFunction();
    }

    MiscSymbol symbol = d->m_symbolMap.symbolAt( index );

    if ( !(symbol.flags() & Misc::FunctionSymbol ) ) {
        d->m_engine->setError( Misc::SyntaxError, QString( "illegal use of variable '%1'" ).arg( id.name() ) );
        return MiscFunction();
    }

    int slot = d->m_slots.at( index );

    MiscFunctionPrivate* function = d->m_engine->function( slot );

    return MiscFunctionPrivate::toPublic( function );
}

MiscSymbol MiscUnitPrivate::symbolAt( int index ) const
{
    return m_symbolMap.symbolAt( index );
}

int MiscUnitPrivate::slotAt( int index ) const
{
    return m_slots.at( index );
}

int MiscUnitPrivate::indexOf( MiscIdentifierPrivate* identifier ) const
{
    return m_symbolMap.indexOf( identifier );
}
