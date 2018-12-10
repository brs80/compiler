#pragma once

#include <cassert>
#include <array>
#include <vector>

// Node_ranges

/// Represents a traversable range of nodes.
template<typename T>
class Node_range
{
public:
  using iterator = T**;

  Node_range(T** f, T** l) : m_first(f), m_last(l) { }
  /// Constructs the range.

  bool empty() const { return m_first == m_last; }
  /// Returns true if this is empty.

  std::size_t size() const { return m_last - m_first; }
  /// Returns the number of elements in the range.

  T* front() const { return *m_first; }
  /// Returns the first element in the range.

  T* back() const { return *(m_last - 1); }
  /// Returns the last element in the range.

  T* at(std::size_t n) const { return *(m_first + n); }
  /// Returns the nth element in the range.

  Node_range front(std::size_t n) const { return {m_first, m_first + n}; }
  /// Returns the first n elements in the range.

  Node_range back(std::size_t n) const { return {m_last - n, m_last}; }
  /// Returns the last n elements in the range.

  T** begin() { return m_first; }
  /// Returns an iterator to the first element of the range.
  
  T** end() { return m_last; }
  /// Returns an iterator past the last element of the range.

  Node_range tail() const { return {m_first + 1, m_last}; }
  /// Returns the tail of this sequence.

  Node_range rtail() const { return {m_first, m_last - 1}; }
  /// Returns the tail of this sequence.

private:
  T** m_first;
  T** m_last;
};

/// Specialization for const ranges.
template<typename T>
class Node_range<T const>
{
public:
  using iterator = T* const*;

  Node_range(T* const* f, T* const* l) : m_first(f), m_last(l) { }
  /// Constructs the range.

  bool empty() const { return m_first == m_last; }
  /// Returns true if this is empty.

  std::size_t size() const { return m_last - m_first; }
  /// Returns the number of elements in the range.

  T const* front() const { return *m_first; }
  /// Returns the first element in the range.

  T const* back() const { return *(m_last - 1); }
  /// Returns the last element in the range.

  T* const* begin() const { return m_first; }
  /// Returns an iterator to the first element of the range.
  
  T const* at(std::size_t n) const { return *(m_first + n); }
  /// Returns the nth element in the range.

  Node_range front(std::size_t n) const { return {m_first, m_first + n}; }
  /// Returns the first n elements in the range.

  Node_range back(std::size_t n) const { return {m_last - n, m_last}; }
  /// Returns the last n elements in the range.

  T* const* end() const { return m_last; }
  /// Returns an iterator past the last element of the range.
  
  Node_range tail() const { return {m_first + 1, m_last}; }
  /// Returns the tail of this sequence.

  Node_range rtail() const { return {m_first, m_last - 1}; }
  /// Returns the tail of this sequence.

private:
  T* const* m_first;
  T* const* m_last;
};


/// Range operations

/// Returns the tail of the sequence (i.e., all but front).
template<typename T>
inline Node_range<T>
tail(std::vector<T*>& vec)
{
  assert(vec.size() > 1);
  T** first = vec.data() + 1;
  T** last = vec.data() + vec.size();
  return Node_range<T>(first, last);
}

/// Returns the reverse tail of the sequence (i.e., all but back).
template<typename T>
inline Node_range<T>
rtail(std::vector<T*>& vec)
{
  assert(vec.size() > 1);
  T** first = vec.data();
  T** last = vec.data() - 1;
  return Node_range<T>(first, last);
}


// Nodes

/// A node with a statically fixed number of operands N. The type T 
/// specifies the the type of node in the hierarchy.
template<typename T, int N>
class Static_arity_node
{
public:
  Static_arity_node();
  /// Constructs a nullary expression.
  
  Static_arity_node(T* e);
  /// Constructs a unary expression.
  
  Static_arity_node(T* e1, T* e2);
  /// Constructs a binary expression.

  Static_arity_node(T* e1, T* e2, T* e3);
  /// Constructs a ternary expression.

  // Accessors

  static constexpr int get_arity() { return N; }
  /// Returns the arity of the expression.

  T* get_child(int n) const;
  /// Returns the nth operand.

  T* get_child() const;
  /// Returns the sole operand. This is valid only when N == 1.

  // Iteration

  T** begin() { return m_ops.data(); }
  /// Returns an iterator pointing to the first operand.
  
  T** end() { return m_ops.data() + m_ops.size(); }
  /// Returns an iterator pointing past the last operand.

  T* const* begin() const { return m_ops.data(); }
  /// Returns an iterator pointing to the first operand.
  
  T* const* end() const { return m_ops.data() + m_ops.size(); }
  /// Returns an iterator pointing past the last operand.

  Node_range<T> get_children() { return {begin(), end()}; }
  /// Returns the range of children.
  
  Node_range<T const> get_children() const { return {begin(), end()}; }
  /// Returns the range of children.

private:
  std::array<T*, N> m_ops;
};

template<typename T, int N>
inline
Static_arity_node<T, N>::Static_arity_node()
  : m_ops()
{
  assert(N == 0);
}

template<typename T, int N>
inline
Static_arity_node<T, N>::Static_arity_node(T* e)
  : m_ops {e}
{
  assert(N == 1);
}

template<typename T, int N>
inline Static_arity_node<T, N>::Static_arity_node(T* e1, T* e2)
  : m_ops {e1, e2}
{
  assert(N == 2);
}

template<typename T, int N>
inline Static_arity_node<T, N>::Static_arity_node(T* e1, T* e2, T* e3)
  : m_ops {e1, e2, e3}
{
  assert(N == 3);
}

template<typename T, int N>
inline T*
Static_arity_node<T, N>::get_child(int n) const
{
  assert(0 <= n && n < N);
  return m_ops[n];
}

template<typename T, int N>
inline T*
Static_arity_node<T, N>::get_child() const
{
  static_assert(N == 1);
  return m_ops[0];
}


/// An expression with a variable number of operands.
///
/// \todo Supply an allocator so that we can ensure that all memory for
/// a single tree is allocated from the same pool.
///
/// \todo There are really two concepts here: arbitrarily, but fixed size
/// nodes and extendable nodes. The former works well for function types,
/// call expressions, tuples, etc. The latter is practically essential for
/// declarations. 
template<typename T>
class Dynamic_arity_node
{
public:
  Dynamic_arity_node();
  /// Constructs the node with no children.

  Dynamic_arity_node(std::initializer_list<T*> list);
  /// Constructs the node with the children in `list`.

  Dynamic_arity_node(std::vector<T*> const& vec);
  /// Constructs the node with the children in `vec`.

  Dynamic_arity_node(std::vector<T*>&& vec);
  /// Constructs the node with the children in `vec`.

  // Accessors

  int get_arity() const { return m_ops.size(); }
  /// Returns the arity of the expression.

  T* get_child(int n) const;
  /// Returns the nth operand of the node.

  // Extension

  void add_child(T* n) { m_ops.push_back(n); }
  /// Adds a child to the node.

  // Iteration

  T** begin() { return m_ops.data(); }
  /// Returns an iterator pointing to the first operand.
  
  T** end() { return m_ops.data() + m_ops.size(); }
  /// Returns an iterator pointing past the last operand.

  T* const* begin() const { return m_ops.data(); }
  /// Returns an iterator pointing to the first operand.
  
  T* const* end() const { return m_ops.data() + m_ops.size(); }
  /// Returns an iterator pointing past the last operand.

  Node_range<T> get_children() { return {begin(), end()}; }
  /// Returns the range of children.
  
  Node_range<T const> get_children() const { return {begin(), end()}; }
  /// Returns the range of children.

private:
  std::vector<T*> m_ops;
};

template<typename T>
inline
Dynamic_arity_node<T>::Dynamic_arity_node()
  : m_ops()
{ }

template<typename T>
inline
Dynamic_arity_node<T>::Dynamic_arity_node(std::initializer_list<T*> list)
  : m_ops(list)
{ }

template<typename T>
inline
Dynamic_arity_node<T>::Dynamic_arity_node(std::vector<T*> const& vec)
  : m_ops(vec)
{ }

template<typename T>
inline
Dynamic_arity_node<T>::Dynamic_arity_node(std::vector<T*>&& vec)
  : m_ops(std::move(vec))
{ }

template<typename T>
inline T*
Dynamic_arity_node<T>::get_child(int n) const
{
  return m_ops[n];
}


