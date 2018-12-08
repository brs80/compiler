#include "decl.hpp"
#include "name.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include "printer.hpp"

#include <iostream>

char const*
Decl::get_kind_name() const
{
  switch (m_kind) {
  case prog_decl: return "prog_decl";
  case var_decl: return "var_decl";
  case fn_decl: return "fn_decl";
  }
}

bool
Decl::is_object() const
{
  return is_variable() && get_type()->is_object();
}

bool
Decl::is_reference() const
{
  return is_variable() && get_type()->is_reference();
}

Value_decl*
Decl::get_as_value()
{
  switch (m_kind) {
  case var_decl:
    return static_cast<Var_decl*>(this);
  case fn_decl:
    return static_cast<Fn_decl*>(this);
  default:
    return nullptr;
  }
}

Value_decl const*
Decl::get_as_value() const
{
  return const_cast<Decl*>(this)->get_as_value();
}


Fn_type*
Fn_decl::get_function_type() const
{
  return static_cast<Fn_type*>(get_type());
}

std::size_t
Fn_decl::get_num_parameters() const
{
  return get_function_type()->get_num_parameters();
}

Node_range<Decl>
Fn_decl::get_parameters()
{
  return get_children().front(get_num_parameters());
}

Node_range<Decl const>
Fn_decl::get_parameters() const
{
  return get_children().front(get_num_parameters());
}

void
Fn_decl::set_parameters(std::vector<Decl*> const& parms)
{
  for (Decl* p : parms)
    add_parameter(p);
}

void
Fn_decl::set_parameters(std::initializer_list<Decl*> parms)
{
  for (Decl* p : parms)
    add_parameter(p);
}

void
Fn_decl::add_parameter(Decl* d)
{
  assert(d->is_variable());
  assert(get_children().size() <= get_num_parameters());
  add_child(d);
}

Decl* 
Fn_decl::get_return()
{
  return get_children().at(get_num_parameters());
}

Decl const* 
Fn_decl::get_return() const
{
  return get_children().at(get_num_parameters());
}

void
Fn_decl::set_return(Decl* d)
{
  assert(d->is_variable());
  assert(get_children().size() == get_num_parameters());
  add_child(d);
}
