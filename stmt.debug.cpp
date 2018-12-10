#include "stmt.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "printer.hpp"

#include <iostream>

static void 
debug_header(Printer& p, Stmt const* s)
{
  p.print_indentation();
  p.get_stream() << s->get_kind_name() << ' ';
  p.get_stream() << (void*)s << ' ';
}

static void
debug_attrs(Printer& p, Stmt const* s)
{
  // Do statements have attributes?
  p.print_newline();
}

static void
debug_if(Printer& p, If_stmt const* s)
{
  debug_expr(p, s->get_condition());
}

static void
debug_while(Printer& p, While_stmt const* s)
{
  debug_expr(p, s->get_condition());
}

static void
debug_ret(Printer& p, Ret_stmt const* s)
{
  debug_expr(p, s->get_return_value());
}

static void
debug_expr(Printer& p, Expr_stmt const* s)
{
  debug_expr(p, s->get_expression());
}

static void
debug_decl(Printer& p, Decl_stmt const* s)
{
  // debug_decl(p, s->get_declaration());
}

// Some statements have non-statement "children".
static void
debug_extra_kids(Printer& p, Stmt const* s)
{
  switch (s->get_kind()) {
  default:
    return;
  case Stmt::if_stmt:
    return debug_if(p, static_cast<If_stmt const*>(s));
  case Stmt::while_stmt:
    return debug_while(p, static_cast<While_stmt const*>(s));
  case Stmt::ret_stmt:
    return debug_ret(p, static_cast<Ret_stmt const*>(s));
  case Stmt::expr_stmt:
    return debug_expr(p, static_cast<Expr_stmt const*>(s));
  case Stmt::decl_stmt:
    return debug_decl(p, static_cast<Decl_stmt const*>(s));
  }
}

static void
debug_kids(Printer& p, Stmt const* s)
{
  p.indent();
  debug_extra_kids(p, s);
  for (Stmt const* child : s->get_children())
    debug_stmt(p, child);
  p.undent();
}

void
debug_stmt(Printer& p, Stmt const* s)
{
  debug_header(p, s);
  debug_attrs(p, s);
  debug_kids(p, s);
}

void
Stmt::debug() const
{
  Printer p(std::cerr);
  debug_stmt(p, this);
}
