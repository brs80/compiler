#include "name.hpp"
#include "printer.hpp"

#include <iostream>

void
print_name(Printer& p, Name const* n)
{
  p.get_stream() << n->get_string();
}


std::ostream& 
operator<<(std::ostream& os, Name const& n)
{
  Printer p(os);
  print_name(p, &n);
  return os;
}

