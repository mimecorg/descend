// This file was generated by qlalr - DO NOT EDIT!
#ifndef MISCGRAMMAR_P_H
#define MISCGRAMMAR_P_H

class MiscGrammar
{
public:
  enum VariousConstants {
    EOF_SYMBOL = 0,
    T_AND = 4,
    T_AND_AND = 5,
    T_AND_EQ = 6,
    T_BOOL = 42,
    T_BREAK = 41,
    T_COLON = 7,
    T_COMMA = 8,
    T_CONST = 43,
    T_CONTINUE = 44,
    T_DIVIDE = 9,
    T_DIVIDE_EQ = 10,
    T_DO = 45,
    T_DOT = 11,
    T_ELSE = 46,
    T_EQ = 12,
    T_EQ_EQ = 13,
    T_ERROR = 59,
    T_FALSE = 47,
    T_FLOAT = 48,
    T_FOR = 49,
    T_GT = 14,
    T_GT_EQ = 15,
    T_ID = 58,
    T_IF = 50,
    T_LBRACE = 16,
    T_LBRACKET = 17,
    T_LPAREN = 18,
    T_LT = 19,
    T_LT_EQ = 20,
    T_MATRIX = 51,
    T_MINUS = 21,
    T_MINUS_EQ = 22,
    T_MODULO = 23,
    T_MODULO_EQ = 24,
    T_MULTIPLY = 25,
    T_MULTIPLY_EQ = 26,
    T_NOT = 27,
    T_NOT_EQ = 28,
    T_NUMBER = 57,
    T_OR = 29,
    T_OR_EQ = 30,
    T_OR_OR = 31,
    T_PLUS = 32,
    T_PLUS_EQ = 33,
    T_POWER = 34,
    T_POWER_EQ = 35,
    T_QUESTION = 36,
    T_RBRACE = 37,
    T_RBRACKET = 38,
    T_RETURN = 52,
    T_RPAREN = 39,
    T_SEMICOLON = 40,
    T_START_EXPRESSION = 2,
    T_START_SYMBOL = 3,
    T_START_UNIT = 1,
    T_TRUE = 53,
    T_VEC = 54,
    T_VOID = 55,
    T_WHILE = 56,

    ACCEPT_STATE = 5,
    RULE_COUNT = 122,
    STATE_COUNT = 220,
    TERMINAL_COUNT = 60,
    NON_TERMINAL_COUNT = 48,

    GOTO_INDEX_OFFSET = 220,
    GOTO_INFO_OFFSET = 380,
    GOTO_CHECK_OFFSET = 380
  };

  static const char  *const    spell [];
  static const short             lhs [];
  static const short             rhs [];
  static const short    goto_default [];
  static const short  action_default [];
  static const short    action_index [];
  static const short     action_info [];
  static const short    action_check [];

  static inline int nt_action (int state, int nt)
  {
    const int yyn = action_index [GOTO_INDEX_OFFSET + state] + nt;
    if (yyn < 0 || action_check [GOTO_CHECK_OFFSET + yyn] != nt)
      return goto_default [nt];

    return action_info [GOTO_INFO_OFFSET + yyn];
  }

  static inline int t_action (int state, int token)
  {
    const int yyn = action_index [state] + token;

    if (yyn < 0 || action_check [yyn] != token)
      return - action_default [state];

    return action_info [yyn];
  }
};


#endif // MISCGRAMMAR_P_H

