#pragma once

#include "token.hpp"
#include "lexer.hpp"

#include <cassert>
#include <vector>

class Symbol_table;

/// The parser is responsible for determining
/// if a string (read from the lexer) can be
/// derived from the grammar of the language.
///
/// It's going to do a lot more than this later.
class Generator
{
public:
  Generator(Symbol_table& syms, std::string const& input);


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

  void parse_expression();

  void parse_additive_expression();
  void parse_additive_expression_rest();
  /// Parse an additive-expression.
  ///
  ///   additive-expression -> additive-expression '+' multiplicative-expression
  ///   additive-expression -> additive-expression '-' multiplicative-expression
  ///   additive-expression -> multiplicative-expression
  
  void parse_multiplicative_expression();
  void parse_multiplicative_expression_rest();
  /// Parse a multiplicative-expression.
  ///
  ///   multiplicative-expression -> multiplicative-expression '*' postfix-expression
  ///   multiplicative-expression -> multiplicative-expression '/' postfix-expression
  ///   multiplicative-expression -> multiplicative-expression '%' postfix-expression
  ///   multiplicative-expression -> prefix-expression

  void parse_prefix_expression();
  /// Parse a prefix-expression.
  ///
  ///   prefix-expression -> '-' prefix-expression
  ///   prefix-expression -> '/' prefix-expression
  ///   prefix-expression -> postfix-expression

  void parse_postfix_expression();
  void parse_postfix_expression_rest();
  /// Parse a postfix expression
  ///
  ///   postfix-expression -> primary-expression '(' argument-list ')'
  ///   postfix-expression -> primary-expression

  void parse_primary_expression();
  /// Parse a factor
  ///
  ///   primary-expression -> integer-literal
  ///   primary-expression -> float-literal
  ///   primary-expression -> identifier
  ///   primary-expression -> '(' factor ')'

private:
  Lexer m_lex;
  // The lexer.

  std::vector<Token> m_toks;
  // Holds the "string" being parsed.

  Token* m_lookahead;
  /// The index of the current token.

  Token* m_last;
  /// Points past the end of the token buffer.
};



