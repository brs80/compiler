#include "expr.hpp"
#include "name.hpp"
#include "type.hpp"
#include "decl.hpp"
#include "printer.hpp"

#include <iostream>

static void 
debug_header(Printer& p, Expr const* e)
{
  p.print_indentation();
  p.get_stream() << e->get_kind_name() << ' ';
  p.get_stream() << (void*)e << ' ';
  p.get_stream() << "type='" << *e->get_type() << "' ";
}

static void
debug_value(Printer& p, Literal_expr const* e)
{
  p.get_stream() << "value=" << e->get_value() << ' ';
}

static void
debug_attrs(Printer& p, Expr const* e)
{
  Print_final_newline nl(p);

  switch (e->get_kind()) {
  case Expr::bool_lit:
  case Expr::int_lit:
  case Expr::float_lit:
    return debug_value(p, static_cast<Literal_expr const*>(e));
  case Expr::id_expr:
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

static void
debug_kids(Printer& p, Expr const* e)
{
  p.indent();
  for (Expr const* child : e->get_children())
    debug_expr(p, child);
  p.undent();
}

void
debug_expr(Printer& p, Expr const* e)
{
  debug_header(p, e);
  debug_attrs(p, e);
  debug_kids(p, e);
}

void
Expr::debug() const
{
  Printer p(std::cerr);
  debug_expr(p, this);
}