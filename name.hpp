#pragma once

#include <iosfwd>

class Printer;


/// Represents the name of a declaration.
class Name
{
public:
  Name(const char* str);
  /// Constructs a name with the given spelling.

  char const* get_string() const { return m_str; }
  /// Returns the spelling of the name.

private:
  char const* m_str;
  /// The text of the name.
};

inline
Name::Name(char const* str)
  : m_str(str)
{ }


// Operations

void print_name(Printer& p, Name const* n);
/// Print `d` using the given printer.


std::ostream& operator<<(std::ostream& os, Name const& n);
/// Write `d` to the output stream.

