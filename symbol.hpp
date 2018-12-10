#pragma once

#include <string>
#include <unordered_set>


class Symbol
{
  friend class Symbol_table;
  
  Symbol(std::string const* str) : m_str(str) { }
  /// Constructs the symbol from `str`.

public:
  Symbol() : m_str() { }

  std::string const& str() const { return *m_str; }
  /// Returns the spelling of the token.

  friend bool operator==(Symbol a, Symbol b) 
  {
    return a.m_str == b.m_str;
  }

  friend bool operator!=(Symbol a, Symbol b) 
  {
    return a.m_str != b.m_str;
  }

private:
  std::string const* m_str;  
};


class Symbol_table : std::unordered_set<std::string>
{
public:
  Symbol get(std::string const& str);
  /// Returns the unique symbol for `str`.
  
  Symbol get(char const* str);
  /// Returns the unique symbol for `str`.
};

inline Symbol
Symbol_table::get(std::string const& str)
{
  return &*emplace(str).first;
}

inline Symbol
Symbol_table::get(char const* str)
{
  return &*emplace(str).first;
}


namespace std
{
  template<>
  struct hash<::Symbol>
  {
    std::size_t operator()(::Symbol sym) const noexcept
    {
      std::hash<std::string const*> h;
      return h(&sym.str());
    }
  };
};