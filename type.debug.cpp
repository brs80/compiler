#include "type.hpp"
#include "printer.hpp"

#include <iostream>

static void
debug_header(Printer& p, Type const* t)
{
  p.print_indentation();
  p.get_stream() << t->get_kind_name() << ' ';
  p.get_stream() << (void*)t;
}

static void
debug_literal(Printer& p, char const* str)
{
  p.get_stream() << "type='" << str << "' ";
}

static void
debug_attrs(Printer& p, Type const* t)
{
  Print_final_newline nl(p);

  switch (t->get_kind()) {
  case Type::bool_type:
    return debug_literal(p, "bool");
  case Type::int_type:
    return debug_literal(p, "int");
  case Type::float_type:
    return debug_literal(p, "float");
  case Type::ref_type:
  case Type::fn_type:
    break;
  }
}

static void
debug_kids(Printer& p, Type const* t)
{
  p.indent();
  for (Type const* child : t->get_children())
    debug_type(p, child);
  p.undent();
}

void
debug_type(Printer& p, Type const* t)
{
  debug_header(p, t);
  debug_attrs(p, t);
  debug_kids(p, t);
}

void
Type::debug() const
{
  Printer p(std::cerr);
  debug_type(p, this);
}
