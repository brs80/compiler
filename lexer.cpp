#include "lexer.hpp"

#include <iostream>
#include <sstream>

static bool 
is_nondigit(char c)
{
  return std::isalpha(c) || c == '_';
}

static bool
is_digit(char c)
{
  return std::isdigit(c);
}

static bool
is_nondigit_or_digit(char c)
{
  return is_nondigit(c) || is_digit(c);
}

static bool
is_hexdigit(char c)
{
  return std::isxdigit(c);
}

Lexer::Lexer(Symbol_table& syms,
             char const* first,
             char const* limit)
  : m_syms(&syms),
    m_first(first),
    m_limit(limit),
    m_line(1)
{
  m_kws.emplace("and", Token::and_kw);
  m_kws.emplace("bool", Token::bool_kw);
  m_kws.emplace("break", Token::break_kw);
  m_kws.emplace("continue", Token::continue_kw);
  m_kws.emplace("else", Token::else_kw);
  m_kws.emplace("false", Token::false_kw);
  m_kws.emplace("fun", Token::fun_kw);
  m_kws.emplace("if", Token::if_kw);
  m_kws.emplace("int", Token::int_kw);
  m_kws.emplace("not", Token::not_kw);
  m_kws.emplace("or", Token::or_kw);
  m_kws.emplace("ref", Token::ref_kw);
  m_kws.emplace("return", Token::return_kw);
  m_kws.emplace("true", Token::true_kw);
  m_kws.emplace("var", Token::var_kw);
  m_kws.emplace("while", Token::while_kw);
}

Lexer::Lexer(Symbol_table& syms, std::string const& str)
  : Lexer(syms, str.data(), str.data() + str.size())
{ }

Token
Lexer::get_next_token()
{
  while (!is_eof()) {
    switch (peek()) {
    case ' ':
    case '\t':
      consume();
      continue;

    case '\n':
      ++m_line;
      consume();
      continue;

    case '{':
      return match(Token::lbrace, 1);
    case '}':
      return match(Token::rbrace, 1);
    case '(':
      return match(Token::lparen, 1);
    case ')':
      return match(Token::rparen, 1);
    case ',':
      return match(Token::comma, 1);
    case ';':
      return match(Token::semicolon, 1);
    case ':':
      return match(Token::colon, 1);
    case '+':
      return match(Token::plus, 1);
    case '-':
      return match(Token::minus, 1);
    case '*':
      return match(Token::star, 1);
    case '/':
      return match(Token::slash, 1);
    case '%':
      return match(Token::percent, 1);
    case '<':
      if (peek(1) == '=')
        return match(Token::less_equal, 2);
      return match(Token::less, 1);
    case '>':
      if (peek(1) == '=')
        return match(Token::greater_equal, 2);
      return match(Token::lbrace, 1);
    case '=':
      if (peek(1) == '=')
        return match(Token::equal_equal, 2);
      return match(Token::equal, 1);
    case '!':
      if (peek(1) == '=')
        return match(Token::bang_equal, 2);
      
      // Consume the invalid input and then restart processing.
      consume();
      std::cerr << "error: " << m_line << ": " << "expected '=' after '!'\n";
      continue;

      // You could do this. But Don't.
      // throw std::runtime_error("not a token");

    default: {
      if (is_nondigit(*m_first))
        return match_word();
      
      if (is_digit(*m_first))
        return match_number();

      std::stringstream ss;
      ss << "error: " << m_line << ": "
         << "invalid character '" << (int)*m_first << "'";
      std::cerr << ss.str() << '\n';
      consume();
      continue;
    }
    }
  }

  return Token();
}

char 
Lexer::peek() const
{ 
  if (is_eof())
    return 0;
  return *m_first; 
}

char 
Lexer::peek(int n) const
{
  if (m_limit - m_first <= n)
    return 0;
  return *(m_first + n); 
}


Token
Lexer::match(Token::Name n, int len)
{
  std::string str(m_first, m_first + len);
  Symbol sym = m_syms->get(str);
  Token tok = Token(n, sym);
  // FIXME: Add source info.

  // Update the lexer.
  m_first += len;
  
  return tok;
}

Token
Lexer::match_word()
{
  char const* iter = m_first + 1;
  while (!is_eof(iter) && is_nondigit_or_digit(*iter))
    ++iter;

  // Build the token.
  std::string id(m_first, iter);
  Symbol sym = m_syms->get(id);

  // Look to see if the identifier is actually a keyword.
  Token::Name kind;
  auto lookup = m_kws.find(id);
  if (lookup == m_kws.end())
    kind = Token::identifier;
  else
    kind = lookup->second;

  // Advance the lexer
  m_first = iter;

  return Token(kind, sym);
}

Token
Lexer::match_number()
{
  char const* iter = m_first + 1;
  while (!is_eof(iter) && is_digit(*iter))
    ++iter;

  // Build the token.
  std::string id(m_first, iter);
  Symbol sym = m_syms->get(id);

  // Advance the lexer
  m_first = iter;

  return Token(Token::integer_literal, sym);
}