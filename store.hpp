#pragma once

#include "object.hpp"

#include <unordered_map>
#include <vector>

class Decl;


/// A store maintains a set of live objects. A monotonic store does not allow
/// storage to be returned incrementally; all storage is released at the same
/// time. This is used to implement the static store and automatic stores.
class Monotonic_store
{
public:
  Object* allocate(Decl* d);
  /// Allocate storage for an object.

  Object* locate(Decl* d);
  /// Returns the object for the given declaration.

  void alias(Decl* d, Object* o);
  /// Make `d` an alias for the given object. This is used to support
  /// reference binding.

private:
  std::vector<Object> m_storage;
  /// The list of allocated objects.
  
  std::unordered_map<Decl*, int> m_lookup;
  /// Associates a declaration with its corresponding object in
  /// the store.
};
