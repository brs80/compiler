#pragma once

#include "symbol.hpp"
#include "location.hpp"

/// A token (in the language) is a name and a
/// set of attributes including its lexeme and
/// its source location. The name of a toke is
/// the name of the abstract symbol.
class Token
{
public:
  enum Name
  {
    // misc
    eof,

    // punctuation
    lbrace,
    rbrace,
    lparen,
    rparen,
    colon,
    semicolon,
    comma,
    arrow,

    // operators
    plus,
    minus,
    star,
    slash,
    percent,
    question,
    equal,
    equal_equal,
    bang_equal,
    less,
    greater,
    less_equal,
    greater_equal,

    // keywords
    and_kw,
    bool_kw,
    break_kw,
    continue_kw,
    else_kw,
    false_kw,
    fun_kw,
    if_kw,
    int_kw,
    not_kw,
    or_kw,
    ref_kw,
    return_kw,
    true_kw,
    var_kw,
    while_kw,

    // literals
    integer_literal,
    float_literal,
    
    // identifiers
    identifier,
  };

  Token()
    : Token(eof, Symbol())
  { }

  Token(Name n, Symbol sym, Location = {})
    : m_name(n), m_lex(sym), m_loc()
  { }

  // Operators

  explicit operator bool() const { return m_name != eof; }
  /// Converts to true when this is not eof.

  // Token name

  Name get_name() const { return m_name; }
  /// Returns the name of the token.

  bool is_keyword() const;
  /// Returns true if this is a keyword.

  bool is_logical_operator() const;
  /// Returns true if this is a logical operator.

  // Token attributes

  Symbol get_lexeme() const { return m_lex; }
  /// Returns the lexeme (spelling) of the token.

private:
  Name m_name;
  Symbol m_lex;
  Location m_loc;
};

std::ostream& operator<<(std::ostream& os, Token const& tok);

