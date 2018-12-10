#include "builder.hpp"
#include "type.hpp"
#include "expr.hpp"

Expr*
Builder::require_bool(Expr* e)
{
  // First convert to a value.
  Expr* c = convert_to_value(e);

  // If the type is not bool, emit an error.
  Type* t = c->get_type();
  if (!t->is_bool()) {
    if(t->is_reference()) 
      throw std::runtime_error("operand not boolean, found reference.");
    if(t->is_object())
      throw std::runtime_error("Found object expected arithmetic");
    if(t->is_arithmetic()) 
      throw std::runtime_error("Found arithmetic expected boolean");
    throw std::runtime_error("operand not boolean");
  }
  return c;
}

Expr*
Builder::require_arithmetic(Expr* e)
{
  // First convert to a value.
  Expr* c = convert_to_value(e);

  // If the type is not arithmetic, emit an error.
  Type* t = c->get_type();
  if (!t->is_arithmetic())
    if(t->is_bool()) 
      throw std::runtime_error("Found bool expected arithmetic");
    if(t->is_reference())
      throw std::runtime_error("Found reference expected arithmetic");
    if(t->is_object()) 
      throw std::runtime_error("Found object expected arithmetic");

    throw std::runtime_error("operand not arithmetic");

  return c;
}

Expr*
Builder::require_function(Expr* e)
{
  Expr* c = convert_to_value(e);
  Type* t = c->get_type();
  if(!t->is_function()) { 
    if(t->is_bool()) 
      throw std::runtime_error("not  a function. Found bool.");
    if(t->is_reference())
      throw std::runtime_error("Found reference instead of function");
    if(t->is_object())
      throw std::runtime_error("Found object instead of function");
    if(t->is_arithmetic())
      throw std::runtime_error("Found arithmetic instead of function");
  }

  if (t->is_function())
    return c;
}

Expr*
Builder::require_type(Expr* e, Type* t)
{
  if (t->is_reference())
    return require_reference_to(e, t);
  else
    return require_value_of(e, t);
}

Expr*
Builder::require_value_of(Expr* e, Type* t)
{
  assert(t->is_object());

  e = convert_to_value(e);

  if (!e->get_type()->is_same_as(t))
    throw std::runtime_error("invalid operand");

  return e;
}

Expr*
Builder::require_reference_to(Expr* e, Type* t)
{
  Type* t1 = e->get_type();
  if (t1->is_reference_to(t))
    return e;
  throw std::runtime_error("invalid operand reference");
}

std::pair<Expr*, Expr*>
Builder::require_same(Expr* e1, Expr* e2)
{
  Type* t1 = e1->get_type();
  Type* t2 = e2->get_type();
  if (!t1->is_same_as(t2))
    throw std::runtime_error("operands have different type");
  return {e1, e2};
}

std::pair<Expr*, Expr*>
Builder::require_same_value(Expr* e1, Expr* e2)
{
  e1 = convert_to_value(e1);
  e2 = convert_to_value(e2);
  return require_same(e1, e2);
}

std::pair<Expr*, Expr*>
Builder::require_same_arithmetic(Expr* e1, Expr* e2)
{
  e1 = convert_to_value(e1);
  e2 = convert_to_value(e2);
  auto p = require_same(e1, e2);

  Type* t = p.first->get_type();
  if (!t->is_arithmetic())
    throw std::runtime_error("operands are not arithmetic");
  
  return p;
}

std::pair<Expr*, Expr*>
Builder::require_common(Expr* e1, Expr* e2)
{
  Type* t1 = e1->get_type();
  Type* t2 = e2->get_type();
  
  // If both expressions have reference type, they must refer to the
  // the same type of object.
  if (t1->is_reference() && t2->is_reference())
    return require_same(e1, e2);

  // Otherwise, they must both have the same value type.
  return require_same_value(e1, e2);
}

