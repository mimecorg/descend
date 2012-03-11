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

#include "miscsymbolmap_p.h"

#include "miscengine_p.h"

MiscSymbolMap::MiscSymbolMap( MiscEnginePrivate* engine ) :
    m_engine( engine )
{
}

MiscSymbolMap::~MiscSymbolMap()
{
}

MiscSymbol MiscSymbolMap::makeSymbol( Misc::AST::FunctionNode* functionNode )
{
    MiscSymbol symbol( functionNode->m_type, MiscIdentifierPrivate::toPublic( functionNode->m_id ) );
    symbol.setFlags( Misc::FunctionSymbol );

    Misc::AST::ListNode* listNode = functionNode->m_args;

    if ( listNode != NULL ) {
        QList<MiscSymbol::Parameter> parameters;

        parameters.reserve( listNode->m_count );

        for ( Misc::AST::Node* node = listNode->m_head; node != NULL; node = node->m_next ) {
            Misc::AST::ArgumentNode* argNode = static_cast<Misc::AST::ArgumentNode*>( node );

            MiscSymbol::Parameter parameter( argNode->m_type, MiscIdentifierPrivate::toPublic( argNode->m_id ) );
            parameters.append( parameter );
        }

        symbol.setParameters( parameters );
    }

    return symbol;
}

MiscSymbol MiscSymbolMap::makeSymbol( Misc::AST::DeclarationNode* declarationNode, Misc::AST::VariableNode* variableNode )
{
    MiscSymbol symbol( declarationNode->m_type, MiscIdentifierPrivate::toPublic( variableNode->m_id ) );
    symbol.setFlags( declarationNode->m_flags );

    return symbol;
}

bool MiscSymbolMap::checkSymbol( const MiscSymbol& symbol, int lineNumber /*= 0*/ )
{
    if ( !symbol.isValid() || !symbol.identifier().isValid() || ( !( symbol.flags() & Misc::FunctionSymbol ) && !symbol.parameters().isEmpty() ) ) {
        m_engine->setError( Misc::ArgumentError, "invalid symbol", lineNumber );
        return false;
    }

    if ( isReserved( MiscIdentifierPrivate::get( symbol.identifier() ), lineNumber ) )
        return false;

    if ( symbol.flags() & Misc::FunctionSymbol ) {
        QSet<int> used;

        for ( int i = 0; i < symbol.parameters().count(); i++ ) {
            MiscSymbol::Parameter parameter = symbol.parameters().at( i );

            MiscIdentifierPrivate* identifier = MiscIdentifierPrivate::get( parameter.identifier() );

            if ( isReserved( identifier, lineNumber ) )
                return false;

            if ( used.contains( identifier->m_index ) ) {
                m_engine->setError( Misc::SyntaxError, QString( "redefinition of symbol '%1'" ).arg( identifier->name() ), lineNumber );
                return false;
            }

            used.insert( identifier->m_index );

            if ( parameter.type() == Misc::VoidType ) {
                m_engine->setError( Misc::TypeError, "illegal use of type 'void'", lineNumber );
                return false;
            }
        }
    } else {
        if ( symbol.type() == Misc::VoidType ) {
            m_engine->setError( Misc::TypeError, "illegal use of type 'void'", lineNumber );
            return false;
        }
    }

    return true;
}

bool MiscSymbolMap::isReserved( MiscIdentifierPrivate* identifier, int lineNumber /*= 0*/ )
{
    if ( identifier->m_index < MiscEnginePrivate::Reserved ) {
        m_engine->setError( Misc::SyntaxError, QString( "illegal use of reserved identifier '%1'" ).arg( identifier->name() ), lineNumber );
        return true;
    }

    return false;
}

int MiscSymbolMap::addSymbol( const MiscSymbol& symbol, int lineNumber /*= 0*/ )
{
    if ( !checkSymbol( symbol, lineNumber ) )
        return -1;

    MiscIdentifierPrivate* identifier = MiscIdentifierPrivate::get( symbol.identifier() );

    if ( m_map.contains( identifier->m_index ) ) {
        m_engine->setError( Misc::SyntaxError, QString( "redefinition of symbol '%1'" ).arg( identifier->name() ), lineNumber );
        return -1;
    }

    int index = m_symbols.count();

    m_map.insert( identifier->m_index, index );
    m_symbols.append( symbol );

    return index;
}

int MiscSymbolMap::indexOf( MiscIdentifierPrivate* identifier ) const
{
    return m_map.value( identifier->m_index, -1 );
}

MiscSymbol MiscSymbolMap::symbolAt( int index ) const
{
    return m_symbols.at( index );
}
