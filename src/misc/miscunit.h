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

#ifndef MISCUNIT_H
#define MISCUNIT_H

#include "misc.h"

#include <QObject>

class MiscUnitPrivate;
class MiscEngine;
class MiscSymbol;
class MiscIdentifier;
class MiscValue;
class MiscCode;
class MiscFunction;

class MiscUnit : public QObject
{
    Q_OBJECT
public:
    explicit MiscUnit( MiscEngine* engine );
    explicit MiscUnit( MiscUnit* parent );
    ~MiscUnit();

public:
    MiscCode compileUnit( const QString& text );

    MiscSymbol parseSymbol( const QString& text );

    bool addVariable( const MiscSymbol& symbol );
    MiscCode addVariable( const MiscSymbol& symbol, const QString& text );

    bool addFunction( const MiscSymbol& symbol );
    MiscFunction addFunction( const MiscSymbol& symbol, const QString& text );

    bool setVariable( const MiscIdentifier& id, const MiscValue& value );
    MiscValue variable( const MiscIdentifier& id ) const;

    bool setFunction( const MiscIdentifier& id, const MiscFunction& function );
    MiscFunction function( const MiscIdentifier& id ) const;

private:
    MiscUnitPrivate* d;

    Q_DISABLE_COPY( MiscUnit )
};

#endif
