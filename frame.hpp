#pragma once

#include "store.hpp"

/// Represents a stack frame or activation record in the call stack. Each
/// frame has a local store containing the local, automatic variables for
/// its corresponding function definition.
class Frame
{
public:
  Frame(Frame* prev, Decl* fn);
  /// Constructs the stack frame.

  Frame* get_caller() const { return m_prev; }
  /// Returns the frame for the calling function or null if this is the
  /// entry point.

  Decl* get_function() const { return m_fn; }
  /// Returns the called function.

  int get_index() const { return m_index; }
  /// Returns the index of the stack frame.

  Monotonic_store& get_locals() { return m_locals; }
  /// Returns the automatic store for local variables.

  Object* allocate_local(Decl* d);
  /// Allocates a local object.

  Object* locate_local(Decl* d);
  /// Returns the object defined by `d` in this frame.

  void alias_local(Decl* d, Object* obj);
  /// Create an alias for `obj` in this frame.

private:
  Frame* m_prev;
  /// The frame of the calling function.

  Decl* m_fn;
  /// The function being called.

  int m_index;
  /// The index of the call stack (i.e., its depth).

  Monotonic_store m_locals;
  /// The automatic storage for the stack frame.
};

inline
Frame::Frame(Frame* prev, Decl* d)
  : m_prev(prev), m_fn(d), m_index(prev ? (prev->m_index + 1) : 0)
{ }


/// The call stack is a vector of frames. 
class Call_stack 
{
public:
  Call_stack();

  Frame* push(Decl* fn);
  /// Push a new frame for onto the call stack.

  void pop();
  /// Pop the active frame from the call stack.

  Frame* get_top() const { return m_top; }
  /// Returns the top frame on the stack.

  Frame* get_frame(int n) const { return m_frames[n]; }
  /// Returns the nth frame on the stack.

private:
  Frame* m_top;
  /// Points to the current frame.

  std::vector<Frame*> m_frames;
  /// Indexes into the call stack for efficient object location.
};

inline
Call_stack::Call_stack()
  : m_top(), m_frames()
{ }