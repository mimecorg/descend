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

#ifndef MISCPRINTVISITOR_P_H
#define MISCPRINTVISITOR_P_H

#include "misc.h"

#if defined( MISC_DEBUG )

#include "miscvisitor_p.h"
#include "miscast_p.h"

class QTextStream;

class MiscPrintVisitor : public MiscVisitor
{
public:
    explicit MiscPrintVisitor( QTextStream& stream );
    ~MiscPrintVisitor();

public:
    bool enter( Misc::AST::ListNode* node );
    void leave( Misc::AST::ListNode* node );

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
    void leave( Misc::AST::LogicalNode* node );

    bool enter( Misc::AST::ConditionalNode* node );
    void leave( Misc::AST::ConditionalNode* node );

    bool enter( Misc::AST::CallNode* node );
    void leave( Misc::AST::CallNode* node );

    bool enter( Misc::AST::AssignmentNode* node );
    void leave( Misc::AST::AssignmentNode* node );

    bool enter( Misc::AST::VoidNode* node );
    void leave( Misc::AST::VoidNode* node );

    bool enter( Misc::AST::IfNode* node );
    void leave( Misc::AST::IfNode* node );

    bool enter( Misc::AST::WhileNode* node );
    void leave( Misc::AST::WhileNode* node );

    bool enter( Misc::AST::DoWhileNode* node );
    void leave( Misc::AST::DoWhileNode* node );

    bool enter( Misc::AST::ForNode* node );
    void leave( Misc::AST::ForNode* node );

    bool enter( Misc::AST::ReturnNode* node );
    void leave( Misc::AST::ReturnNode* node );

    void visit( Misc::AST::BreakNode* node );

    void visit( Misc::AST::ContinueNode* node );

    bool enter( Misc::AST::BlockNode* node );
    void leave( Misc::AST::BlockNode* node );

    bool enter( Misc::AST::SimpleNode* node );
    void leave( Misc::AST::SimpleNode* node );

    void visit( Misc::AST::ArgumentNode* node );

    bool enter( Misc::AST::VariableNode* node );
    void leave( Misc::AST::VariableNode* node );

    bool enter( Misc::AST::DeclarationNode* node );
    void leave( Misc::AST::DeclarationNode* node );

    bool enter( Misc::AST::FunctionNode* node );
    void leave( Misc::AST::FunctionNode* node );

    bool enter( Misc::AST::UnitNode* node );
    void leave( Misc::AST::UnitNode* node );

private:
    bool enter();
    void leave();

    void indent();

    void print( Misc::Type type );
    void print( Misc::AST::Operator op );

private:
    QTextStream& m_stream;
    int m_indent;
};

#endif // defined( MISC_DEBUG )

#endif
