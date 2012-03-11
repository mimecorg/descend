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

#include "miscprintvisitor_p.h"

#if defined( MISC_DEBUG )

#include "miscidentifier_p.h"

#include <QTextStream>

MiscPrintVisitor::MiscPrintVisitor( QTextStream& stream ) :
    m_stream( stream ),
    m_indent( 0 )
{
}

MiscPrintVisitor::~MiscPrintVisitor()
{
}

bool MiscPrintVisitor::enter( Misc::AST::ListNode* /*node*/ )
{
    indent();
    m_stream << "List" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::ListNode* /*node*/ )
{
    leave();
}

void MiscPrintVisitor::visit( Misc::AST::IdNode* node )
{
    indent();
    m_stream << "Id ( '" << node->m_id->name() << "' )" << endl;
}

void MiscPrintVisitor::visit( Misc::AST::SubscriptNode* node )
{
    indent();
    m_stream << "Subscript ( '" << node->m_id->name() << "' . '" << node->m_component->name() << "' )" << endl;
}

void MiscPrintVisitor::visit( Misc::AST::BooleanNode* node )
{
    indent();
    m_stream << "Boolean ( " << ( node->m_bool ? "true" : "false" ) << " )" << endl;
}

void MiscPrintVisitor::visit( Misc::AST::FloatNode* node )
{
    indent();
    m_stream << "Float ( " << node->m_float << " )" << endl;
}

bool MiscPrintVisitor::enter( Misc::AST::VectorNode* /*node*/ )
{
    indent();
    m_stream << "Vector" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::VectorNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::MatrixNode* /*node*/ )
{
    indent();
    m_stream << "Matrix" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::MatrixNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::UnaryNode* node )
{
    indent();
    m_stream << "Unary ( ";
    print( node->m_op );
    m_stream << " )" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::UnaryNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::BinaryNode* node )
{
    indent();
    m_stream << "Binary ( ";
    print( node->m_op );
    m_stream << " )" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::BinaryNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::LogicalNode* node )
{
    indent();
    m_stream << "Logical ( ";
    print( node->m_op );
    m_stream << " )" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::LogicalNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::ConditionalNode* /*node*/ )
{
    indent();
    m_stream << "Conditional" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::ConditionalNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::CallNode* node )
{
    indent();
    m_stream << "Call ( '" << node->m_id->name() << "' )" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::CallNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::AssignmentNode* node )
{
    indent();
    m_stream << "Assignment ( ";
    print( node->m_op );
    m_stream << " )" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::AssignmentNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::VoidNode* /*node*/ )
{
    indent();
    m_stream << "Void" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::VoidNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::IfNode* /*node*/ )
{
    indent();
    m_stream << "If" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::IfNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::WhileNode* /*node*/ )
{
    indent();
    m_stream << "While" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::WhileNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::DoWhileNode* /*node*/ )
{
    indent();
    m_stream << "DoWhile" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::DoWhileNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::ForNode* /*node*/ )
{
    indent();
    m_stream << "For" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::ForNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::ReturnNode* /*node*/ )
{
    indent();
    m_stream << "Return" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::ReturnNode* /*node*/ )
{
    leave();
}

void MiscPrintVisitor::visit( Misc::AST::BreakNode* /*node*/ )
{
    indent();
    m_stream << "Break" << endl;
}

void MiscPrintVisitor::visit( Misc::AST::ContinueNode* /*node*/ )
{
    indent();
    m_stream << "Continue" << endl;
}

bool MiscPrintVisitor::enter( Misc::AST::BlockNode* /*node*/ )
{
    indent();
    m_stream << "Block" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::BlockNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::SimpleNode* /*node*/ )
{
    indent();
    m_stream << "Simple" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::SimpleNode* /*node*/ )
{
    leave();
}

void MiscPrintVisitor::visit( Misc::AST::ArgumentNode* node )
{
    indent();
    m_stream << "Argument ( ";
    print( node->m_type );
    m_stream << " '" << node->m_id->name() << "' )" << endl;
}

bool MiscPrintVisitor::enter( Misc::AST::VariableNode* node )
{
    indent();
    m_stream << "Variable ( '" << node->m_id->name() << "' )" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::VariableNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::DeclarationNode* node )
{
    indent();
    m_stream << "Declaration ( ";
    if ( node->m_flags & Misc::ConstantSymbol )
        m_stream << "const ";
    print( node->m_type );
    m_stream << " )" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::DeclarationNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::FunctionNode* node )
{
    indent();
    m_stream << "Function ( ";
    print( node->m_type );
    m_stream << " '" << node->m_id->name() << "' )" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::FunctionNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter( Misc::AST::UnitNode* /*node*/ )
{
    indent();
    m_stream << "Unit" << endl;
    return enter();
}

void MiscPrintVisitor::leave( Misc::AST::UnitNode* /*node*/ )
{
    leave();
}

bool MiscPrintVisitor::enter()
{
    indent();
    m_stream << "[" << endl;
    m_indent++;
    return true;
}

void MiscPrintVisitor::leave()
{
    m_indent--;
    indent();
    m_stream << "]" << endl;
}

void MiscPrintVisitor::indent()
{
    for ( int i = 0; i < m_indent; i++ )
        m_stream << "    ";
}

static const char* const type_names[] =
{
    "void", "bool", "float", "vec", "matrix"
};

void MiscPrintVisitor::print( Misc::Type type )
{
    m_stream << type_names[ type - 1 ];
}

void MiscPrintVisitor::print( Misc::AST::Operator op )
{
    m_stream << operatorName( op );
}

#endif // defined( MISC_DEBUG )
