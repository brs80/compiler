#include "generator.hpp"
#include "symbol.hpp"

#include <iostream>

Generator::Generator(Symbol_table& syms, 
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
Generator::consume()
{
  assert(!is_eof());
  Token ret = *m_lookahead;
  ++m_lookahead;
  return ret;
}

Token 
Generator::match(Token::Name n)
{
  if (next_token_is(n))
    return consume();
  return Token();
}

Token 
Generator::expect(Token::Name n)
{
  if (next_token_is(n))
    return consume();

  throw std::runtime_error("syntax error");
  return Token();
}

Token 
Generator::require(Token::Name n)
{
  assert(next_token_is(n));
  return consume();
}

void
Generator::parse_expression()
{
  parse_additive_expression();
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
void 
Generator::parse_additive_expression()
{
  parse_multiplicative_expression();

  // Step 4. Eliminate extra call.
  while (true) {
    if (match(Token::plus)) {
      parse_multiplicative_expression();
      std::cout << "add\n";
    }
    else if (match(Token::minus)) {
      parse_multiplicative_expression();
      std::cout << "sub\n";
    }
    else
      break;
  }
}

///   additive-expression-rest -> '+' multiplicative-expression additive-expression-rest
///   additive-expression-rest -> '-' multiplicative-expression additive-expression-rest
///   additive-expression-rest -> <empty>
void 
Generator::parse_additive_expression_rest()
{
  // Step3. Eliminated tail recursion.
  while (match(Token::plus) || match(Token::minus)) {
    parse_multiplicative_expression();
  }

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
void
Generator::parse_multiplicative_expression()
{
  parse_prefix_expression();
  parse_multiplicative_expression_rest();
}

void
Generator::parse_multiplicative_expression_rest()
{
  if (match(Token::star)) {
    parse_prefix_expression();
    std::cout << "mul\n";
    parse_multiplicative_expression_rest();
    return;
  }
  if (match(Token::slash)) {
    parse_prefix_expression();
    std::cout << "div\n";
    parse_multiplicative_expression_rest();
    return;
  }
  if (match(Token::percent)) {
    parse_prefix_expression();
    std::cout << "rem\n";
    parse_multiplicative_expression_rest();
    return;
  }
}

///   prefix-expression -> '-' prefix-expression
///   prefix-expression -> '/' prefix-expression
///   prefix-expression -> postfix-expression
void
Generator::parse_prefix_expression()
{
  if (match(Token::minus)) {
    std::cout << "push 0\n"; // Emits 0 - n
    parse_prefix_expression();
    std::cout << "sub\n";
    return;
  }
  if (match(Token::slash)) {
    std::cout << "push 1\n"; // Emits 1 / n
    parse_prefix_expression();
    std::cout << "div\n";
    return;
  }
  parse_postfix_expression();
}

///   postfix-expression -> postfix-expression '(' argument-list ')'
///   postfix-expression -> primary-expression
void
Generator::parse_postfix_expression()
{
  parse_primary_expression();
  parse_postfix_expression_rest();
}

void
Generator::parse_postfix_expression_rest()
{
  if (match(Token::lparen)) {
    assert(false);
    // parse_argument_list();
    expect(Token::rparen);
    parse_postfix_expression_rest();
    return;
  }
}

/// Parse a factor
///
  ///   primary-expression -> integer-literal
  ///   primary-expression -> float-literal
  ///   primary-expression -> identifier
  ///   primary-expression -> '(' factor ')'
void
Generator::parse_primary_expression()
{
  if (Token tok = match(Token::integer_literal)) {
    std::cout << "push " << tok.get_lexeme().str() << '\n';
    return;
  }

  if (match(Token::identifier)) {
    assert(false);
    return;
  }

  if (match(Token::lparen)) {
    parse_expression();
    expect(Token::rparen);
    return;
  }

  throw std::runtime_error("expected factor");
}


