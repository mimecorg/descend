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

#include "miscscope_p.h"

MiscScope::MiscScope( MiscEnginePrivate* engine, MiscScope* parent /*= NULL*/ ) :
    m_parent( parent ),
    m_engine( engine ),
    m_symbolMap( engine ),
    m_loopMode( false ),
    m_offset( 0 )
{
    if ( parent != NULL )
        m_offset = parent->m_offset + parent->count();
}

MiscScope::~MiscScope()
{
}

void MiscScope::setFunction( const MiscSymbol& symbol )
{
    Q_ASSERT( m_parent == NULL );

    m_function = symbol;

    int paramsCount = symbol.parameters().count();

    for ( int i = 0; i < paramsCount; i++ ) {
        MiscSymbol::Parameter parameter = symbol.parameters().at( i );

        MiscSymbol symbol( parameter.type(), parameter.identifier() );
        m_symbolMap.addSymbol( symbol );
    }
}

int MiscScope::addVariable( const MiscSymbol& symbol, int lineNumber /*= 0*/ )
{
    int index = m_symbolMap.addSymbol( symbol, lineNumber );

    if ( index < 0 )
        return -1;

    return index + m_offset;
}

MiscSymbol MiscScope::symbolAt( int index ) const
{
    return m_symbolMap.symbolAt( index - m_offset );
}

int MiscScope::indexOf( MiscIdentifierPrivate* identifier ) const
{
    int index = m_symbolMap.indexOf( identifier );

    if ( index < 0 )
        return -1;

    return index + m_offset;
}

void MiscScope::setLoopMode( bool on )
{
    m_loopMode = on;
}

void MiscScope::addBreak( size_t pos )
{
    m_breaks.append( pos );
}

void MiscScope::addContinue( size_t pos )
{
    m_continues.append( pos );
}
