#pragma once

#include <cassert>
#include <iosfwd>


class Var_decl;
class Fn_decl;


/// An integer value.
using Int_value = long long;


/// A floating point value.
using Float_value = double;


/// A function value.
using Fn_value = Fn_decl*;


/// An object address is an index into a stack frame and the defining
/// declaration for the object.
///
/// \todo If we support subobjects, this will also need a path through
/// the complete object to the designated subobject.
struct Addr_value
{
  int store;
  /// The store in which the object resides. If store >= 0, the object
  /// resides the automatic store of the corresponding stack frame.
  /// If store == -1, the object resides in the static store.
  
  Var_decl* def;
  /// The defining declaration for the object. This is a key for the
  /// store containing the object.
};


/// Represents a value computed by an expression.
class Value
{
public:
  enum Kind
  {
    non_val,
    int_val,
    float_val,
    fn_val,
    addr_val,
  };

  Value();
  /// Constructs an indeterminate value.

  explicit Value(bool b);
  /// Constructs an integer value.

  explicit Value(int n);
  /// Constructs an integer value.

  explicit Value(Int_value n);
  /// Constructs an integer value.

  explicit Value(float n);
  /// Constructs an integer value.

  explicit Value(Float_value n);
  /// Constructs an integer value.

  explicit Value(Fn_value f);
  /// Constructs a function value.

  explicit Value(Addr_value const& addr);
  /// Constructs an address value.

  // Kind

  Kind get_kind() const { return m_kind; }
  /// Returns the kind of the expression.

  bool is_indeterminate() const { return m_kind == non_val; }
  /// Returns true if the value is indeterminate.

  bool is_int() const { return m_kind == int_val; }
  /// Returns true if this is an integer value.

  bool is_float() const { return m_kind == float_val; }
  /// Returns true if this is a float value.

  bool is_function() const { return m_kind == fn_val; }
  /// Returns true if this is a function value.

  bool is_address() const { return m_kind == addr_val; }
  /// Returns true if this is an address.

  // Accessors

  Int_value get_int() const;
  /// Returns the integer value.

  Float_value get_float() const;
  /// Returns the floating point value.

  Fn_value get_function() const;
  /// Returns the function value.

  Addr_value const& get_address() const;
  /// Returns the address value.

private:
  Kind m_kind;
  /// The kind of value.

  union Data {
    Data() = default;
    Data(Int_value n) : num(n) { }
    Data(Float_value n) : fp(n) { }
    Data(Fn_value f) : fn(f) { }
    Data(Addr_value a) : addr(a) { }
    
    Int_value num;
    Float_value fp;
    Fn_value fn;
    Addr_value addr;
  };

  Data m_data;
  /// The actual value.
};

inline
Value::Value()
  : m_kind(non_val)
{ }

inline
Value::Value(bool b)
  : Value((Int_value)b)
{ }

inline
Value::Value(int n)
  : Value((Int_value)n)
{ }

inline
Value::Value(Int_value n)
  : m_kind(int_val), m_data(n)
{ }

inline
Value::Value(float n)
  : Value((Float_value)n)
{ }

inline
Value::Value(Float_value n)
  : m_kind(float_val), m_data(n)
{ }

inline
Value::Value(Fn_value f)
  : m_kind(fn_val), m_data(f)
{ }

inline
Value::Value(Addr_value const& val)
  : m_kind(addr_val), m_data(val)
{ }

inline Int_value
Value::get_int() const
{
  assert(is_int());
  return m_data.num;
}

inline Float_value
Value::get_float() const
{
  assert(is_float());
  return m_data.fp;
}

inline Fn_value
Value::get_function() const
{
  assert(is_function());
  return m_data.fn;
}

inline Addr_value const&
Value::get_address() const
{
  assert(is_address());
  return m_data.addr;
}


// Operations

std::ostream& operator<<(std::ostream& os, Value const& val);
