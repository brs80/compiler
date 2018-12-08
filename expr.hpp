#pragma once

#include "tree.hpp"
#include "value.hpp"

class Printer;
class Type;
class Decl;

/// Represents an expression in the language.
class Expr
{
public:
  enum Kind
  {
    bool_lit,
    int_lit,
    float_lit,
    id_expr,
    add_expr,
    sub_expr,
    mul_expr,
    div_expr,
    rem_expr,
    neg_expr,
    rec_expr,
    eq_expr,
    ne_expr,
    lt_expr,
    gt_expr,
    le_expr,
    ge_expr,
    cond_expr,
    and_expr,
    or_expr,
    not_expr,
    assign_expr,
    call_expr,
    value_conv,
  };

protected:
  Expr(Kind k, Type* t);

public:
  Kind get_kind() const { return m_kind; }
  /// Returns the kind of expression.

  char const* get_kind_name() const;
  /// Returns a string representing the kind.

  // Typing
  Type* get_type() const { return m_type; }
  /// Returns the type of the expression.

  // Children

  virtual Node_range<Expr> get_children() = 0;
  /// Returns the children of the expression.

  virtual Node_range<Expr const> get_children() const = 0;
  /// Returns the children of the expression.

  // Debugging

  void debug() const;
  /// Emit debugging information about this expression.

private:
  Kind m_kind;
  /// The kind of expression.

  Type* m_type;
  /// The type of the expression.
};

inline
Expr::Expr(Kind k, Type* t)
  : m_kind(k), m_type(t)
{ }



// General purpose expression classes

/// Represents nullary expressions (i.e., leaf nodes).
class Nullary_expr : public Expr, public Static_arity_node<Expr, 0>
{
  using Base = Static_arity_node<Expr, 0>;
protected:
  Nullary_expr(Kind k, Type* t);
  /// Constructs an expression with the given arguments.

public:
  Node_range<Expr> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Expr const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Nullary_expr::Nullary_expr(Kind k, Type* t)
  : Expr(k, t), Static_arity_node<Expr, 0>()
{ }


/// Represents unary expressions (i.e., expressions with one operand).
class Unary_expr : public Expr, public Static_arity_node<Expr, 1>
{
  using Base = Static_arity_node<Expr, 1>;
protected:
  Unary_expr(Kind k, Type* t, Expr* e);
  /// Constructs the expression with the given arguments.

public:
  Node_range<Expr> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Expr const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Unary_expr::Unary_expr(Kind k, Type* t, Expr* e)
  : Expr(k, t), Static_arity_node<Expr, 1>(e)
{ }


/// Represents binary expressions (i.e., expressions with two operands).
class Binary_expr : public Expr, public Static_arity_node<Expr, 2>
{
  using Base = Static_arity_node<Expr, 2>;
protected:
  Binary_expr(Kind k, Type* t, Expr* e1, Expr* e2);
  /// Constructs the expression with the given arguments.

public:
  Node_range<Expr> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Expr const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Binary_expr::Binary_expr(Kind k, Type* t, Expr* e1, Expr* e2)
  : Expr(k, t), Static_arity_node<Expr, 2>(e1, e2)
{ }


/// Represents ternary expressions.
class Ternary_expr : public Expr, public Static_arity_node<Expr, 3>
{
  using Base = Static_arity_node<Expr, 3>;
protected:
  Ternary_expr(Kind k, Type* t, Expr* e1, Expr* e2, Expr* e3);
  /// Constructs the expression with the given arguments.

public:
  Node_range<Expr> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Expr const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Ternary_expr::Ternary_expr(Kind k, Type* t, Expr* e1, Expr* e2, Expr* e3)
  : Expr(k, t), Static_arity_node<Expr, 3>(e1, e2, e3)
{ }


/// Represents k-ary expressions (i.e., nodes with k children).
class Kary_expr : public Expr, public Dynamic_arity_node<Expr>
{
  using Base = Dynamic_arity_node<Expr>;
protected:
  Kary_expr(Kind k, Type* t, std::initializer_list<Expr*> list);
  /// Constructs the expression with the given arguments.
  
  Kary_expr(Kind k, Type* t, std::vector<Expr*> const& vec);
  /// Constructs the expression with the given arguments.
  
  Kary_expr(Kind k, Type* t, std::vector<Expr*>&& vec);
  /// Constructs the expression with the given arguments.

public:
  Node_range<Expr> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Expr const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Kary_expr::Kary_expr(Kind k, Type* t, std::initializer_list<Expr*> list)
  : Expr(k, t), Dynamic_arity_node<Expr>(list)
{ }

inline
Kary_expr::Kary_expr(Kind k, Type* t, std::vector<Expr*> const& vec)
  : Expr(k, t), Dynamic_arity_node<Expr>(vec)
{ }

inline
Kary_expr::Kary_expr(Kind k, Type* t, std::vector<Expr*>&& vec)
  : Expr(k, t), Dynamic_arity_node<Expr>(std::move(vec))
{ }


// Literals

// A literal is a nullary node with a value.
class Literal_expr : public Nullary_expr
{
protected:
  Literal_expr(Kind k, Type* t, Value const& val);
  /// Constructs a literal from the given value.

public:
  Value const& get_value() const { return m_value; }
  /// Returns the value.

private:
  Value m_value;  
  /// The interpreted value of the literal.
};

inline
Literal_expr::Literal_expr(Kind k, Type* t, Value const& val)
  : Nullary_expr(k, t), m_value(val)
{ }


/// A boolean literal, true or false.
class Bool_expr : public Literal_expr
{
public:
  Bool_expr(Type* t, Value const& val);
  /// Constructs the `val`.

  bool get_bool_value() const { return get_value().get_int(); }
  /// Returns the boolean value of the literal.
};

inline
Bool_expr::Bool_expr(Type* t, Value const& val)
  : Literal_expr(bool_lit, t, val)
{ }


/// An integer literal, 0, 1, 2, ..., n.
class Int_expr : public Literal_expr
{
public:
  Int_expr(Type* t, Value const& val);
  /// Constructs the literal `val`.

  int get_int_value() const { return get_value().get_int(); }
  /// Returns the boolean value of the literal.
};

inline
Int_expr::Int_expr(Type* t, Value const& val)
  : Literal_expr(int_lit, t, val)
{ }


/// An floating point literal e.g., 3.14.
class Float_expr : public Literal_expr
{
public:
  Float_expr(Type* t, Value const& val);
  /// Constructs the literal `val`.

  double get_float_value() const { return get_value().get_float(); }
  /// Returns the boolean value of the literal.
};

inline
Float_expr::Float_expr(Type* t, Value const& val)
  : Literal_expr(int_lit, t, val)
{ }


/// Represents id-expressions that name declarations.
class Id_expr : public Nullary_expr
{
public:
  Id_expr(Type* t, Decl* d);
  /// Constructs the literal `d`.

  Decl* get_declaration() const { return m_decl; }
  /// Returns the named declaration.

private:
  Decl* m_decl;
};

inline
Id_expr::Id_expr(Type* t, Decl* d)
  : Nullary_expr(id_expr, t), m_decl(d)
{ }


// Arithmetic expressions

/// Represents expressions of the form `e1 + e2`.
class Add_expr : public Binary_expr
{
public:
  Add_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 + e2`.
};

inline
Add_expr::Add_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(add_expr, t, e1, e2)
{ }


/// Represents expressions of the form `e1 - e2`.
class Sub_expr : public Binary_expr
{
public:
  Sub_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 - e2`.
};

inline
Sub_expr::Sub_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(sub_expr, t, e1, e2)
{ }


/// Represents expressions of the form `e1 * e2`.
class Mul_expr : public Binary_expr
{
public:
  Mul_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 * e2`.
};

inline
Mul_expr::Mul_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(mul_expr, t, e1, e2)
{ }


/// Represents expressions of the form `e1 / e2`.
class Div_expr : public Binary_expr
{
public:
  Div_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 / e2`.
};

inline
Div_expr::Div_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(div_expr, t, e1, e2)
{ }


/// Represents expressions of the form `e1 % e2`.
class Rem_expr : public Binary_expr
{
public:
  Rem_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 % e2`.
};

inline
Rem_expr::Rem_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(rem_expr, t, e1, e2)
{ }


/// Represents expressions of the form `-e1`.
class Neg_expr : public Unary_expr
{
public:
  Neg_expr(Type* t, Expr* e1);
  /// Construct the expression `-e1`.
};

inline
Neg_expr::Neg_expr(Type* t, Expr* e1)
  : Unary_expr(neg_expr, t, e1)
{ }


/// Represents expressions of the form `/e1`.
class Rec_expr : public Unary_expr
{
public:
  Rec_expr(Type* t, Expr* e1);
  /// Construct the expression `/e1`.
};

inline
Rec_expr::Rec_expr(Type* t, Expr* e1)
  : Unary_expr(neg_expr, t, e1)
{ }


// Relational expressions

/// Represents expressions of the form `e1 == e2`.
class Eq_expr : public Binary_expr
{
public:
  Eq_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 == e2`.
};

inline
Eq_expr::Eq_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(eq_expr, t, e1, e2)
{ }


/// Represents expressions of the form `e1 != e2`.
class Ne_expr : public Binary_expr
{
public:
  Ne_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 != e2`.
};

inline
Ne_expr::Ne_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(ne_expr, t, e1, e2)
{ }


/// Represents expressions of the form `e1 < e2`.
class Lt_expr : public Binary_expr
{
public:
  Lt_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 < e2`.
};

inline
Lt_expr::Lt_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(lt_expr, t, e1, e2)
{ }


/// Represents expressions of the form `e1 > e2`.
class Gt_expr : public Binary_expr
{
public:
  Gt_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 > e2`.
};

inline
Gt_expr::Gt_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(gt_expr, t, e1, e2)
{ }


/// Represents expressions of the form `e1 <= e2`.
class Le_expr : public Binary_expr
{
public:
  Le_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 <= e2`.
};

inline
Le_expr::Le_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(le_expr, t, e1, e2)
{ }


/// Represents expressions of the form `e1 >= e2`.
class Ge_expr : public Binary_expr
{
public:
  Ge_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 >= e2`.
};

inline
Ge_expr::Ge_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(ge_expr, t, e1, e2)
{ }


// Logical expressions

/// Represents expressions of the form `e1 ? e2 : e3`.
class Cond_expr : public Ternary_expr
{
public:
  Cond_expr(Type* t, Expr* e1, Expr* e2, Expr* e3);
  /// Construct the expression `e1 ? e2 : e3`.

  Expr* get_condition() const { return get_child(0); }
  /// Returns the condition of the expression.
  
  Expr* get_true_value() const { return get_child(1); }
  /// Returns the expression to evaluate when the condition is true.
  
  Expr* get_false_value() const { return get_child(2); }
  /// Returns the expression to evaluate when the condition is false.
};

inline
Cond_expr::Cond_expr(Type* t, Expr* e1, Expr* e2, Expr* e3)
  : Ternary_expr(cond_expr, t, e1, e2, e3)
{ }


/// Represents expressions of the form `e1 && e2`.
class And_expr : public Binary_expr
{
public:
  And_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 && e2`.
};

inline
And_expr::And_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(and_expr, t, e1, e2)
{ }


/// Represents expressions of the form `e1 || e2`.
class Or_expr : public Binary_expr
{
public:
  Or_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 || e2`.
};

inline
Or_expr::Or_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(or_expr, t, e1, e2)
{ }


/// Represents expressions of the form `!e1`.
class Not_expr : public Unary_expr
{
public:
  Not_expr(Type* t, Expr* e1);
  /// Construct the expression `!e1`.
};

inline
Not_expr::Not_expr(Type* t, Expr* e1)
  : Unary_expr(not_expr, t, e1)
{ }


/// Represents expressions of the form `e1 = e2`.
class Assign_expr : public Binary_expr
{
public:
  Assign_expr(Type* t, Expr* e1, Expr* e2);
  /// Construct the expression `e1 = e2`.
};

inline
Assign_expr::Assign_expr(Type* t, Expr* e1, Expr* e2)
  : Binary_expr(assign_expr, t, e1, e2)
{ }



// Functions

/// Represents call expressions of the form `e1(e2, e3, ..., en)`.
class Call_expr : public Kary_expr
{
public:
  Call_expr(Type* t, std::initializer_list<Expr*> list);
  /// Construct the expression `e(e1, e2, ..., en)`.
  
  Call_expr(Type* t, std::vector<Expr*> const& vec);
  /// Construct the expression `e(e1, e2, ..., en)`.
  
  Call_expr(Type* t, std::vector<Expr*>&& vec);
  /// Construct the expression `e(e1, e2, ..., en)`.

  Expr* get_function() { return get_children().front(); }
  /// Returns the expression designating the function to be called.

  Expr const* get_function() const { return get_children().front(); }
  /// Returns the expression designating the function to be called.

  Node_range<Expr> get_arguments() { return get_children().tail(); }
  /// Returns the arguments to the function call.
  
  Node_range<Expr const> get_arguments() const { return get_children().tail(); }
  /// Returns the arguments to the function call.
};

inline
Call_expr::Call_expr(Type* t, std::initializer_list<Expr*> list)
  : Kary_expr(call_expr, t, list)
{ }

inline
Call_expr::Call_expr(Type* t, std::vector<Expr*> const& vec)
  : Kary_expr(call_expr, t, vec)
{ }

inline
Call_expr::Call_expr(Type* t, std::vector<Expr*>&& vec)
  : Kary_expr(call_expr, t, std::move(vec))
{ }


/// Represents the implicit conversion of a reference to a value.
class Value_conv : public Unary_expr
{
public:
  Value_conv(Type* t, Expr* e1);
  /// Construct the value-conversion to `t`.

  Expr* get_source() const { return get_child(0); }
  /// Return the source expression.
};

inline
Value_conv::Value_conv(Type* t, Expr* e1)
  : Unary_expr(value_conv, t, e1)
{ }


// Operations

void print_expr(Printer& p, Expr const* e);
/// Print `d` using the given printer.

void debug_expr(Printer& p, Expr const* e);
/// Print debug information about `d` using the printer.

std::ostream& operator<<(std::ostream& os, Expr const& e);
/// Write `d` to the output stream.
