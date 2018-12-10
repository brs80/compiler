#include "parser.hpp"

#include <iostream>

Expr*
Parser::parse_expression()
{
  return parse_assignment_expression();
}

/// Parse an assignment expression.
///
///   assignment-expression -> additive-expression '=' assignment-expression
///                          | additive-expression
Expr*
Parser::parse_assignment_expression()
{
  Expr *expr = parse_additive_expression();
  if (match(Token::equal))
    return parse_assignment_expression();
  return expr;
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
Expr* 
Parser::parse_additive_expression()
{
  parse_multiplicative_expression();

  // Step 4. Eliminate extra call.
  while (match(Token::plus) || match(Token::minus))
    parse_multiplicative_expression();
}

///   multiplicative-expression -> multiplicative-expression '*' postfix-expression
///   multiplicative-expression -> multiplicative-expression '/' postfix-expression
///   multiplicative-expression -> multiplicative-expression '%' postfix-expression
///   multiplicative-expression -> prefix-expression
Expr*
Parser::parse_multiplicative_expression()
{
  Expr* lhs = parse_prefix_expression();  
  while (true) {
    if (Token star = match(Token::star)) {      
      Expr* rhs = parse_prefix_expression();
      lhs = m_act.on_multiplication_expression(lhs, rhs);
    }
    if (Token slash = match(Token::slash)) {      
      Expr* rhs = parse_prefix_expression();
      lhs = m_act.on_division_expression(lhs, rhs);
    }
    if (Token percent = match(Token::percent)) {      
      Expr* rhs = parse_prefix_expression();
      lhs = m_act.on_remainder_expression(lhs, rhs);
    }
  }
}


///   prefix-expression -> '-' prefix-expression
///   prefix-expression -> '/' prefix-expression
///   prefix-expression -> postfix-expression
Expr*
Parser::parse_prefix_expression()
{
  if (Token op = match(Token::minus)) {
    Expr* arg = parse_prefix_expression();
    return m_act.on_negation_expression(arg);
  }
  if (Token op = match(Token::slash)) {
    Expr* arg = parse_prefix_expression();
    return m_act.on_reciprocal_expression(arg);
  }
  parse_postfix_expression();
}

///   postfix-expression -> postfix-expression '(' argument-list ')'
///   postfix-expression -> primary-expression
Expr*
Parser::parse_postfix_expression()
{
  parse_primary_expression();
  while (true) {
    if (match(Token::lparen)) {
      // parse_argument_list();
      expect(Token::rparen);
      // return nullptr;
    }
    else {
      break;
    }
  }
  return nullptr;
}

/// Parse a primary-expression.
///
///   primary-expression -> integer-literal
///   primary-expression -> float-literal
///   primary-expression -> identifier
///   primary-expression -> '(' expression ')'
Expr*
Parser::parse_primary_expression()
{
  if (Token tok = match(Token::integer_literal))
    return m_act.on_integer_literal(tok);

  if (Token tok = match(Token::identifier))
    return m_act.on_id_expression(tok);

  if (match(Token::lparen)) {
    Expr* expr = parse_expression();
    expect(Token::rparen);
    return expr;
  }

  throw std::runtime_error("expected factor");
}


