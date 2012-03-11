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

#ifndef MISCVISITOR_P_H
#define MISCVISITOR_P_H

#include "misc.h"

namespace Misc { namespace AST {

class Node;
class ListNode;
class IdNode;
class SubscriptNode;
class BooleanNode;
class FloatNode;
class VectorNode;
class MatrixNode;
class UnaryNode;
class BinaryNode;
class LogicalNode;
class ConditionalNode;
class CallNode;
class AssignmentNode;
class VoidNode;
class IfNode;
class WhileNode;
class DoWhileNode;
class ForNode;
class ReturnNode;
class BreakNode;
class ContinueNode;
class BlockNode;
class SimpleNode;
class ArgumentNode;
class VariableNode;
class DeclarationNode;
class FunctionNode;
class UnitNode;

} }

class MiscVisitor
{
public:
    MiscVisitor() { }
    ~MiscVisitor() { }

public:
    virtual bool isValid() { return true; }

    virtual bool enter( Misc::AST::ListNode* ) { return true; }
    virtual void leave( Misc::AST::ListNode* ) { }

    virtual void visit( Misc::AST::IdNode* ) { }

    virtual void visit( Misc::AST::SubscriptNode* ) { }

    virtual void visit( Misc::AST::BooleanNode* ) { }

    virtual void visit( Misc::AST::FloatNode* ) { }

    virtual bool enter( Misc::AST::VectorNode* ) { return true; }
    virtual void leave( Misc::AST::VectorNode* ) { }

    virtual bool enter( Misc::AST::MatrixNode* ) { return true; }
    virtual void leave( Misc::AST::MatrixNode* ) { }

    virtual bool enter( Misc::AST::UnaryNode* ) { return true; }
    virtual void leave( Misc::AST::UnaryNode* ) { }

    virtual bool enter( Misc::AST::BinaryNode* ) { return true; }
    virtual void leave( Misc::AST::BinaryNode* ) { }

    virtual bool enter( Misc::AST::LogicalNode* ) { return true; }
    virtual void leave( Misc::AST::LogicalNode* ) { }

    virtual bool enter( Misc::AST::ConditionalNode* ) { return true; }
    virtual void leave( Misc::AST::ConditionalNode* ) { }

    virtual bool enter( Misc::AST::CallNode* ) { return true; }
    virtual void leave( Misc::AST::CallNode* ) { }

    virtual bool enter( Misc::AST::AssignmentNode* ) { return true; }
    virtual void leave( Misc::AST::AssignmentNode* ) { }

    virtual bool enter( Misc::AST::VoidNode* ) { return true; }
    virtual void leave( Misc::AST::VoidNode* ) { }

    virtual bool enter( Misc::AST::IfNode* ) { return true; }
    virtual void leave( Misc::AST::IfNode* ) { }

    virtual bool enter( Misc::AST::WhileNode* ) { return true; }
    virtual void leave( Misc::AST::WhileNode* ) { }

    virtual bool enter( Misc::AST::DoWhileNode* ) { return true; }
    virtual void leave( Misc::AST::DoWhileNode* ) { }

    virtual bool enter( Misc::AST::ForNode* ) { return true; }
    virtual void leave( Misc::AST::ForNode* ) { }

    virtual bool enter( Misc::AST::ReturnNode* ) { return true; }
    virtual void leave( Misc::AST::ReturnNode* ) { }

    virtual void visit( Misc::AST::BreakNode* ) { }

    virtual void visit( Misc::AST::ContinueNode* ) { }

    virtual bool enter( Misc::AST::BlockNode* ) { return true; }
    virtual void leave( Misc::AST::BlockNode* ) { }

    virtual bool enter( Misc::AST::SimpleNode* ) { return true; }
    virtual void leave( Misc::AST::SimpleNode* ) { }

    virtual void visit( Misc::AST::ArgumentNode* ) { }

    virtual bool enter( Misc::AST::VariableNode* ) { return true; }
    virtual void leave( Misc::AST::VariableNode* ) { }

    virtual bool enter( Misc::AST::DeclarationNode* ) { return true; }
    virtual void leave( Misc::AST::DeclarationNode* ) { }

    virtual bool enter( Misc::AST::FunctionNode* ) { return true; }
    virtual void leave( Misc::AST::FunctionNode* ) { }

    virtual bool enter( Misc::AST::UnitNode* ) { return true; }
    virtual void leave( Misc::AST::UnitNode* ) { }
};

#endif
