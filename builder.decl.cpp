#include "builder.hpp"
#include "decl.hpp"

Var_decl*
Builder::make_variable(Name* n, Type* t)
{
  return new Var_decl(n, t);
}

Fn_decl*
Builder::make_function(Name* n, Type* t)
{
  return new Fn_decl(n, t);
}
