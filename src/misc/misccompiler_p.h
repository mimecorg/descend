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

#ifndef MISCCOMPILER_P_H
#define MISCCOMPILER_P_H

#include "miscvisitor_p.h"
#include "miscast_p.h"
#include "miscop_p.h"

class MiscEnginePrivate;
class MiscUnitPrivate;
class MiscScope;
class MiscCodePrivate;
class MiscFunctionPrivate;
class MiscIdentifierPrivate;
class MiscSymbol;

class MiscCompiler : private MiscVisitor
{
public:
    explicit MiscCompiler( MiscUnitPrivate* unit );
    ~MiscCompiler();

public:
    MiscCodePrivate* compileUnit( Misc::AST::UnitNode* node );

    MiscFunctionPrivate* compileFunction( const MiscSymbol& symbol, Misc::AST::Node* node );

    MiscCodePrivate* compileInitializer( int slot, Misc::Type type, Misc::AST::Node* node );

private: // MiscVisitor implementation
    bool isValid();

    void visit( Misc::AST::IdNode* node );

    void visit( Misc::AST::SubscriptNode* node );

    void visit( Misc::AST::BooleanNode* node );

    void visit( Misc::AST::FloatNode* node );

    bool enter( Misc::AST::VectorNode* node );
    void leave( Misc::AST::VectorNode* node );

    bool enter( Misc::AST::MatrixNode* node );
    void leave( Misc::AST::MatrixNode* node );

    bool enter( Misc::AST::UnaryNode* node );
    void leave( Misc::AST::UnaryNode* node );

    bool enter( Misc::AST::BinaryNode* node );
    void leave( Misc::AST::BinaryNode* node );

    bool enter( Misc::AST::LogicalNode* node );

    bool enter( Misc::AST::ConditionalNode* node );

    bool enter( Misc::AST::CallNode* node );
    void leave( Misc::AST::CallNode* node );

    bool enter( Misc::AST::AssignmentNode* node );

    bool enter( Misc::AST::VoidNode* node );
    void leave( Misc::AST::VoidNode* node );

    bool enter( Misc::AST::IfNode* node );

    bool enter( Misc::AST::WhileNode* node );

    bool enter( Misc::AST::DoWhileNode* node );

    bool enter( Misc::AST::ForNode* node );

    bool enter( Misc::AST::ReturnNode* node );

    void visit( Misc::AST::BreakNode* node );

    void visit( Misc::AST::ContinueNode* node );

    bool enter( Misc::AST::BlockNode* node );

    bool enter( Misc::AST::DeclarationNode* node );

    bool enter( Misc::AST::FunctionNode* node );

private:
    MiscCodePrivate* createCode();
    MiscFunctionPrivate* createFunction( const MiscSymbol& symbol );

    bool binaryOperator( Misc::AST::Operator op, int lineNumber = 0 );

    int componentIndex( Misc::Type type, MiscIdentifierPrivate* component, int lineNumber = 0 );

    void appendPushDefault( Misc::Type type );

    void appendPop( int count );

    void appendLoadLocal( Misc::Type type, int index );
    void appendLoadGlobal( Misc::Type type, int slot );
    void appendLoadLocal( Misc::Type type, int index, int comp );
    void appendLoadGlobal( Misc::Type type, int slot, int comp );

    void appendStoreLocal( Misc::Type type, int index );
    void appendStoreGlobal( Misc::Type type, int slot );
    void appendStoreLocal( Misc::Type type, int index, int comp );
    void appendStoreGlobal( Misc::Type type, int slot, int comp );

    void append( Misc::OP::Code code );
    void append( Misc::OP::Code code, int arg );
    void append( Misc::OP::Code code, int arg1, int arg2 );
    void append( Misc::OP::Code code, float arg );

    Misc::OP::Instruction* growOps( size_t size )
    {
        if ( m_opsSize + size > m_opsCapacity )
            reallocOps();

        Misc::OP::Instruction* result = reinterpret_cast<Misc::OP::Instruction*>( m_opsTable + m_opsSize );

        m_opsSize += size;

        return result;
    }

    void reallocOps();

    void adjustLoop( MiscScope* scope, size_t continuePos, size_t breakPos );

    void adjustJump( size_t pos, size_t target );

    void push( Misc::Type type );

    void pop() { m_stackTop--; }
    void pop( int n ) { m_stackTop -= n; }

private:
    MiscUnitPrivate* m_unit;
    MiscScope* m_scope;
    MiscEnginePrivate* m_engine;

    size_t m_opsCapacity;
    size_t m_opsSize;
    char* m_opsTable;

    int m_stackCapacity;
    int m_stackSize;
    int m_stackTop;
    Misc::Type* m_stack;
};

#endif
