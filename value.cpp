#include "value.hpp"
#include "name.hpp"
#include "decl.hpp"

#include <iostream>

std::ostream&
operator<<(std::ostream& os, Value const& val)
{
  switch (val.get_kind()) {
  case Value::non_val:
    return os << "<indeterminate>";
  
  case Value::int_val:
    return os << val.get_int();
  
  case Value::float_val:
    return os << val.get_float();
  
  case Value::fn_val: {
    Fn_decl* fn = val.get_function();
    os << '<' << "function" << ' ' << *fn->get_name() << ' ' << fn << '>';
    return os;
  }
  case Value::addr_val: {
    Addr_value const& addr = val.get_address();
    os << '<';
    if (addr.store == -1)
      os << "static";
    else
      os << "auto" << '(' << addr.store << ')';
    os << ' ' << *addr.def->get_name() << ' ' << addr.def << '>';
    return os;
  }
  default:
    break;
  }
  os << "<unknown>";
  return os;
}