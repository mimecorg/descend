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

#ifndef MISCSYMBOL_H
#define MISCSYMBOL_H

#include "miscidentifier.h"

#include <QSharedDataPointer>
#include <QList>

class MiscSymbolPrivate;

class MiscSymbol
{
public:
    class Parameter
    {
    public:
        Parameter();
        Parameter( Misc::Type type, const MiscIdentifier& id );
        Parameter( const Parameter& other );
        ~Parameter();

    public:
        bool isValid() const { return m_type != Misc::InvalidType; }

        Misc::Type type() const { return m_type; }

        const MiscIdentifier& identifier() const { return m_id; }

    public:
        Parameter& operator =( const Parameter& other );

        friend bool operator ==( const Parameter& lhs, const Parameter& rhs )
        {
            return lhs.m_type == rhs.m_type;
        }

        friend bool operator !=( const Parameter& lhs, const Parameter& rhs )
        {
            return lhs.m_type != rhs.m_type;
        }

    private:
        Misc::Type m_type;
        MiscIdentifier m_id;
    };

public:
    MiscSymbol();
    MiscSymbol( Misc::Type type, const MiscIdentifier& id, Misc::SymbolFlags flags = 0 );
    MiscSymbol( const MiscSymbol& other );
    ~MiscSymbol();

public:
    bool isValid() const;

    void setType( Misc::Type type );
    Misc::Type type() const;

    void setIdentifier( const MiscIdentifier& id );
    MiscIdentifier identifier() const;

    void setParameters( const QList<Parameter>& params );
    QList<Parameter> parameters() const;

    void setFlags( Misc::SymbolFlags flags );
    Misc::SymbolFlags flags() const;

public:
    MiscSymbol& operator =( const MiscSymbol& other );

    friend bool operator ==( const MiscSymbol& lhs, const MiscSymbol& rhs );
    friend bool operator !=( const MiscSymbol& lhs, const MiscSymbol& rhs );

private:
    QSharedDataPointer<MiscSymbolPrivate> d;
};

#endif
