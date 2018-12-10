#include "stmt.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "printer.hpp"

#include <iostream>

static void
print_literal(Printer& p, char const* str)
{
  p.get_stream() << str << ';';
}

static void
print_block(Printer& p, Block_stmt const* s)
{
  // FIXME: Indent this.
  p.get_stream() << '{';
  for (Stmt* sub : *s) {
    print_stmt(p, sub);
    p.print_newline();
  }
  p.get_stream() << '}';
}

static void
print_expr(Printer& p, Expr_stmt const* s)
{
  print_expr(p, s->get_expression());
}

static void
print_ret(Printer& p, Ret_stmt const* s)
{
  p.get_stream() << "return ";
  print_expr(p, s->get_return_value());
}

void
print_stmt(Printer& p, Stmt const* s)
{
  switch (s->get_kind()) {
  case Stmt::skip_stmt:
    return print_literal(p, "skip");
  case Stmt::block_stmt:
    return print_block(p, static_cast<Block_stmt const*>(s));
  case Stmt::if_stmt:
  case Stmt::while_stmt:
    break;
  case Stmt::break_stmt:
    return print_literal(p, "break");
  case Stmt::cont_stmt:
    return print_literal(p, "continue");
  case Stmt::ret_stmt:
    return print_ret(p, static_cast<Ret_stmt const*>(s));
  case Stmt::expr_stmt:
    return print_expr(p, static_cast<Expr_stmt const*>(s));
  case Stmt::decl_stmt:
    break;
  }
  assert(false);
}

std::ostream&
operator<<(std::ostream& os, Stmt const& s)
{
  Printer p(os);
  print_stmt(p, &s);
  return os;  
}
