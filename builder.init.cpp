#include "builder.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "decl.hpp"

void
Builder::copy_initialize(Decl* d, Expr* e)
{
  assert(d->is_variable());
  Var_decl* var = static_cast<Var_decl*>(d);

  if (d->is_reference())
    return reference_initialize(d, e);

  e = require_type(e, d->get_type());

  var->set_initializer(e);
}

void
Builder::reference_initialize(Decl* d, Expr* e)
{
  assert(d->is_variable());
  Var_decl* var = static_cast<Var_decl*>(d);

  e = require_type(e, d->get_type());

  var->set_initializer(e);
}

