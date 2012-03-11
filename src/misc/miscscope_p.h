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

#ifndef MISCSCOPE_P_H
#define MISCSCOPE_P_H

#include "miscsymbolmap_p.h"

#include <QList>

class MiscEnginePrivate;

class MiscScope
{
public:
    explicit MiscScope( MiscEnginePrivate* engine, MiscScope* parent = NULL );
    ~MiscScope();

public:
    MiscScope* parent() const { return m_parent; }

    void setFunction( const MiscSymbol& symbol );
    MiscSymbol function() const { return m_function; }

    int addVariable( const MiscSymbol& symbol, int lineNumber = 0 );

    int count() const { return m_symbolMap.count(); }
    int offset() const { return m_offset; }

    MiscSymbol symbolAt( int index ) const;

    int indexOf( MiscIdentifierPrivate* identifier ) const;

    void setLoopMode( bool on );
    bool isLoopMode() { return m_loopMode; }

    void addBreak( size_t pos );
    void addContinue( size_t pos );

    QList<size_t> breaks() const { return m_breaks; }
    QList<size_t> continues() const { return m_continues; }

private:
    MiscScope* m_parent;
    MiscEnginePrivate* m_engine;

    MiscSymbolMap m_symbolMap;

    MiscSymbol m_function;

    bool m_loopMode;
    QList<size_t> m_breaks;
    QList<size_t> m_continues;

    int m_offset;
};

#endif
