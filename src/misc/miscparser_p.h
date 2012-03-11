
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

class MiscParser : protected MiscGrammar
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

