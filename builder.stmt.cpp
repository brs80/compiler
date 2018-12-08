#include "builder.hpp"
#include "decl.hpp"
#include "stmt.hpp"

Stmt*
Builder::make_skip()
{
  return new Skip_stmt();
}

Stmt*
Builder::make_block(std::initializer_list<Stmt*> ss)
{
  return new Block_stmt(ss);
}

Stmt*
Builder::make_block(std::vector<Stmt*> const& ss)
{
  return new Block_stmt(ss);
}

Stmt*
Builder::make_if(Expr* e, Stmt* s1, Stmt* s2)
{
  e = require_bool(e);
  return new If_stmt(e, s1, s2);
}

Stmt*
Builder::make_while(Expr* e, Stmt* s1)
{
  e = require_bool(e);
  return new While_stmt(e, s1);
}

Stmt*
Builder::make_break()
{
  return new Break_stmt();
}

Stmt*
Builder::make_continue()
{
  return new Cont_stmt();
}

Stmt*
Builder::make_return(Decl* d, Expr* e)
{
  assert(d->is_variable());
  Var_decl* var = static_cast<Var_decl*>(d);
  
  copy_initialize(var, e);
  
  return new Ret_stmt(var->get_initializer());
}

Stmt*
Builder::make_expression(Expr* e)
{
  return new Expr_stmt(e);
}

Stmt*
Builder::make_declaration(Decl* d)
{
  return new Decl_stmt(d);
}


