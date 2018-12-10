#include "calculator.hpp"

#include <iostream>
#include <string>

Calculator::Calculator(Symbol_table& syms, 
               std::string const& input)
  : m_lex(syms, input)
{
  // Pull all of the tokens in one shot.
  while (Token tok = m_lex.get_next_token())
    m_toks.push_back(tok);

  m_lookahead = m_toks.data();
  m_last = m_lookahead + m_toks.size();
}

Token 
Calculator::consume()
{
  assert(!is_eof());
  Token ret = *m_lookahead;
  ++m_lookahead;
  return ret;
}

Token 
Calculator::match(Token::Name n)
{
  if (next_token_is(n))
    return consume();
  return Token();
}

Token 
Calculator::expect(Token::Name n)
{
  if (next_token_is(n))
    return consume();

  throw std::runtime_error("syntax error");
  return Token();
}

Token 
Calculator::require(Token::Name n)
{
  assert(next_token_is(n));
  return consume();
}

int
Calculator::parse_expression()
{
  return parse_additive_expression();
}

/// Parse an expression.
///
///   additive-expression -> additive-expression '+' multiplicative-expression
///   additive-expression -> additive-expression '-' multiplicative-expression
///   additive-expression -> multiplicative-expression
///
/// Refactored:
///
///   additive-expression -> multiplicative-expression additive-expression-rest
int 
Calculator::parse_additive_expression()
{
  int lhs = parse_multiplicative_expression();

  // Step 4. Eliminate extra call.
  while (true) {
    if (match(Token::plus)) {
      int rhs = parse_multiplicative_expression();
      lhs += rhs;
    }
    else if (match(Token::minus)) {
      int rhs = parse_multiplicative_expression();
      lhs -= rhs;
    }
    else
      break;
  }

  return lhs;
}

///   additive-expression-rest -> '+' multiplicative-expression additive-expression-rest
///   additive-expression-rest -> '-' multiplicative-expression additive-expression-rest
///   additive-expression-rest -> <empty>
int 
Calculator::parse_additive_expression_rest(int)
{
  // Step3. Eliminated tail recursion.
  while (match(Token::plus) || match(Token::minus)) {
    parse_multiplicative_expression();
  }

  return 0;

  // Step 2. Make tail recursive.
  // if (!match(Token::plus) && !match(Token::minus))
  //   return;

  // parse_multiplicative_expression();
  // parse_additive_expression_rest();

  // Step 1.
  // if (match(Token::plus) || match(Token::minus)) {
  //   parse_multiplicative_expression();
  //   parse_additive_expression_rest();
  // }

  // Step 0.
  // if (match(Token::plus)) {
  //   parse_multiplicative_expression();
  //   parse_additive_expression_rest();
  //   return;
  // }
  // if (match(Token::minus)) {
  //   parse_multiplicative_expression();
  //   parse_additive_expression_rest();
  //   return;
  // }
}

///   multiplicative-expression -> multiplicative-expression '*' postfix-expression
///   multiplicative-expression -> multiplicative-expression '/' postfix-expression
///   multiplicative-expression -> multiplicative-expression '%' postfix-expression
///   multiplicative-expression -> prefix-expression
int
Calculator::parse_multiplicative_expression()
{
  int lhs = parse_prefix_expression();
  return parse_multiplicative_expression_rest(lhs);
}

int
Calculator::parse_multiplicative_expression_rest(int lhs)
{
  if (match(Token::star)) {
    int rhs = parse_prefix_expression();
    return parse_multiplicative_expression_rest(lhs * rhs);
  }
  if (match(Token::slash)) {
    int rhs = parse_prefix_expression();
    return parse_multiplicative_expression_rest(lhs / rhs);
  }
  if (match(Token::percent)) {
    int rhs = parse_prefix_expression();
    return parse_multiplicative_expression_rest(lhs % rhs);
  }
  return lhs;
}

///   prefix-expression -> '-' prefix-expression
///   prefix-expression -> '/' prefix-expression
///   prefix-expression -> postfix-expression
int
Calculator::parse_prefix_expression()
{
  if (match(Token::minus)) {
    int n = parse_prefix_expression();
    return -n;
  }
  if (match(Token::slash)) {
    int n = parse_prefix_expression();
    return 1 / n;
  }
  return parse_postfix_expression();
}

///   postfix-expression -> postfix-expression '(' argument-list ')'
///   postfix-expression -> primary-expression
int
Calculator::parse_postfix_expression()
{
  int lhs = parse_primary_expression();
  return parse_postfix_expression_rest(lhs);
}

int
Calculator::parse_postfix_expression_rest(int lhs)
{
  if (match(Token::lparen)) {
    assert(false);
    // parse_argument_list();
    expect(Token::rparen);
    parse_postfix_expression_rest(lhs);
    return 0;
  }
  return lhs;
}

/// Parse a factor
///
  ///   primary-expression -> integer-literal
  ///   primary-expression -> float-literal
  ///   primary-expression -> identifier
  ///   primary-expression -> '(' factor ')'
int
Calculator::parse_primary_expression()
{
  if (Token tok = match(Token::integer_literal)) {
    return std::stoi(tok.get_lexeme().str());
  }

  if (Token tok = match(Token::identifier)) {
    assert(false);
    return 0;
  }

  if (match(Token::lparen)) {
    int n = parse_expression();
    expect(Token::rparen);
    return n;
  }

  throw std::runtime_error("expected factor");
}


