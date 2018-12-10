#include "parser.hpp"
#include "stmt.hpp"

#include <iostream>

Stmt*
Parser::parse_statement()
{
  switch (lookahead()) {
  case Token::semicolon:
    return parse_empty_statement();

  case Token::lbrace:
    return parse_block_statement();

  case Token::if_kw:
    return parse_if_statement();

  case Token::while_kw:
    return parse_while_statement();
  
  case Token::break_kw:
    return parse_break_statement();

  case Token::continue_kw:
    return parse_continue_statement();
  
  case Token::var_kw:
  case Token::ref_kw:
    return parse_declaration_statement();

  default:
    return parse_expression_statement();
  }
}

Stmt*
Parser::parse_empty_statement()
{
  require(Token::semicolon);
  return new Skip_stmt();
}

Stmt*
Parser::parse_block_statement()
{
  require(Token::lbrace);

  std::vector<Stmt*> ss;
  while (next_token_is_not(Token::rbrace)) {
    Stmt * s = parse_statement();
    ss.push_back(s);
  }

  expect(Token::rbrace);
  return new Block_stmt(std::move(ss));
}

Stmt*
Parser::parse_if_statement()
{
  require(Token::if_kw);
  expect(Token::lparen);
  Expr *cond = parse_expression();
  expect(Token::rparen);
  Stmt* ts = parse_statement();
  expect(Token::else_kw);
  Stmt* fs = parse_statement();
  return new If_stmt(cond, ts, fs);
}

Stmt*
Parser::parse_while_statement()
{
  require(Token::while_kw);
  expect(Token::lparen);
  Expr *cond = parse_expression();
  expect(Token::rparen);
  Stmt* body = parse_statement();
  return new While_stmt(cond, body);
}

Stmt*
Parser::parse_break_statement()
{
  require(Token::break_kw);
  expect(Token::semicolon);
  return new Break_stmt();
}

Stmt*
Parser::parse_continue_statement()
{
  require(Token::continue_kw);
  expect(Token::semicolon);
  return new Cont_stmt();
}

Stmt*
Parser::parse_return_statement()
{
  require(Token::return_kw);
  Expr* ret = parse_expression();
  expect(Token::semicolon);
  return new Ret_stmt(ret);
}

Stmt*
Parser::parse_declaration_statement()
{
  Decl* d = parse_local_declaration();
  return new Decl_stmt(d);
}

Stmt*
Parser::parse_expression_statement()
{
  Expr *expr = parse_expression();
  expect(Token::semicolon);
  return new Expr_stmt(expr);
}

