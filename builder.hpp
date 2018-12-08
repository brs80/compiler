#pragma once

#include <vector>

#include "type.hpp"

class Name;
class Type;
class Expr;
class Stmt;
class Decl;
class Var_decl;
class Fn_decl;


/// The Builder class is responsible for performing semantic analysis
/// and 
class Builder
{
public:
  // Names

  Name* get_name(char const* str);
  /// Returns a name spelled by `str`.

  // Types

  Type* get_bool_type() { return &m_bool_type; }
  /// Returns the type `bool`.
  
  Type* get_int_type() { return &m_int_type; }
  /// Returns the type `int`.
  
  Type* get_float_type() { return &m_float_type; }
  /// Returns the type `float`.

  Type* get_reference_type(Type* t);
  /// Returns the type `ref t`.

  Type* get_function_type(std::vector<Type*> const& ts);
  /// Returns the type `(t1, t2, ..., tn) -> tr`.

  // Expressions
  
  Expr* make_bool(bool b);
  /// Returns a new boolean literal.

  Expr* make_true();
  /// Returns the literal `true`.

  Expr* make_false();
  /// Returns the literal `false`.

  Expr* make_int(int n);
  /// Returns a new integer literal.

  Expr* make_float(double n);
  /// Returns a new floating point literal.

  Expr* make_and(Expr* e1, Expr* e2);
  /// Returns the expression `e1 && e2`.

  Expr* make_or(Expr* e1, Expr* e2);
  /// Returns the expression `e1 || e2`.

  Expr* make_not(Expr* e1);
  /// Returns the expression `!e1`.

  Expr* make_conditional(Expr* e1, Expr* e2, Expr* e3);
  /// Returns the expression `e1 ? e2 : e3`.

  Expr* make_id(Decl* d);
  /// Returns an id-expression referring to `d`.

  // Relational expressions

  Expr* make_eq(Expr* e1, Expr* e2);
  /// Returns the expression `e1 == e2`.

  Expr* make_ne(Expr* e1, Expr* e2);
  /// Returns the expression `e1 != e2`.

  Expr* make_lt(Expr* e1, Expr* e2);
  /// Returns the expression `e1 < e2`.

  Expr* make_gt(Expr* e1, Expr* e2);
  /// Returns the expression `e1 > e2`.

  Expr* make_le(Expr* e1, Expr* e2);
  /// Returns the expression `e1 <= e2`.

  Expr* make_ge(Expr* e1, Expr* e2);
  /// Returns the expression `e1 >= e2`.

  // Arithmetic expressions

  Expr* make_add(Expr* e1, Expr* e2);
  /// Returns the expression `e1 + e2`.

  Expr* make_sub(Expr* e1, Expr* e2);
  /// Returns the expression `e1 - e2`.

  Expr* make_mul(Expr* e1, Expr* e2);
  /// Returns the expression `e1 * e2`.

  Expr* make_div(Expr* e1, Expr* e2);
  /// Returns the expression `e1 / e2`.

  Expr* make_rem(Expr* e1, Expr* e2);
  /// Returns the expression `e1 % e2`.

  Expr* make_neg(Expr* e1);
  /// Returns the expression `-e1`.

  Expr* make_rec(Expr* e1);
  /// Returns the expression `/e1`.

  Expr* make_assign(Expr* e1, Expr* e2);
  /// Returns the expression `e1 = e2`.

  Expr* make_call(std::vector<Expr*> const& es);
  /// Returns the expression `e1(e2, e3, ..., en).

  // Statements

  Stmt* make_skip();
  /// Returns the statement `skip`.

  Stmt* make_block(std::initializer_list<Stmt*> ss);
  /// Returns the statement `{ s1 s2 ... sn }`.

  Stmt* make_block(std::vector<Stmt*> const& ss);
  /// Returns the statement `{ s1 s2 ... sn }`.

  Stmt* make_if(Expr* e, Stmt* s1, Stmt* s2);
  /// Returns the statement `if (e) s1 else s2`.

  Stmt* make_while(Expr* e, Stmt* s1);
  /// Returns the statement `while (e) s1`.

  Stmt* make_break();
  /// Returns the statement `skip`.

  Stmt* make_continue();
  /// Returns the statement `skip`.

  Stmt* make_return(Decl* d, Expr* e);
  /// Returns the statement `return e;`.

  Stmt* make_expression(Expr* e);
  /// Returns the statement `e;`.

  Stmt* make_declaration(Decl* d);
  /// Returns the statement `d`.

  // Declarations

  Var_decl* make_variable(Name* n, Type* t);
  /// Returns a new object or reference definition. 

  Fn_decl* make_function(Name* n, Type* t);
  /// Returns a new function definition.

  // Typing

  Expr* require_bool(Expr* e);
  /// Return `e` if its converted type is `bool`.

  Expr* require_arithmetic(Expr* e);
  /// Returns `e` if its converted type is arithmetic.

  Expr* require_function(Expr* e);
  /// Returns `e` if it is a function type.

  Expr* require_type(Expr* e, Type* t);
  /// Returns `e` converted to `t`.

  Expr* require_value_of(Expr* e, Type* t);
  /// Returns `e` if it is a reference to an object of type `t`.

  Expr* require_reference_to(Expr* e, Type* t);
  /// Returns `e` if it is a reference to an object of type `t`.

  std::pair<Expr*, Expr*> require_same(Expr* e1, Expr* e2);
  /// Returns {e1, e2} if they can have the same type.

  std::pair<Expr*, Expr*> require_same_value(Expr* e1, Expr* e2);
  /// Returns {e1, e2} if they can be converted to the same value.

  std::pair<Expr*, Expr*> require_same_arithmetic(Expr* e1, Expr* e2);
  /// Returns {e1, e2} if they can be converted to the same arithmetic value.

  std::pair<Expr*, Expr*> require_common(Expr* e1, Expr* e2);
  /// Returns {e1, e2} if they can be converted to a common type.

  // Conversions

  Expr* convert_to_value(Expr* e);
  /// Convert a reference to a value.

  // Initialization

  void copy_initialize(Decl* d, Expr* e);
  /// Initialize `d` with the expression `e`. Returns the converted expression.

  void reference_initialize(Decl* d, Expr* e);
  /// Bind `d` to the expression `e`. Returns the converted expression.

private:
  Bool_type m_bool_type;
  /// The type `bool`.

  Int_type m_int_type;
  /// The type `int`.

  Float_type m_float_type;
  /// The type `float`.
};
