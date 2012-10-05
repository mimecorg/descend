---------------------------------------------------------------------------
-- This file is part of the Descend program
-- Copyright (C) 2012 Michał Męciński
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------------

-- NOTE: This file is used to generate miscgrammar.cpp, miscgrammar_p.h,
-- miscparser.cpp and miscparser_p.h with qlarl. You can grab qlarl
-- from <http://qt.gitorious.org/qt/qlalr>.

%parser MiscGrammar
%decl miscparser_p.h
%impl miscparser.cpp

%expect 1

%token T_START_UNIT
%token T_START_EXPRESSION
%token T_START_SYMBOL

%token T_AND "&"
%token T_AND_AND "&&"
%token T_AND_EQ "&="
%token T_COLON ":"
%token T_COMMA ","
%token T_DIVIDE "/"
%token T_DIVIDE_EQ "/="
%token T_DOT "."
%token T_EQ "="
%token T_EQ_EQ "=="
%token T_GT ">"
%token T_GT_EQ ">="
%token T_LBRACE "{"
%token T_LBRACKET "["
%token T_LPAREN "("
%token T_LT "<"
%token T_LT_EQ "<="
%token T_MINUS "-"
%token T_MINUS_EQ "-="
%token T_MODULO "%"
%token T_MODULO_EQ "%="
%token T_MULTIPLY "*"
%token T_MULTIPLY_EQ "*="
%token T_NOT "!"
%token T_NOT_EQ "!="
%token T_OR "|"
%token T_OR_EQ "|="
%token T_OR_OR "||"
%token T_PLUS "+"
%token T_PLUS_EQ "+="
%token T_POWER "^"
%token T_POWER_EQ "^="
%token T_QUESTION "?"
%token T_RBRACE "}"
%token T_RBRACKET "]"
%token T_RPAREN ")"
%token T_SEMICOLON ";"

%token T_BREAK "break"
%token T_BOOL "bool"
%token T_CONST "const"
%token T_CONTINUE "continue"
%token T_DO "do"
%token T_ELSE "else"
%token T_FALSE "false"
%token T_FLOAT "float"
%token T_FOR "for"
%token T_IF "if"
%token T_MATRIX "matrix"
%token T_RETURN "return"
%token T_TRUE "true"
%token T_VEC "vec"
%token T_VOID "void"
%token T_WHILE "while"

%token T_NUMBER "number"
%token T_ID "indentifier"

%token T_ERROR

%start Start

-- HEADER FILE ------------------------------------------------------------

/:
// NOTE: This file was automatically generated from misc.g by qlalr.
// Do not modify this file directly. You can grab qlarl from
// <http://qt.gitorious.org/qt/qlalr>.

#ifndef MISCPARSER_P_H
#define MISCPARSER_P_H

#include "miscgrammar_p.h"
#include "miscast_p.h"

#include <QString>

class MiscPool;
class MiscLexer;

class MiscParser : protected $table
{
public:
    MiscParser( MiscPool* pool );
    ~MiscParser();

public:
    bool parse( MiscLexer* lexer );

    Misc::AST::Node* result() const { return m_result; }

    const QString& errorMessage() const { return m_errorMessage; }
    int errorLineNumber() const { return m_errorLineNumber; }

private:
    union Value
    {
        Misc::Type m_type;
        MiscIdentifierPrivate* m_id;
        Misc::AST::Node* m_node;
        Misc::AST::ListNode* m_list;
    };

private:
    void reallocateStack();

    Value& sym( int index ) { return m_symStack[ m_tos + index - 1 ]; }

    int line() const { return m_lineStack[ m_tos ]; }

private:
    MiscPool* m_pool;

    int m_tos;

    int m_stackSize;
    Value* m_symStack;
    int* m_stateStack;
    int* m_lineStack;

    Misc::AST::Node* m_result;

    QString m_errorMessage;
    int m_errorLineNumber;
};

#endif

:/

-- IMPLEMENTATION FILE ----------------------------------------------------

/.
// NOTE: This file was automatically generated from misc.g by qlalr.
// Do not modify this file directly. You can grab qlarl from
// <http://qt.gitorious.org/qt/qlalr>.

#include "miscparser_p.h"

#include "miscpool_p.h"
#include "misclexer_p.h"

MiscParser::MiscParser( MiscPool* pool ) :
    m_pool( pool ),
    m_tos( 0 ),
    m_stackSize( 0 ),
    m_symStack( NULL ),
    m_stateStack( NULL ),
    m_lineStack( NULL ),
    m_result( NULL ),
    m_errorLineNumber( 0 )
{
}

MiscParser::~MiscParser()
{
    free( m_symStack );
    free( m_stateStack );
    free( m_lineStack );
}

bool MiscParser::parse( MiscLexer* lexer )
{
    int token = -1;

    reallocateStack();

    m_tos = 0;
    m_stateStack[ m_tos ] = 0;

    m_result = NULL;

    while ( true ) {
        const int state = m_stateStack[ m_tos ];

        if ( token == -1 && action_index[ state ] != -TERMINAL_COUNT )
            token = lexer->lex();

        int act = t_action( state, token );

        if ( act == ACCEPT_STATE )
            return true;

        if ( act > 0 ) {
            if ( ++m_tos == m_stackSize )
                reallocateStack();

            m_stateStack[ m_tos ] = act;
            m_lineStack[ m_tos ] = lexer->lineNumber();

            token = -1;
        } else if ( act < 0 ) {
            int r = -act - 1;

            m_tos -= rhs[ r ];
            act = m_stateStack[ m_tos++ ];

            switch ( r ) {
./

-- BEGIN RULES ------------------------------------------------------------

Start ::= T_START_UNIT StatementsOrFunctionsListOpt ;
/.
                case $rule_number:
                    m_result = new ( m_pool ) Misc::AST::UnitNode( sym( 2 ).m_list, line() );
                    break;
./
Start ::= T_START_EXPRESSION Expression ;
/.
                case $rule_number:
                    m_result = sym( 2 ).m_node;
                    break;
./
Start ::= T_START_SYMBOL Symbol ;
/.
                case $rule_number:
                    m_result = sym( 2 ).m_node;
                    break;
./

StatementsOrFunctionsListOpt ::= StatementsOrFunctionsList ;
StatementsOrFunctionsListOpt ::= ;
/.
                case $rule_number:
                    sym( 1 ).m_node = NULL;
                    break;
./

StatementsOrFunctionsList ::= StatementsOrFunctionsList StatementOrFunction ;
/.
                case $rule_number:
                    sym( 1 ).m_list->append( sym( 2 ).m_node );
                    break;
./
StatementsOrFunctionsList ::= StatementOrFunction ;
/.
                case $rule_number:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;
./

StatementOrFunction ::= Statement ;
StatementOrFunction ::= FunctionDeclaration ;

StatementsListOpt ::= StatementsList ;
StatementsListOpt ::= ;
/.
                case $rule_number:
                    sym( 1 ).m_node = NULL;
                    break;
./

StatementsList ::= StatementsList Statement ;
/.
                case $rule_number:
                    if ( sym( 2 ).m_node )
                        sym( 1 ).m_list->append( sym( 2 ).m_node );
                    break;
./
StatementsList ::= Statement ;
/.
                case $rule_number:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;
./

Statement ::= VariableDeclaration ;
Statement ::= SimpleStatement T_SEMICOLON ;
Statement ::= Block ;
Statement ::= T_IF T_LPAREN Expression T_RPAREN Statement ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::IfNode( sym( 3 ).m_node, sym( 5 ).m_node, NULL, line() );
                    break;
./
Statement ::= T_IF T_LPAREN Expression T_RPAREN Statement T_ELSE Statement ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::IfNode( sym( 3 ).m_node, sym( 5 ).m_node, sym( 7 ).m_node, line() );
                    break;
./
Statement ::= T_WHILE T_LPAREN Expression T_RPAREN Statement ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::WhileNode( sym( 3 ).m_node, sym( 5 ).m_node, line() );
                    break;
./
Statement ::= T_DO Statement T_WHILE T_LPAREN Expression T_RPAREN T_SEMICOLON ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DoWhileNode( sym( 2 ).m_node, sym( 5 ).m_node, line() );
                    break;
./
Statement ::= T_FOR T_LPAREN SimpleStatementOrDeclaration T_SEMICOLON ExpressionOpt T_SEMICOLON SimpleStatement T_RPAREN Statement ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::ForNode( sym( 3 ).m_node, sym( 5 ).m_node, sym( 7 ).m_node, sym( 9 ).m_node, line() );
                    break;
./
Statement ::= T_RETURN ExpressionOpt T_SEMICOLON ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::ReturnNode( sym( 2 ).m_node, line() );
                    break;
./
Statement ::= T_BREAK T_SEMICOLON ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BreakNode( line() );
                    break;
./
Statement ::= T_CONTINUE T_SEMICOLON ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::ContinueNode( line() );
                    break;
./

Block ::= T_LBRACE StatementsListOpt T_RBRACE ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BlockNode( sym( 2 ).m_list, line() );
                    break;
./

FunctionDeclaration ::= Type Id T_LPAREN ArgListOpt T_RPAREN Block ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::FunctionNode( sym( 1 ).m_type, sym( 2 ).m_id, sym( 4 ).m_list, sym( 6 ).m_node, line() );
                    break;
./
FunctionDeclaration ::= Type Id T_LPAREN ArgListOpt T_RPAREN T_EQ Expression T_SEMICOLON ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::FunctionNode( sym( 1 ).m_type, sym( 2 ).m_id, sym( 4 ).m_list, sym( 7 ).m_node, line() );
                    break;
./

ArgListOpt ::= ArgList ;
ArgListOpt ::= ;
/.
                case $rule_number:
                    sym( 1 ).m_node = NULL;
                    break;
./

ArgList ::= ArgList T_COMMA Argument ;
/.
                case $rule_number:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;
./
ArgList ::= Argument ;
/.
                case $rule_number:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;
./

Argument ::= Type Id ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::ArgumentNode( sym( 1 ).m_type, sym( 2 ).m_id, line() );
                    break;
./

Type ::= T_VOID ;
/.
                case $rule_number:
                    sym( 1 ).m_type = Misc::VoidType;
                    break;
./
Type ::= T_BOOL ;
/.
                case $rule_number:
                    sym( 1 ).m_type = Misc::BoolType;
                    break;
./
Type ::= T_FLOAT ;
/.
                case $rule_number:
                    sym( 1 ).m_type = Misc::FloatType;
                    break;
./
Type ::= T_VEC ;
/.
                case $rule_number:
                    sym( 1 ).m_type = Misc::VectorType;
                    break;
./
Type ::= T_MATRIX ;
/.
                case $rule_number:
                    sym( 1 ).m_type = Misc::MatrixType;
                    break;
./

Id ::= T_ID ;
/.
                case $rule_number:
                    sym( 1 ).m_id = lexer->id();
                    break;
./

VariableDeclaration ::= Type VariableDeclarationsList T_SEMICOLON ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DeclarationNode( 0, sym( 1 ).m_type, sym( 2 ).m_list, line() );
                    break;
./
VariableDeclaration ::= T_CONST Type InitializedVariableDeclarationsList T_SEMICOLON ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DeclarationNode( Misc::ConstantSymbol, sym( 2 ).m_type, sym( 3 ).m_list, line() );
                    break;
./

VariableDeclarationsList ::= VariableDeclarationsList T_COMMA Variable ;
/.
                case $rule_number:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;
./
VariableDeclarationsList ::= Variable ;
/.
                case $rule_number:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;
./

Variable ::= InitializedVariable ;
Variable ::= Id ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::VariableNode( sym( 1 ).m_id, NULL, line() );
                    break;
./

InitializedVariableDeclarationsList ::= InitializedVariableDeclarationsList T_COMMA InitializedVariable ;
/.
                case $rule_number:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;
./
InitializedVariableDeclarationsList ::= InitializedVariable ;
/.
                case $rule_number:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;
./

InitializedVariable ::= Id T_EQ Expression;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::VariableNode( sym( 1 ).m_id, sym( 3 ).m_node, line() );
                    break;
./

SimpleStatementOrDeclaration ::= SimpleStatement ;
SimpleStatementOrDeclaration ::= Type InitializedVariableDeclarationsList ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DeclarationNode( 0, sym( 1 ).m_type, sym( 2 ).m_list, line() );
                    break;
./

SimpleStatement ::= AssignmentOrVoidListOpt ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::SimpleNode( sym( 1 ).m_list, line() );
                    break;
./

AssignmentOrVoidListOpt ::= AssignmentOrVoidList ;
AssignmentOrVoidListOpt ::= ;
/.
                case $rule_number:
                    sym( 1 ).m_node = NULL;
                    break;
./

AssignmentOrVoidList ::= AssignmentOrVoidList T_COMMA AssignmentOrVoid ;
/.
                case $rule_number:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;
./
AssignmentOrVoidList ::= AssignmentOrVoid ;
/.
                case $rule_number:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;
./

AssignmentOrVoid ::= LValue T_EQ Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Eq, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AssignmentOrVoid ::= LValue T_PLUS_EQ Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Plus, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AssignmentOrVoid ::= LValue T_MINUS_EQ Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Minus, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AssignmentOrVoid ::= LValue T_MULTIPLY_EQ Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Multiply, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AssignmentOrVoid ::= LValue T_DIVIDE_EQ Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Divide, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AssignmentOrVoid ::= LValue T_MODULO_EQ Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Modulo, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AssignmentOrVoid ::= LValue T_POWER_EQ Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Power, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AssignmentOrVoid ::= LValue T_AND_EQ Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::And, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AssignmentOrVoid ::= LValue T_OR_EQ Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Or, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AssignmentOrVoid ::= FunctionCall ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::VoidNode( sym( 1 ).m_node, line() );
                    break;
./

ExpressionOpt ::= Expression ;
ExpressionOpt ::= ;
/.
                case $rule_number:
                    sym( 1 ).m_node = NULL;
                    break;
./

Expression ::= ConditionalExpression ;

ConditionalExpression ::= LogicalOrExpression T_QUESTION ConditionalExpression T_COLON ConditionalExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::ConditionalNode( sym( 1 ).m_node, sym( 3 ).m_node, sym( 5 ).m_node, line() );
                    break;
./
ConditionalExpression ::= LogicalOrExpression ;

LogicalOrExpression ::= LogicalOrExpression T_OR_OR LogicalAndExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::LogicalNode( Misc::AST::OrOr, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
LogicalOrExpression ::= LogicalAndExpression ;

LogicalAndExpression ::= LogicalAndExpression T_AND_AND BitwiseOrExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::LogicalNode( Misc::AST::AndAnd, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
LogicalAndExpression ::= BitwiseOrExpression ;

BitwiseOrExpression ::= BitwiseOrExpression T_OR BitwiseAndExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Or, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
BitwiseOrExpression ::= BitwiseAndExpression ;

BitwiseAndExpression ::= BitwiseAndExpression T_AND EqualityExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::And, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
BitwiseAndExpression ::= EqualityExpression ;

EqualityExpression ::= EqualityExpression T_EQ_EQ RelationalExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::EqEq, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
EqualityExpression ::= EqualityExpression T_NOT_EQ RelationalExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::NotEq, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
EqualityExpression ::= RelationalExpression ;

RelationalExpression ::= RelationalExpression T_GT AdditiveExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Gt, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
RelationalExpression ::= RelationalExpression T_GT_EQ AdditiveExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::GtEq, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
RelationalExpression ::= RelationalExpression T_LT AdditiveExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Lt, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
RelationalExpression ::= RelationalExpression T_LT_EQ AdditiveExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::LtEq, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
RelationalExpression ::= AdditiveExpression ;

AdditiveExpression ::= AdditiveExpression T_PLUS MultiplicativeExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Plus, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AdditiveExpression ::= AdditiveExpression T_MINUS MultiplicativeExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Minus, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
AdditiveExpression ::= MultiplicativeExpression ;

MultiplicativeExpression ::= MultiplicativeExpression T_MULTIPLY PowerExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Multiply, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
MultiplicativeExpression ::= MultiplicativeExpression T_DIVIDE PowerExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Divide, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
MultiplicativeExpression ::= MultiplicativeExpression T_MODULO PowerExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Modulo, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
MultiplicativeExpression ::= PowerExpression ;

PowerExpression ::= PowerExpression T_POWER PrefixExpression ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Power, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;
./
PowerExpression ::= PrefixExpression ;

PrefixExpression ::= T_PLUS Literal ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::UnaryNode( Misc::AST::Plus, sym( 2 ).m_node, line() );
                    break;
./
PrefixExpression ::= T_MINUS Literal ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::UnaryNode( Misc::AST::Minus, sym( 2 ).m_node, line() );
                    break;
./
PrefixExpression ::= T_NOT Literal ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::UnaryNode( Misc::AST::Not, sym( 2 ).m_node, line() );
                    break;
./
PrefixExpression ::= Literal ;

Literal ::= T_LPAREN Expression T_RPAREN ;
/.
                case $rule_number:
                    sym( 1 ).m_node = sym( 2 ).m_node;
                    break;
./
Literal ::= T_LBRACKET Vector T_RBRACKET ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::VectorNode( sym( 2 ).m_list, line() );
                    break;
./
Literal ::= T_LBRACKET Matrix T_RBRACKET ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::MatrixNode( sym( 2 ).m_list, line() );
                    break;
./
Literal ::= T_NUMBER ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::FloatNode( lexer->number(), line() );
                    break;
./
Literal ::= T_TRUE ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BooleanNode( true, line() );
                    break;
./
Literal ::= T_FALSE ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BooleanNode( false, line() );
                    break;
./
Literal ::= LValue ;
Literal ::= FunctionCall ;

LValue ::= Id ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::IdNode( sym( 1 ).m_id, line() );
                    break;
./
LValue ::= Id T_DOT Id ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::SubscriptNode( sym( 1 ).m_id, sym( 3 ).m_id, line() );
                    break;
./

FunctionCall ::= Id T_LPAREN ExpressionsListOpt T_RPAREN ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::CallNode( sym( 1 ).m_id, sym( 3 ).m_list, line() );
                    break;
./

ExpressionsListOpt ::= ExpressionsList ;
ExpressionsListOpt ::= ;
/.
                case $rule_number:
                    sym( 1 ).m_node = NULL;
                    break;
./

ExpressionsList ::= ExpressionsList T_COMMA Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;
./
ExpressionsList ::= Expression ;
/.
                case $rule_number:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;
./

Vector ::= ExpressionsList;

Matrix ::= Matrix T_SEMICOLON ExpressionsList ;
/.
                case $rule_number:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;
./
Matrix ::= ExpressionsList T_SEMICOLON ExpressionsList;
/.
                case $rule_number:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;
./

Symbol ::= Type SymbolVariableList ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DeclarationNode( 0, sym( 1 ).m_type, sym( 2 ).m_list, line() );
                    break;
./
Symbol ::= T_CONST Type SymbolVariableList ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DeclarationNode( Misc::ConstantSymbol, sym( 2 ).m_type, sym( 3 ).m_list, line() );
                    break;
./
Symbol ::= Type Id T_LPAREN ArgListOpt T_RPAREN ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::FunctionNode( sym( 1 ).m_type, sym( 2 ).m_id, sym( 4 ).m_list, NULL, line() );
                    break;
./

SymbolVariableList ::= SymbolVariable ;
/.
                case $rule_number:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;
./

SymbolVariable ::= Id ;
/.
                case $rule_number:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::VariableNode( sym( 1 ).m_id, NULL, line() );
                    break;
./

-- END RULES --------------------------------------------------------------

/.
            }

            m_stateStack[ m_tos ] = nt_action( act, lhs[ r ] - TERMINAL_COUNT );
        } else {
            int token = lexer->token();

            if ( token == T_ERROR )
                m_errorMessage = lexer->errorMessage();
            else if ( token == EOF_SYMBOL )
                m_errorMessage = QString( "unexpected end of input" );
            else if ( spell[ token ] )
                m_errorMessage = QString( "unexpected '%1'" ).arg( spell[ token ] );

            m_errorLineNumber = lexer->lineNumber();

            return false;
        }
    }

    return false;
}

void MiscParser::reallocateStack()
{
    if ( m_stackSize == 0 )
        m_stackSize = 128;
    else
        m_stackSize <<= 1;

    m_symStack = static_cast<Value*>( realloc( m_symStack, m_stackSize * sizeof( Value ) ) );
    m_stateStack = static_cast<int*>( realloc( m_stateStack, m_stackSize * sizeof( int ) ) );
    m_lineStack = static_cast<int*>( realloc( m_lineStack, m_stackSize * sizeof( int ) ) );
}

./
