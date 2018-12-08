#pragma once

#include "tree.hpp"
#include "value.hpp"

class Printer;


/// Represents the types in the language.
class Type
{
public:
  enum Kind
  {
    bool_type,
    int_type,
    float_type,
    ref_type,
    fn_type,
  };

protected:
  Type(Kind k);

public:
  // Queries

  Kind get_kind() const { return m_kind; }
  /// Returns the kind of type.

  char const* get_kind_name() const;
  /// Returns the spelling of the kind.

  bool is_bool() const { return m_kind == bool_type; }
  /// Returns true if this is `bool`.
  
  bool is_int() const { return m_kind == int_type; }
  /// Returns true if this is `int`.

  bool is_integral() const { return is_bool() || is_int(); }
  /// Returns true if this is an integral type.
  
  bool is_float() const { return m_kind == float_type; }
  /// Returns true if this is `float`.
  
  bool is_reference() const { return m_kind == ref_type; }
  /// Returns true if this is a reference type `ref t`.
  
  bool is_function() const { return m_kind == fn_type; }
  /// Returns true if this is a function type `(t1, t2, ..., n-1)->tn`.

  bool is_object() const  { return m_kind != ref_type; }
  /// Returns true if this is an object type.

  bool is_arithmetic() const;
  /// Returns true if this is an arithmetic type.

  bool is_same_as(Type const* that) const;
  /// Returns true if this is the same type as that.

  bool is_reference_to(Type const* that) const;
  /// Returns true if this is a reference to that.

  // Children

  virtual Node_range<Type> get_children() = 0;
  /// Returns the children of the expression.

  virtual Node_range<Type const> get_children() const = 0;
  /// Returns the children of the expression.

  // Debugging

  void debug() const;
  /// Emit debugging information about this type.

private:
  Kind m_kind;
  /// The kind of expression.
};

inline
Type::Type(Kind k)
  : m_kind(k)
{ }

inline bool
Type::is_arithmetic() const
{
  return is_int() || is_float();
}


// General purpose expression classes

/// Represents nullary types (i.e., base types or leaf nodes).
class Nullary_type : public Type, public Static_arity_node<Type, 0>
{
  using Base = Static_arity_node<Type, 0>;
protected:
  Nullary_type(Kind k);

public:
  Node_range<Type> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Type const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Nullary_type::Nullary_type(Kind k)
  : Type(k), Base()
{ }


/// Represents unary type constructors (i.e., types with one operand).
class Unary_type : public Type, public Static_arity_node<Type, 1>
{
  using Base = Static_arity_node<Type, 1>;
protected:
  Unary_type(Kind k, Type* e);
  /// Constructs the expression with the given arguments.

public:
  Node_range<Type> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Type const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Unary_type::Unary_type(Kind k, Type* e)
  : Type(k), Base(e)
{ }


/// Represents k-ary type constructors (i.e., nodes with k children).
class Kary_type : public Type, public Dynamic_arity_node<Type>
{
  using Base = Dynamic_arity_node<Type>;
protected:
  Kary_type(Kind k, std::initializer_list<Type*> list);
  /// Constructs the expression with the given arguments.
  
  Kary_type(Kind k, std::vector<Type*> const& vec);
  /// Constructs the expression with the given arguments.
  
  Kary_type(Kind k, std::vector<Type*>&& vec);
  /// Constructs the expression with the given arguments.

public:
  Node_range<Type> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Type const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Kary_type::Kary_type(Kind k, std::initializer_list<Type*> list)
  : Type(k), Base(list)
{ }

inline
Kary_type::Kary_type(Kind k, std::vector<Type*> const& vec)
  : Type(k), Base(vec)
{ }

inline
Kary_type::Kary_type(Kind k, std::vector<Type*>&& vec)
  : Type(k), Base(std::move(vec))
{ }


// Base types


/// The type `bool`.
class Bool_type : public Nullary_type
{
public:
  Bool_type();
  /// Constructs the type `bool`.
};

inline
Bool_type::Bool_type()
  : Nullary_type(bool_type)
{ }


/// The type `int`.
class Int_type : public Nullary_type
{
public:
  Int_type();
  /// Constructs the type `int`.
};

inline
Int_type::Int_type()
  : Nullary_type(int_type)
{ }


/// The type `float`.
class Float_type : public Nullary_type
{
public:
  Float_type();
  /// Constructs the type `float`.
};

inline
Float_type::Float_type()
  : Nullary_type(float_type)
{ }


// Type constructors

/// Represents types of the form `ref t`.
class Ref_type : public Unary_type
{
public:
  Ref_type(Type* t);
  /// Constructs the type `ref t`.

  Type* get_object_type() const { return get_child(0); }
  /// Returns type of object type referenced.
};

inline
Ref_type::Ref_type(Type* t)
  : Unary_type(ref_type, t)
{ }


/// Represents function types of the form `(t1, t2, ..., tn-1) -> tn`.
class Fn_type : public Kary_type
{
public:
  Fn_type(std::initializer_list<Type*> list);
  /// Construct the expression `e(e1, e2, ..., en)`.
  
  Fn_type(std::vector<Type*> const& vec);
  /// Construct the expression `e(e1, e2, ..., en)`.
  
  Fn_type(std::vector<Type*>&& vec);
  /// Construct the expression `e(e1, e2, ..., en)`.

  std::size_t get_num_parameters() const { return get_children().size() - 1; }
  /// Returns the number of parameters.

  Node_range<Type> get_parameter_types() { return get_children().rtail(); }
  /// Returns the parameter types of this type.

  Node_range<Type const> get_parameter_types() const { return get_children().rtail(); }
  /// Returns the parameter types of this type.

  Type* get_return_type() { return get_children().back(); }
  /// Returns the return type of this type.

  Type const* get_return_type() const { return get_children().back(); }
  /// Returns the return type of this type.
};

inline
Fn_type::Fn_type(std::initializer_list<Type*> list)
  : Kary_type(fn_type, list)
{ }

inline
Fn_type::Fn_type(std::vector<Type*> const& vec)
  : Kary_type(fn_type, vec)
{ }

inline
Fn_type::Fn_type(std::vector<Type*>&& vec)
  : Kary_type(fn_type, std::move(vec))
{ }


// Operations

bool is_same(Type const* a, Type const* b);
/// Returns true if `a` and `b` are the same type.

void print_type(Printer& p, Type const* t);
/// Print `t` using the given printer.

void debug_type(Printer& p, Type const* t);
/// Print debugging information about `t`.

std::ostream& operator<<(std::ostream& os, Type const& t);
/// Write `t` to the output stream.

