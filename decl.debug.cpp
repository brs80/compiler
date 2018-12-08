#include "decl.hpp"
#include "name.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include "printer.hpp"

#include <iostream>

static void 
debug_header(Printer& p, Decl const* d)
{
  p.print_indentation();
  p.get_stream() << d->get_kind_name() << ' ';
  p.get_stream() << (void*)d << ' ';
  
  if (Name* n = d->get_name())
    p.get_stream() << "name='" << *n << "' ";
  
  if (Type* t = d->get_type())
    p.get_stream() << "type='" << *t << "' ";
}

static void
debug_attrs(Printer& p, Decl const* d)
{
  // FIXME: Any other attributes to print?
  p.print_newline();
}

static void
debug_kids_real(Printer& p, Decl const* d)
{
  p.indent();
  for (Decl const* child : d->get_children())
    debug_decl(p, child);
  p.undent();
}

static void
debug_fn(Printer& p, Fn_decl const* d)
{
  p.indent();
  for (Decl const* child : d->get_parameters())
    debug_decl(p, child);
  debug_decl(p, d->get_return());
  debug_stmt(p, d->get_body());
  p.undent();
}

static void
debug_kids(Printer& p, Decl const* d)
{
  switch (d->get_kind()) {
  default:
    return;
  case Decl::fn_decl:
    return debug_fn(p, static_cast<Fn_decl const*>(d));
  case Decl::prog_decl:
    return debug_kids_real(p, static_cast<Prog_decl const*>(d));
  }
}

void
debug_decl(Printer& p, Decl const* d)
{
  debug_header(p, d);
  debug_attrs(p, d);
  debug_kids(p, d);
}

void
Decl::debug() const
{
  Printer p(std::cerr);
  debug_decl(p, this);
}
