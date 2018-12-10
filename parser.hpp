#pragma once

#include "token.hpp"
#include "lexer.hpp"
#include "actions.hpp"
#include "expr.hpp"
#include "calculator.hpp"
#include "generator.hpp"

#include <cassert>
#include <vector>

class Symbol_table;
class Type;
class Expr;
class Stmt;
class Decl;

class Parser
{
public:
  Parser(Symbol_table& syms, std::string const& input);

private:
  // Helper functions

  bool is_eof() const { return m_lookahead == m_last; }
  /// True if at end of file.

  const Token& peek() const { return *m_lookahead; }
  /// Peeks at the lookahead token.

  Token::Name lookahead() const { return peek().get_name(); }
  /// Returns the name of the lookahead token.

  bool next_token_is(Token::Name n) const { return lookahead() == n; }
  /// True if the lookahead is n.

  bool next_token_is_not(Token::Name n) { return lookahead() != n; }
  /// True if the lookahead is not n.

  Token consume();
  /// Advance to the next token.

  Token match(Token::Name n);
  /// If lookahead is n, consume. Otherwise return EoF.

  Token expect(Token::Name n);
  /// If lookahead is n, consume. Otherwise error.

  Token require(Token::Name n);
  /// If lookahead is n, consume. Otherwise UB.


public:
  // Actual parsing.

  void parse_program();

  Expr* parse_expression();

  Expr* parse_assignment_expression();

  Expr* parse_additive_expression();
  /// Parse an additive-expression.
  ///
  ///   additive-expression -> additive-expression '+' multiplicative-expression
  ///   additive-expression -> additive-expression '-' multiplicative-expression
  ///   additive-expression -> multiplicative-expression
  
  Expr* parse_multiplicative_expression();
  /// Parse a multiplicative-expression.
  ///
  ///   multiplicative-expression -> multiplicative-expression '*' postfix-expression
  ///   multiplicative-expression -> multiplicative-expression '/' postfix-expression
  ///   multiplicative-expression -> multiplicative-expression '%' postfix-expression
  ///   multiplicative-expression -> prefix-expression

  Expr* parse_prefix_expression();
  /// Parse a prefix-expression.
  ///
  ///   prefix-expression -> '-' prefix-expression
  ///   prefix-expression -> '/' prefix-expression
  ///   prefix-expression -> postfix-expression

  Expr* parse_postfix_expression();
  /// Parse a postfix expression
  ///
  ///   postfix-expression -> primary-expression '(' argument-list ')'
  ///   postfix-expression -> primary-expression

  Expr* parse_primary_expression();
  /// Parse a factor
  ///
  ///   primary-expression -> integer-literal
  ///   primary-expression -> float-literal
  ///   primary-expression -> identifier
  ///   primary-expression -> '(' factor ')'

  // Statement parsing

  Stmt* parse_statement();
  Stmt* parse_empty_statement();
  Stmt* parse_block_statement();
  Stmt* parse_if_statement();
  Stmt* parse_while_statement();
  Stmt* parse_break_statement();
  Stmt* parse_continue_statement();
  Stmt* parse_return_statement();
  Stmt* parse_declaration_statement();
  Stmt* parse_expression_statement();

  // Types

  Type* parse_type();

  // Declarations

  Decl* parse_declaration();
  Decl* parse_local_declaration();
  Decl* parse_function_definition();
  Decl* parse_object_definition();

private:
  Lexer m_lex;
  // The lexer.

  std::vector<Token> m_toks;
  // Holds the "string" being parsed.

  Token* m_lookahead;
  /// The index of the current token.

  Token* m_last;
  /// Points past the end of the token buffer.

  Actions m_act;
  /// The semantic actions for the parser.
};

