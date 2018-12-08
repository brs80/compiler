#include "builder.hpp"
#include "type.hpp"

// FIXME: Make types unique.
Type*
Builder::get_reference_type(Type* t)
{
  return new Ref_type(t);
}

/// FIXME: Make types unique.
Type*
Builder::get_function_type(std::vector<Type*> const& ts)
{
  return new Fn_type(ts);
}


