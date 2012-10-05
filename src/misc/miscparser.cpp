
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

                case 0:
                    m_result = new ( m_pool ) Misc::AST::UnitNode( sym( 2 ).m_list, line() );
                    break;

                case 1:
                    m_result = sym( 2 ).m_node;
                    break;

                case 2:
                    m_result = sym( 2 ).m_node;
                    break;

                case 4:
                    sym( 1 ).m_node = NULL;
                    break;

                case 5:
                    sym( 1 ).m_list->append( sym( 2 ).m_node );
                    break;

                case 6:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;

                case 10:
                    sym( 1 ).m_node = NULL;
                    break;

                case 11:
                    if ( sym( 2 ).m_node )
                        sym( 1 ).m_list->append( sym( 2 ).m_node );
                    break;

                case 12:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;

                case 16:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::IfNode( sym( 3 ).m_node, sym( 5 ).m_node, NULL, line() );
                    break;

                case 17:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::IfNode( sym( 3 ).m_node, sym( 5 ).m_node, sym( 7 ).m_node, line() );
                    break;

                case 18:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::WhileNode( sym( 3 ).m_node, sym( 5 ).m_node, line() );
                    break;

                case 19:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DoWhileNode( sym( 2 ).m_node, sym( 5 ).m_node, line() );
                    break;

                case 20:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::ForNode( sym( 3 ).m_node, sym( 5 ).m_node, sym( 7 ).m_node, sym( 9 ).m_node, line() );
                    break;

                case 21:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::ReturnNode( sym( 2 ).m_node, line() );
                    break;

                case 22:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BreakNode( line() );
                    break;

                case 23:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::ContinueNode( line() );
                    break;

                case 24:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BlockNode( sym( 2 ).m_list, line() );
                    break;

                case 25:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::FunctionNode( sym( 1 ).m_type, sym( 2 ).m_id, sym( 4 ).m_list, sym( 6 ).m_node, line() );
                    break;

                case 26:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::FunctionNode( sym( 1 ).m_type, sym( 2 ).m_id, sym( 4 ).m_list, sym( 7 ).m_node, line() );
                    break;

                case 28:
                    sym( 1 ).m_node = NULL;
                    break;

                case 29:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;

                case 30:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;

                case 31:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::ArgumentNode( sym( 1 ).m_type, sym( 2 ).m_id, line() );
                    break;

                case 32:
                    sym( 1 ).m_type = Misc::VoidType;
                    break;

                case 33:
                    sym( 1 ).m_type = Misc::BoolType;
                    break;

                case 34:
                    sym( 1 ).m_type = Misc::FloatType;
                    break;

                case 35:
                    sym( 1 ).m_type = Misc::VectorType;
                    break;

                case 36:
                    sym( 1 ).m_type = Misc::MatrixType;
                    break;

                case 37:
                    sym( 1 ).m_id = lexer->id();
                    break;

                case 38:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DeclarationNode( 0, sym( 1 ).m_type, sym( 2 ).m_list, line() );
                    break;

                case 39:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DeclarationNode( Misc::ConstantSymbol, sym( 2 ).m_type, sym( 3 ).m_list, line() );
                    break;

                case 40:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;

                case 41:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;

                case 43:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::VariableNode( sym( 1 ).m_id, NULL, line() );
                    break;

                case 44:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;

                case 45:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;

                case 46:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::VariableNode( sym( 1 ).m_id, sym( 3 ).m_node, line() );
                    break;

                case 48:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DeclarationNode( 0, sym( 1 ).m_type, sym( 2 ).m_list, line() );
                    break;

                case 49:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::SimpleNode( sym( 1 ).m_list, line() );
                    break;

                case 51:
                    sym( 1 ).m_node = NULL;
                    break;

                case 52:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;

                case 53:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;

                case 54:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Eq, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 55:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Plus, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 56:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Minus, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 57:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Multiply, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 58:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Divide, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 59:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Modulo, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 60:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Power, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 61:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::And, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 62:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::AssignmentNode( Misc::AST::Or, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 63:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::VoidNode( sym( 1 ).m_node, line() );
                    break;

                case 65:
                    sym( 1 ).m_node = NULL;
                    break;

                case 67:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::ConditionalNode( sym( 1 ).m_node, sym( 3 ).m_node, sym( 5 ).m_node, line() );
                    break;

                case 69:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::LogicalNode( Misc::AST::OrOr, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 71:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::LogicalNode( Misc::AST::AndAnd, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 73:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Or, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 75:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::And, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 77:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::EqEq, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 78:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::NotEq, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 80:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Gt, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 81:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::GtEq, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 82:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Lt, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 83:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::LtEq, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 85:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Plus, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 86:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Minus, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 88:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Multiply, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 89:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Divide, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 90:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Modulo, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 92:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BinaryNode( Misc::AST::Power, sym( 1 ).m_node, sym( 3 ).m_node, line() );
                    break;

                case 94:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::UnaryNode( Misc::AST::Plus, sym( 2 ).m_node, line() );
                    break;

                case 95:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::UnaryNode( Misc::AST::Minus, sym( 2 ).m_node, line() );
                    break;

                case 96:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::UnaryNode( Misc::AST::Not, sym( 2 ).m_node, line() );
                    break;

                case 98:
                    sym( 1 ).m_node = sym( 2 ).m_node;
                    break;

                case 99:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::VectorNode( sym( 2 ).m_list, line() );
                    break;

                case 100:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::MatrixNode( sym( 2 ).m_list, line() );
                    break;

                case 101:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::FloatNode( lexer->number(), line() );
                    break;

                case 102:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BooleanNode( true, line() );
                    break;

                case 103:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::BooleanNode( false, line() );
                    break;

                case 106:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::IdNode( sym( 1 ).m_id, line() );
                    break;

                case 107:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::SubscriptNode( sym( 1 ).m_id, sym( 3 ).m_id, line() );
                    break;

                case 108:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::CallNode( sym( 1 ).m_id, sym( 3 ).m_list, line() );
                    break;

                case 110:
                    sym( 1 ).m_node = NULL;
                    break;

                case 111:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;

                case 112:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;

                case 114:
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;

                case 115:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    sym( 1 ).m_list->append( sym( 3 ).m_node );
                    break;

                case 116:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DeclarationNode( 0, sym( 1 ).m_type, sym( 2 ).m_list, line() );
                    break;

                case 117:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::DeclarationNode( Misc::ConstantSymbol, sym( 2 ).m_type, sym( 3 ).m_list, line() );
                    break;

                case 118:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::FunctionNode( sym( 1 ).m_type, sym( 2 ).m_id, sym( 4 ).m_list, NULL, line() );
                    break;

                case 119:
                    sym( 1 ).m_list = new ( m_pool ) Misc::AST::ListNode( sym( 1 ).m_node, line() );
                    break;

                case 120:
                    sym( 1 ).m_node = new ( m_pool ) Misc::AST::VariableNode( sym( 1 ).m_id, NULL, line() );
                    break;

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

