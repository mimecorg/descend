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

#ifndef MISCUNIT_P_H
#define MISCUNIT_P_H

#include "miscsymbolmap_p.h"
#include "misclexer_p.h"

class MiscEnginePrivate;
class MiscPool;
class MiscCodePrivate;
class MiscFunctionPrivate;

class MiscUnitPrivate
{
public:
    explicit MiscUnitPrivate( MiscEnginePrivate* engine, MiscUnitPrivate* parent = NULL );
    ~MiscUnitPrivate();

public:
    MiscUnitPrivate* parent() const { return m_parent; }
    MiscEnginePrivate* engine() const { return m_engine; }

    int addVariable( const MiscSymbol& symbol, int lineNumber = 0 );
    int addFunction( const MiscSymbol& symbol, int lineNumber = 0 );

    MiscSymbol symbolAt( int index ) const;
    int slotAt( int index ) const;

    int indexOf( MiscIdentifierPrivate* identifier ) const;

private:
    Misc::AST::Node* parse( MiscLexer::Mode mode, const QString& text, MiscPool* pool );

private:
    MiscUnitPrivate* m_parent;
    MiscEnginePrivate* m_engine;

    MiscSymbolMap m_symbolMap;

    QList<int> m_slots;

    friend class MiscUnit;
};

#endif
