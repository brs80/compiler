#include "builder.hpp"
#include "type.hpp"
#include "expr.hpp"

Expr*
Builder::convert_to_value(Expr* e)
{
  Type* t = e->get_type();
  if (t->is_reference()) {
    Ref_type* ref = static_cast<Ref_type*>(t);
    return new Value_conv(ref->get_object_type(), e);
  }
  return e;
}

