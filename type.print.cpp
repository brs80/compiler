#include "type.hpp"
#include "printer.hpp"

#include <iostream>

static void
print_literal(Printer& p, char const* str)
{
  p.get_stream() << str;
}

static void
print_ref(Printer& p, Ref_type const* t)
{
  p.get_stream() << "ref ";
  print_type(p, t->get_object_type());
}

static void
print_fn(Printer& p, Fn_type const* t)
{
  p.get_stream() << '(';
  auto parms = t->get_parameter_types();
  for (auto iter = parms.begin(); iter != parms.end(); ++iter) {
    print_type(p, *iter);
    if (std::next(iter) != parms.end())
      p.get_stream() << ',';
  }
  p.get_stream() << ')' << "->";
  auto kids = t->get_children();
  print_type(p, t->get_return_type());
}

void
print_type(Printer& p, Type const* t)
{
  switch (t->get_kind()) {
  case Type::bool_type:
    return print_literal(p, "bool");
  case Type::int_type:
    return print_literal(p, "int");
  case Type::float_type:
    return print_literal(p, "float");
  case Type::ref_type:
    return print_ref(p, static_cast<Ref_type const*>(t));
  case Type::fn_type:
    return print_fn(p, static_cast<Fn_type const*>(t));
  }
}

std::ostream&
operator<<(std::ostream& os, Type const& t)
{
  Printer p(os);
  print_type(p, &t);
  return os;
}
