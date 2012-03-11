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

#ifndef MISCLEXER_P_H
#define MISCLEXER_P_H

#include "misc.h"

class MiscEnginePrivate;
class MiscIdentifierPrivate;

#include <QString>

class MiscLexer
{
public:
    enum Mode
    {
        UnitMode,
        ExpressionMode,
        SymbolMode
    };

public:
    MiscLexer( Mode mode, const QString& text, MiscEnginePrivate* engine );
    ~MiscLexer();

public:
    int lex();

    int token() const { return m_token; }

    int lineNumber() const { return m_lineNumber; }

    MiscIdentifierPrivate* id() const { return m_id; }

    float number() const { return m_number; }

    QString errorMessage() const { return m_errorMessage; }

private:
    void skip();

private:
    MiscEnginePrivate* m_engine;

    Mode m_mode;
    QString m_text;

    int m_pos;
    ushort m_ch;
    ushort m_next;

    int m_token;
    int m_lineNumber;

    MiscIdentifierPrivate* m_id;
    float m_number;

    QString m_errorMessage;
};

#endif
