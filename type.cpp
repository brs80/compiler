#include "type.hpp"

#include <algorithm>

char const*
Type::get_kind_name() const
{
  switch (m_kind) {
  case bool_type: return "bool_type";
  case int_type: return "int_type";
  case float_type: return "float_type";
  case ref_type: return "ref_type";
  case fn_type: return "fn_type";
  }
}

bool
Type::is_reference_to(Type const* that) const
{
  if (is_reference()) {
    Ref_type const* ref = static_cast<Ref_type const*>(this);
    if (is_same(ref->get_object_type(), that))
      return true;
  }
  return false;
}

bool
Type::is_same_as(Type const* that) const
{
  return is_same(this, that);
}

static bool
is_same_ref(Ref_type const* a, Ref_type const* b)
{
  return is_same(a->get_child(), b->get_child());
}

static bool
is_same_fn(Fn_type const* a, Fn_type const* b)
{
  return std::equal(a->begin(), a->end(), b->begin(), b->end(), is_same);
}

bool
is_same(Type const* a, Type const* b)
{
  if (a->get_kind() != b->get_kind())
    return false;

  switch (a->get_kind()) {
  case Type::bool_type:
  case Type::int_type:
  case Type::float_type:
    return true;
  case Type::ref_type:
    return is_same_ref(static_cast<Ref_type const*>(a), 
                       static_cast<Ref_type const*>(b));
  case Type::fn_type:
    return is_same_fn(static_cast<Fn_type const*>(a), 
                      static_cast<Fn_type const*>(b));
  }
}

