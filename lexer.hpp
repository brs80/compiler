#pragma once

#include "token.hpp"

#include <unordered_map>

class Lexer
{
public:
  Lexer(Symbol_table& syms, char const* first, char const* limit);
  /// Constructs the lexer.
  
  Lexer(Symbol_table& syms, std::string const& str);
  /// Constructs the lexer for `str`.

  Token get_next_token();
  /// Returns the next token in the input buffer.

private:
  bool is_eof(char const* ptr) const { return ptr == m_limit; }
  /// True if we've consumed all input.

  bool is_eof() const { return is_eof(m_first); }
  /// True if we've consumed all input.

  char peek() const;
  /// Returns the current character.

  char peek(int n) const;
  /// Returns the nth character past the current character.

  char consume() { return *m_first++; }
  // Returns the current character, increments
  // the current input character (lookahead).

  Token match(Token::Name n, int len);
  /// Match the token.

  Token match_word();

  Token match_number();

private:
  Symbol_table* m_syms;

  char const* m_first;
  char const* m_limit;

  int m_line;

  std::unordered_map<std::string, Token::Name> m_kws;
};
