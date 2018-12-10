#include "store.hpp"
#include "type.hpp"
#include "decl.hpp"

#include <algorithm>
#include <iostream>

Object*
Monotonic_store::allocate(Decl* d)
{
  assert(m_lookup.count(d) == 0);
  assert(d->is_object());
  m_storage.emplace_back(d->get_type());
  m_lookup.emplace(d, m_storage.size() - 1);
  return &m_storage.back();
}

Object*
Monotonic_store::locate(Decl* d)
{
  assert(m_lookup.count(d) != 0);
  return &m_storage[m_lookup.find(d)->second];
}

static bool
contains_object(std::vector<Object> const& store, Object* obj)
{
  auto iter = std::find_if(store.begin(), store.end(), [obj](Object const& x) {
    return &x == obj;
  });
  return iter != store.end();
}

void
Monotonic_store::alias(Decl* d, Object* obj)
{
  assert(m_lookup.count(d) == 0);
  assert(contains_object(m_storage, obj));
  m_lookup.emplace(d, obj - m_storage.data());
}
