#include "expr.hpp"
#include "name.hpp"
#include "type.hpp"
#include "decl.hpp"
#include "printer.hpp"

#include <iostream>

static void
print_bool(Printer& p, Bool_expr const* e)
{
  if (e->get_bool_value())
    p.get_stream() << "true";
  else
    p.get_stream() << "false";
}

static void
print_int(Printer& p, Int_expr const* e)
{
  p.get_stream() << e->get_int_value();
}

static void
print_float(Printer& p, Float_expr const* e)
{
  p.get_stream() << e->get_float_value();
}

static void
print_id(Printer& p, Id_expr const* e)
{
  p.get_stream() << e->get_declaration()->get_name()->get_string();
}

void
print_expr(Printer& p, Expr const* e)
{
  switch (e->get_kind()) {
  case Expr::bool_lit:
    return print_bool(p, static_cast<Bool_expr const*>(e));
  case Expr::int_lit:
    return print_int(p, static_cast<Int_expr const*>(e));
  case Expr::float_lit:
    return print_float(p, static_cast<Float_expr const*>(e));
  case Expr::id_expr:
    return print_id(p, static_cast<Id_expr const*>(e));
  case Expr::add_expr:
  case Expr::sub_expr:
  case Expr::mul_expr:
  case Expr::div_expr:
  case Expr::rem_expr:
  case Expr::neg_expr:
  case Expr::rec_expr:
  case Expr::eq_expr:
  case Expr::ne_expr:
  case Expr::lt_expr:
  case Expr::gt_expr:
  case Expr::le_expr:
  case Expr::ge_expr:
  case Expr::cond_expr:
  case Expr::and_expr:
  case Expr::or_expr:
  case Expr::not_expr:
  case Expr::assign_expr:
  case Expr::call_expr:
  case Expr::value_conv:
    break;
  }
}


std::ostream&
operator<<(std::ostream& os, Expr const& e)
{
  Printer p(os);
  print_expr(p, &e);
  return os;
}
