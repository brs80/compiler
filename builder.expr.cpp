#include "builder.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "decl.hpp"

#include <iostream>

Expr*
Builder::make_bool(bool b)
{
  return new Bool_expr(get_bool_type(), Value(b));
}

Expr*
Builder::make_true()
{
  return make_bool(true);
}

Expr*
Builder::make_false()
{
  return make_bool(false);
}

Expr*
Builder::make_int(int n)
{
  return new Int_expr(get_int_type(), Value(n));
}

Expr*
Builder::make_and(Expr* e1, Expr* e2)
{
  e1 = require_bool(e1);
  e2 = require_bool(e2);
  return new And_expr(e1->get_type(), e1, e2);
}

Expr*
Builder::make_or(Expr* e1, Expr* e2)
{
  e1 = require_bool(e1);
  e2 = require_bool(e2);
  return new Or_expr(e1->get_type(), e1, e2);
}

Expr*
Builder::make_not(Expr* e1)
{
  e1 = require_bool(e1);
  return new Not_expr(e1->get_type(), e1);
}

Expr*
Builder::make_conditional(Expr* e1, Expr* e2, Expr* e3)
{
  e1 = require_bool(e1);
  std::tie(e2, e3) = require_common(e2, e3);
  return new Cond_expr(e2->get_type(), e1, e2, e3);
}

Expr*
Builder::make_id(Decl* d)
{
  Type* t;
  if (d->is_object() || d->is_reference())
    t = get_reference_type(d->get_type());
  else if (d->is_function())
    t = d->get_type();
  else
    throw std::logic_error("invalid id-expression");

  return new Id_expr(t, d);
}

Expr*
Builder::make_eq(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_value(e1, e2);
  return new Eq_expr(get_bool_type(), e1, e2);
}

Expr*
Builder::make_ne(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_value(e1, e2);
  return new Ne_expr(get_bool_type(), e1, e2);
}

Expr*
Builder::make_lt(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_value(e1, e2);
  return new Lt_expr(get_bool_type(), e1, e2);
}

Expr*
Builder::make_gt(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_value(e1, e2);
  return new Gt_expr(get_bool_type(), e1, e2);
}

Expr*
Builder::make_le(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_value(e1, e2);
  return new Le_expr(get_bool_type(), e1, e2);
}

Expr*
Builder::make_ge(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_value(e1, e2);
  return new Ge_expr(get_bool_type(), e1, e2);
}

Expr*
Builder::make_add(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_arithmetic(e1, e2);
  return new Add_expr(e1->get_type(), e1, e2);
}

Expr*
Builder::make_sub(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_arithmetic(e1, e2);
  return new Sub_expr(e1->get_type(), e1, e2);
}

Expr*
Builder::make_mul(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_arithmetic(e1, e2);
  return new Mul_expr(e1->get_type(), e1, e2);
}

Expr*
Builder::make_div(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_arithmetic(e1, e2);
  return new Div_expr(e1->get_type(), e1, e2);
}

Expr*
Builder::make_rem(Expr* e1, Expr* e2)
{
  std::tie(e1, e2) = require_same_arithmetic(e1, e2);
  return new Rem_expr(e1->get_type(), e1, e2);
}

Expr*
Builder::make_neg(Expr* e1)
{
  e1 = require_arithmetic(e1);
  return new Neg_expr(e1->get_type(), e1);
}

Expr*
Builder::make_assign(Expr* e1, Expr* e2)
{
  e2 = convert_to_value(e2);
  e1 = require_reference_to(e1, e2->get_type());
  return new Assign_expr(e1->get_type(), e1, e2);
}

// FIXME: Check arguments.
Expr*
Builder::make_call(std::vector<Expr*> const& es)
{
  // Create a new vector to store the converted arguments.
  std::vector<Expr*> conv = es;

  Expr*& fn = conv[0];
  fn = require_function(fn);

  Fn_type* ft = static_cast<Fn_type*>(fn->get_type());

  auto parms = ft->get_parameter_types();
  auto args = tail(conv);

  // Quick reject for parameter/argument mismatch.
  if (parms.size() < args.size())
    throw std::runtime_error("too many arguments");
  if (parms.size() > args.size())
    throw std::runtime_error("too few arguments");

  auto pi = parms.begin();
  auto ai = args.begin();
  while (pi != parms.end()) {
    // Copy initialize each parameter. We do this with a dummy object
    // and then get the resulting initializer.
    Var_decl dummy(nullptr, *pi);
    copy_initialize(&dummy, *ai);
    *ai = dummy.get_initializer();
    ++pi;
    ++ai;
  }
  
  return new Call_expr(ft->get_return_type(), std::move(conv));
}

