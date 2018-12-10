#include "parser.hpp"
#include "scope.hpp"
#include "decl.hpp"

void
Parser::parse_program()
{
  //enter_scope();
  //parse_declaration_seq();
  //leave_scope();
}

Decl*
Parser::parse_local_declaration()
{
  return parse_object_definition();
}

/// object-definition -> var identifier : type = expression ;
Decl*
Parser::parse_object_definition()
{
  Token tok = require(Token::var_kw);
  Token id = expect(Token::identifier);
  Token colon = expect(Token::colon);
  Type* type = parse_type();

  Decl* var = m_act.on_object_declaration(id, type);

  Token equal = expect(Token::equal);
  Expr* init = parse_expression();
  Token semi = expect(Token::semicolon);

  //m_act.finish_object_definition(var, init);

  return var;
}
/// parameter-definition -> fun identifies (param-list)
std::vector<Decl*>
Parser::parse_parameter_declarations() 
{

}

/// function-definition -> fun identifier (parameter-list?) -> type block-statement
Decl*
Parser::parse_function_definition()
{
  Token tok = require(Token::fun_kw);
  Token id = expect(Token::identifier);
  
  Token lparen = expect(Token::lparen);
  m_act.enter_scope();

  std::vector<Decl*> parms;
  if (next_token_is_not(Token::rparen))
    parms = parse_parameter_declarations();
  Token rparen = expect(Token::rparen);

  Token arrow = expect(Token::arrow);
  Type* type = parse_type();

  Decl* fn = m_act.on_function_declaration(id, parms, type);

  //m_act.start_function_definition(fn, init);
  //Stmt* body = parse_statement();
  // m_act.finish_function_definition(fn, init);

  //m_act.leave_scope();

  //return var;
}
