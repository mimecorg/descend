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

#ifndef MISCAST_P_H
#define MISCAST_P_H

#include "miscvisitor_p.h"

class MiscIdentifierPrivate;

namespace Misc { namespace AST {

enum Kind
{
    List,
    Id,
    Subscript,
    Boolean,
    Float,
    Vector,
    Matrix,
    Unary,
    Binary,
    Logical,
    Conditional,
    Call,
    Assignment,
    Void,
    If,
    While,
    DoWhile,
    For,
    Return,
    Break,
    Continue,
    Block,
    Simple,
    Argument,
    Variable,
    Declaration,
    Function,
    Unit
};

enum Operator
{
    And,
    AndAnd,
    Divide,
    Eq,
    EqEq,
    Gt,
    GtEq,
    Lt,
    LtEq,
    Minus,
    Modulo,
    Multiply,
    Not,
    NotEq,
    Or,
    OrOr,
    Plus,
    Power,
    OperatorsCount
};

const char* operatorName( Operator op );

class Node
{
public:
    Node( Kind kind, int line ) :
        m_kind( kind ),
        m_line( line ),
        m_next( NULL )
    {
    }

    virtual ~Node()
    {
    }

public:
    virtual void accept( MiscVisitor* visitor ) = 0;

public:
    Kind m_kind;
    int m_line;
    Node* m_next;
};

class ListNode : public Node
{
public:
    ListNode( Node* child, int line ) : Node( List, line ),
        m_head( child ),
        m_tail( child ),
        m_count( 1 )
    {
    }

    ~ListNode()
    {
    }

public:
    void append( Node* child )
    {
        m_tail->m_next = child;
        m_tail = child;
        m_count++;
    }

    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            acceptChildren( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

    void acceptChildren( MiscVisitor* visitor )
    {
        Node* node = m_head;
        while ( node != NULL ) {
            node->accept( visitor );
            node = node->m_next;
        }
    }

public:
    Node* m_head;
    Node* m_tail;
    int m_count;
};

class IdNode : public Node
{
public:
    IdNode( MiscIdentifierPrivate* id, int line ) : Node( Id, line ),
        m_id( id )
    {
    }

    ~IdNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() )
            visitor->visit( this );
    }

public:
    MiscIdentifierPrivate* m_id;
};

class SubscriptNode : public Node
{
public:
    SubscriptNode( MiscIdentifierPrivate* id, MiscIdentifierPrivate* component, int line ) : Node( Subscript, line ),
        m_id( id ),
        m_component( component )
    {
    }

    ~SubscriptNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() )
            visitor->visit( this );
    }

public:
    MiscIdentifierPrivate* m_id;
    MiscIdentifierPrivate* m_component;
};

class BooleanNode : public Node
{
public:
    BooleanNode( bool b, int line ) : Node( Boolean, line ),
        m_bool( b )
    {
    }

    ~BooleanNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() )
            visitor->visit( this );
    }

public:
    bool m_bool;
};

class FloatNode : public Node
{
public:
    FloatNode( float f, int line ) : Node( Float, line ),
        m_float( f )
    {
    }

    ~FloatNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() )
            visitor->visit( this );
    }

public:
    float m_float;
};

class VectorNode : public Node
{
public:
    VectorNode( ListNode* list, int line ) : Node( Vector, line ),
        m_list( list )
    {
    }

    ~VectorNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_list->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    ListNode* m_list;
};

class MatrixNode : public Node
{
public:
    MatrixNode( ListNode* list, int line ) : Node( Matrix, line ),
        m_list( list )
    {
    }

    ~MatrixNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_list->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    ListNode* m_list;
};

class UnaryNode : public Node
{
public:
    UnaryNode( Operator op, Node* arg, int line ) : Node( Unary, line ),
        m_op( op ),
        m_arg( arg )
    {
    }

    ~UnaryNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_arg->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Operator m_op;
    Node* m_arg;
};

class BinaryNode : public Node
{
public:
    BinaryNode( Operator op, Node* arg1, Node* arg2, int line ) : Node( Binary, line ),
        m_op( op ),
        m_arg1( arg1 ),
        m_arg2( arg2 )
    {
    }

    ~BinaryNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_arg1->accept( visitor );
            m_arg2->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Operator m_op;
    Node* m_arg1;
    Node* m_arg2;
};

class LogicalNode : public Node
{
public:
    LogicalNode( Operator op, Node* arg1, Node* arg2, int line ) : Node( Logical, line ),
        m_op( op ),
        m_arg1( arg1 ),
        m_arg2( arg2 )
    {
    }

    ~LogicalNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_arg1->accept( visitor );
            m_arg2->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Operator m_op;
    Node* m_arg1;
    Node* m_arg2;
};

class ConditionalNode : public Node
{
public:
    ConditionalNode( Node* condition, Node* exp1, Node* exp2, int line ) : Node( Conditional, line ),
        m_condition( condition ),
        m_exp1( exp1 ),
        m_exp2( exp2 )
    {
    }

    ~ConditionalNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_condition->accept( visitor );
            m_exp1->accept( visitor );
            m_exp2->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Node* m_condition;
    Node* m_exp1;
    Node* m_exp2;
};

class CallNode : public Node
{
public:
    CallNode( MiscIdentifierPrivate* id, ListNode* args, int line ) : Node( Call, line ),
        m_id( id ),
        m_args( args )
    {
    }

    ~CallNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            if ( m_args != NULL )
                m_args->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    MiscIdentifierPrivate* m_id;
    ListNode* m_args;
};

class AssignmentNode : public Node
{
public:
    AssignmentNode( Operator op, Node* lvalue, Node* exp, int line ) : Node( Assignment, line ),
        m_op( op ),
        m_lvalue( lvalue ),
        m_exp( exp )
    {
    }

    ~AssignmentNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_lvalue->accept( visitor );
            m_exp->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Operator m_op;
    Node* m_lvalue;
    Node* m_exp;
};

class VoidNode : public Node
{
public:
    VoidNode( Node* exp, int line ) : Node( Void, line ),
        m_exp( exp )
    {
    }

    ~VoidNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_exp->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Node* m_exp;
};

class IfNode : public Node
{
public:
    IfNode( Node* condition, Node* stat1, Node* stat2, int line ) : Node( If, line ),
        m_condition( condition ),
        m_stat1( stat1 ),
        m_stat2( stat2 )
    {
    }

    ~IfNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_condition->accept( visitor );
            m_stat1->accept( visitor );
            if ( m_stat2 != NULL )
                m_stat2->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Node* m_condition;
    Node* m_stat1;
    Node* m_stat2;
};

class WhileNode : public Node
{
public:
    WhileNode( Node* condition, Node* stat, int line ) : Node( While, line ),
        m_condition( condition ),
        m_stat( stat )
    {
    }

    ~WhileNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_condition->accept( visitor );
            m_stat->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Node* m_condition;
    Node* m_stat;
};

class DoWhileNode : public Node
{
public:
    DoWhileNode( Node* stat, Node* condition, int line ) : Node( DoWhile, line ),
        m_stat( stat ),
        m_condition( condition )
    {
    }

    ~DoWhileNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_stat->accept( visitor );
            m_condition->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Node* m_stat;
    Node* m_condition;
};

class ForNode : public Node
{
public:
    ForNode( Node* stat1, Node* condition, Node* stat2, Node* stat3, int line ) : Node( For, line ),
        m_stat1( stat1 ),
        m_condition( condition ),
        m_stat2( stat2 ),
        m_stat3( stat3 )
    {
    }

    ~ForNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_stat1->accept( visitor );
            if ( m_condition != NULL )
                m_condition->accept( visitor );
            m_stat2->accept( visitor );
            m_stat3->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Node* m_stat1;
    Node* m_condition;
    Node* m_stat2;
    Node* m_stat3;
};

class ReturnNode : public Node
{
public:
    ReturnNode( Node* exp, int line ) : Node( Return, line ),
        m_exp( exp )
    {
    }

    ~ReturnNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            if ( m_exp != NULL )
                m_exp->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Node* m_exp;
};

class BreakNode : public Node
{
public:
    BreakNode( int line ) : Node( Break, line )
    {
    }

    ~BreakNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() )
            visitor->visit( this );
    }
};

class ContinueNode : public Node
{
public:
    ContinueNode( int line ) : Node( Continue, line )
    {
    }

    ~ContinueNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() )
            visitor->visit( this );
    }
};

class BlockNode : public Node
{
public:
    BlockNode( ListNode* list, int line ) : Node( Block, line ),
        m_list( list )
    {
    }

    ~BlockNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            if ( m_list != NULL )
                m_list->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    ListNode* m_list;
};

class SimpleNode : public Node
{
public:
    SimpleNode( ListNode* list, int line ) : Node( Simple, line ),
        m_list( list )
    {
    }

    ~SimpleNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            if ( m_list != NULL )
                m_list->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    ListNode* m_list;
};

class ArgumentNode : public Node
{
public:
    ArgumentNode( Misc::Type type, MiscIdentifierPrivate* id, int line ) : Node( Argument, line ),
        m_type( type ),
        m_id( id )
    {
    }

    ~ArgumentNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() )
            visitor->visit( this );
    }

public:
    Misc::Type m_type;
    MiscIdentifierPrivate* m_id;
};

class VariableNode : public Node
{
public:
    VariableNode( MiscIdentifierPrivate* id, Node* exp, int line ) : Node( Variable, line ),
        m_id( id ),
        m_exp( exp )
    {
    }

    ~VariableNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            if ( m_exp != NULL )
                m_exp->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    MiscIdentifierPrivate* m_id;
    Node* m_exp;
};

class DeclarationNode : public Node
{
public:
    DeclarationNode( Misc::SymbolFlags flags, Misc::Type type, ListNode* vars, int line ) : Node( Declaration, line ),
        m_flags( flags ),
        m_type( type ),
        m_vars( vars )
    {
    }

    ~DeclarationNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            m_vars->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Misc::SymbolFlags m_flags;
    Misc::Type m_type;
    ListNode* m_vars;
};

class FunctionNode : public Node
{
public:
    FunctionNode( Misc::Type type, MiscIdentifierPrivate* id, ListNode* args, Node* body, int line ) : Node( Function, line ),
        m_type( type ),
        m_id( id ),
        m_args( args ),
        m_body( body )
    {
    }

    ~FunctionNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            if ( m_args != NULL )
                m_args->accept( visitor );
            m_body->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    Misc::Type m_type;
    MiscIdentifierPrivate* m_id;
    ListNode* m_args;
    Node* m_body;
};

class UnitNode : public Node
{
public:
    UnitNode( ListNode* list, int line ) : Node( Unit, line ),
        m_list( list )
    {
    }

    ~UnitNode()
    {
    }

public:
    void accept( MiscVisitor* visitor )
    {
        if ( visitor->isValid() && visitor->enter( this ) ) {
            if ( m_list != NULL )
                m_list->accept( visitor );
            if ( visitor->isValid() )
                visitor->leave( this );
        }
    }

public:
    ListNode* m_list;
};

} }

#endif
