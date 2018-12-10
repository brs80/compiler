#include "token.hpp"

#include <iostream>

static char const*
str(Token::Name k) 
{
  switch (k) {
  case Token::eof: return "eof";
  case Token::lbrace: return "lbrace";
  case Token::rbrace: return "rbrace";
  case Token::lparen: return "lparen";
  case Token::rparen: return "rparen";
  case Token::colon: return "colon";
  case Token::semicolon: return "semicolon";
  case Token::comma: return "comma";
  case Token::arrow: return "arrow";

  case Token::plus: return "plus";
  case Token::minus: return "minus";
  case Token::star: return "star";
  case Token::slash: return "slash";
  case Token::percent: return "percent";
  case Token::question: return "question";
  case Token::equal: return "equal";
  case Token::equal_equal: return "equal_equal";
  case Token::bang_equal: return "bang_equal";
  case Token::less: return "less";
  case Token::greater: return "greater";
  case Token::less_equal: return "less_equal";
  case Token::greater_equal: return "greater_equal";

  case Token::and_kw: return "and_kw";
  case Token::bool_kw: return "bool_kw";
  case Token::break_kw: return "break_kw";
  case Token::continue_kw: return "continue_kw";
  case Token::else_kw: return "else_kw";
  case Token::false_kw: return "false_kw";
  case Token::fun_kw: return "fun_kw";
  case Token::if_kw: return "if_kw";
  case Token::int_kw: return "int_kw";
  case Token::not_kw: return "not_kw";
  case Token::or_kw: return "or_kw";
  case Token::ref_kw: return "ref_kw";
  case Token::return_kw: return "return_kw";
  case Token::true_kw: return "true_kw";
  case Token::var_kw: return "var_kw";
  case Token::while_kw: return "while_kw";

  case Token::integer_literal: return "integer_literal";
  case Token::float_literal: return "float_literal";
    
  case Token::identifier: return "identifier";
  }
}

std::ostream& 
operator<<(std::ostream& os, Token const& tok)
{
  return os << "<" << str(tok.get_name()) << ">";
}
