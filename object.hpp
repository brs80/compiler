#pragma once

#include "value.hpp"

#include <utility>

class Type;


/// An object occupies a region of storage and holds a value.
///
/// \todo If we support aggregate objects than an object is either a scalar
/// or an aggregate with subobjects.
class Object
{
public:
  Object(Type* t);
  /// Constructs the object with indeterminate value.

  Object(Type* t, Value const& val);
  /// Constructs the object.

  Object(Type* t, Value&& val);
  /// Constructs the object.

  Type* get_type() const { return m_type; }
  /// Returns the type of the object.

  bool is_initialized() const { return !m_value.is_indeterminate(); }
  /// Returns true if the object is uninitialized.

  bool is_uninitialized() const { return m_value.is_indeterminate(); }
  /// Returns true if the object is uninitialized.

  Value const& load() const { return m_value; }
  /// Returns the value of the object.

  void initialize(Value const& val);
  /// Performs the initial store of the object.

  void initialize(Value&& val);
  /// Performs the initial store of the object.

  void store(Value const& val);
  /// Sets the value of the initialized object.

  void store(Value&& val);
  /// Sets the value of the initialized object.

private:
  Type* m_type;
  /// The type of object.
  
  Value m_value;
  /// The value of the object.
};

inline
Object::Object(Type* t)
  : m_type(t), m_value()
{ }

inline
Object::Object(Type* t, Value const& val)
  : m_type(t), m_value(val)
{ }

inline
Object::Object(Type* t, Value&& val)
  : m_type(t), m_value(std::move(val))
{ }

inline void
Object::initialize(Value const& val)
{
  assert(is_uninitialized());
  m_value = val;
}

inline void
Object::initialize(Value&& val)
{
  assert(is_uninitialized());
  m_value = std::move(val);
}

inline void
Object::store(Value const& val)
{
  assert(is_initialized());
  m_value = val; 
}

inline void
Object::store(Value&& val)
{
  assert(is_initialized());
  m_value = std::move(val); 
}
