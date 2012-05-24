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

#include "misclexer_p.h"

#include "miscgrammar_p.h"
#include "miscengine_p.h"

#include <float.h>

MiscLexer::MiscLexer( Mode mode, const QString& text, MiscEnginePrivate* engine ) :
    m_engine( engine ),
    m_mode( mode ),
    m_text( text ),
    m_pos( -1 ),
    m_ch( 0 ),
    m_next( 0 ),
    m_token( MiscGrammar::EOF_SYMBOL ),
    m_lineNumber( 1 ),
    m_id( NULL ),
    m_number( 0.0f )
{
}

MiscLexer::~MiscLexer()
{
}

static bool isLetter( ushort ch )
{
    return ( ch >= 'A' && ch <= 'Z' ) || ( ch >= 'a' && ch <= 'z' );
}

static bool isDigit( ushort ch )
{
    return ( ch >= '0' && ch <= '9' );
}

static const int keyword_tokens[ MiscEnginePrivate::Keywords ] =
{
    MiscGrammar::T_BREAK, MiscGrammar::T_BOOL, MiscGrammar::T_CONST, MiscGrammar::T_CONTINUE, MiscGrammar::T_DO,
    MiscGrammar::T_ELSE, MiscGrammar::T_FALSE, MiscGrammar::T_FLOAT, MiscGrammar::T_FOR, MiscGrammar::T_IF,
    MiscGrammar::T_MATRIX, MiscGrammar::T_RETURN, MiscGrammar::T_TRUE, MiscGrammar::T_VEC, MiscGrammar::T_VOID,
    MiscGrammar::T_WHILE
};

int MiscLexer::lex()
{
    if ( m_pos < 0 ) {
        switch ( m_mode ) {
            case UnitMode:
                m_token = MiscGrammar::T_START_UNIT;
                break;
            case ExpressionMode:
                m_token = MiscGrammar::T_START_EXPRESSION;
                break;
            case SymbolMode:
                m_token = MiscGrammar::T_START_SYMBOL;
                break;
        }

        m_pos = 0;

        m_ch = ( m_text.length() > 0 ) ? m_text.at( 0 ).unicode() : 0;
        m_next = ( m_text.length() > 1 ) ? m_text.at( 1 ).unicode() : 0;

        return m_token;
    }

    if ( m_token == MiscGrammar::EOF_SYMBOL || m_token == MiscGrammar::T_ERROR )
        return m_token;

    for ( ; ; ) {
        if ( m_ch == 0 ) {
            m_token = MiscGrammar::EOF_SYMBOL;
            break;
        }

        if ( m_ch == ' ' || m_ch == '\t' || m_ch == '\n' || m_ch == '\r' ) {
            skip();
            continue;
        }

        if ( m_ch == '/' && m_next == '/' ) {
            do {
                skip();
            } while ( m_ch != 0 && m_ch != '\n' && m_ch != '\r' );

            continue;
        }

        if ( m_ch == '/' && m_next == '*' ) {
            do {
                skip();
            } while ( m_ch != 0 && ( m_ch != '*' || m_next != '/' ) );
            skip();
            skip();

            continue;
        }

        int start = m_pos;

        if ( isLetter( m_ch ) || m_ch == '_' ) {
            do {
                skip();
            } while ( isLetter( m_ch ) || isDigit( m_ch ) || m_ch == '_' );

            m_id = m_engine->lookupIdentifier( m_text.midRef( start, m_pos - start ) );

            if ( m_id->m_index < MiscEnginePrivate::Keywords )
                m_token = keyword_tokens[ m_id->m_index ];
            else
                m_token = MiscGrammar::T_ID;

            break;
        }

        if ( isDigit( m_ch ) || ( m_ch == '.' && isDigit( m_next ) ) ) {
            do {
                if ( ( m_ch == 'e' || m_ch == 'E' ) && ( m_next == '+' || m_next == '-' ) )
                    skip();
                skip();
            } while ( isDigit( m_ch ) || isLetter( m_ch ) || m_ch == '.' );

	    QByteArray number = m_text.mid( start, m_pos - start ).toLatin1();

	    bool ok;
	    double f = number.toDouble( &ok );

	    if ( !ok || ( f < (double)-FLT_MAX ) || ( f > (double)FLT_MAX ) ) {
                m_errorMessage = "invalid number";
                m_token = MiscGrammar::T_ERROR;
                break;
            }

            m_number = (float)f;
            m_token = MiscGrammar::T_NUMBER;

            break;
        }

        switch ( m_ch ) {
            case '&':
                if ( m_next == '&' ) {
                    m_token = MiscGrammar::T_AND_AND;
                    skip();
                } else if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_AND_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_AND;
                }
                break;

            case ':':
                m_token = MiscGrammar::T_COLON;
                break;

            case ',':
                m_token = MiscGrammar::T_COMMA;
                break;

            case '/':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_DIVIDE_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_DIVIDE;
                }
                break;

            case '.':
                m_token = MiscGrammar::T_DOT;
                break;

            case '=':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_EQ_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_EQ;
                }
                break;

            case '>':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_GT_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_GT;
                }
                break;

            case '{':
                m_token = MiscGrammar::T_LBRACE;
                break;

            case '[':
                m_token = MiscGrammar::T_LBRACKET;
                break;

            case '(':
                m_token = MiscGrammar::T_LPAREN;
                break;

            case '<':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_LT_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_LT;
                }
                break;

            case '-':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_MINUS_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_MINUS;
                }
                break;

            case '%':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_MODULO_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_MODULO;
                }
                break;

            case '*':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_MULTIPLY_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_MULTIPLY;
                }
                break;

             case '!':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_NOT_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_NOT;
                }
                break;

            case '|':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_OR_EQ;
                    skip();
                } else if ( m_next == '|' ) {
                    m_token = MiscGrammar::T_OR_OR;
                    skip();
                } else {
                    m_token = MiscGrammar::T_OR;
                }
                break;

            case '+':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_PLUS_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_PLUS;
                }
                break;

            case '^':
                if ( m_next == '=' ) {
                    m_token = MiscGrammar::T_POWER_EQ;
                    skip();
                } else {
                    m_token = MiscGrammar::T_POWER;
                }
                break;

            case '?':
                m_token = MiscGrammar::T_QUESTION;
                break;

            case '}':
                m_token = MiscGrammar::T_RBRACE;
                break;

            case ']':
                m_token = MiscGrammar::T_RBRACKET;
                break;

            case ')':
                m_token = MiscGrammar::T_RPAREN;
                break;

            case ';':
                m_token = MiscGrammar::T_SEMICOLON;
                break;

            default:
                m_errorMessage = "invalid character";
                m_token = MiscGrammar::T_ERROR;
                break;
        }

        if ( m_token != MiscGrammar::T_ERROR )
            skip();

        break;
    }

    return m_token;
}

void MiscLexer::skip()
{
    if ( m_ch == 0 )
        return;

    if ( m_ch == '\n' || ( m_ch == '\r' && m_next != '\n' ) )
        m_lineNumber++;

    m_pos++;

    m_ch = m_next;
    m_next = ( m_text.length() > m_pos + 1 ) ? m_text.at( m_pos + 1 ).unicode() : 0;
}
