#include "frame.hpp"

#include <iostream>

Object*
Frame::allocate_local(Decl* d)
{
  return m_locals.allocate(d);
}

Object*
Frame::locate_local(Decl* d)
{
  return m_locals.locate(d);
}

void
Frame::alias_local(Decl* d, Object* obj)
{
  m_locals.alias(d, obj);
}

Frame*
Call_stack::push(Decl* fn)
{
  m_top = new Frame(m_top, fn);
  m_frames.push_back(m_top);
  return m_top;
}

void
Call_stack::pop()
{
  m_top = m_top->get_caller();
  delete m_frames.back();
  m_frames.pop_back();
}
