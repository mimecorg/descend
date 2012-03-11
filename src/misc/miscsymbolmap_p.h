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

#ifndef MISCSYMBOLMAP_P_H
#define MISCSYMBOLMAP_P_H

#include "miscsymbol.h"
#include "miscast_p.h"

#include <QHash>

class MiscEnginePrivate;

class MiscSymbolMap
{
public:
    explicit MiscSymbolMap( MiscEnginePrivate* engine );
    ~MiscSymbolMap();

public:
    MiscSymbol makeSymbol( Misc::AST::FunctionNode* functionNode );
    MiscSymbol makeSymbol( Misc::AST::DeclarationNode* declarationNode, Misc::AST::VariableNode* variableNode );

    bool checkSymbol( const MiscSymbol& symbol, int lineNumber = 0 );

    int addSymbol( const MiscSymbol& symbol, int lineNumber = 0 );

    int count() const { return m_symbols.count(); }

    MiscSymbol symbolAt( int index ) const;

    int indexOf( MiscIdentifierPrivate* identifier ) const;

private:
    bool isReserved( MiscIdentifierPrivate* identifier, int lineNumber = 0 );

private:
    MiscEnginePrivate* m_engine;

    QHash<int, int> m_map;

    QList<MiscSymbol> m_symbols;
};

#endif
