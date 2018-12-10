#pragma once

#include "tree.hpp"
#include "value.hpp"

class Expr;
class Decl;
class Printer;


/// Represents the statements of the language.
class Stmt
{
public:
  enum Kind
  {
    skip_stmt,
    block_stmt,
    if_stmt,
    while_stmt,
    break_stmt,
    cont_stmt,
    ret_stmt,
    expr_stmt,
    decl_stmt,
  };

protected:
  Stmt(Kind k);

public:
  // Queries

  Kind get_kind() const { return m_kind; }
  /// Returns the kind of type.

  char const* get_kind_name() const;
  /// Returns the spelling of the kind.

  // Children

  virtual Node_range<Stmt> get_children() = 0;
  /// Returns the children of the expression.

  virtual Node_range<Stmt const> get_children() const = 0;
  /// Returns the children of the expression.

  // Debugging

  void debug() const;
  /// Emit debugging information about this statement.

private:
  Kind m_kind;
  /// The kind of expression.
};

inline
Stmt::Stmt(Kind k)
  : m_kind(k)
{ }

// General purpose expression classes

/// Represents nullary statements.
class Nullary_stmt : public Stmt, public Static_arity_node<Stmt, 0>
{
  using Base = Static_arity_node<Stmt, 0>;
protected:
  Nullary_stmt(Kind k);

public:
  Node_range<Stmt> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Stmt const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Nullary_stmt::Nullary_stmt(Kind k)
  : Stmt(k), Base()
{ }


/// Represents unary statements.
class Unary_stmt : public Stmt, public Static_arity_node<Stmt, 1>
{
  using Base = Static_arity_node<Stmt, 1>;
protected:
  Unary_stmt(Kind k, Stmt* s1);
  /// Constructs the statement with the given arguments.

public:
  Node_range<Stmt> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Stmt const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Unary_stmt::Unary_stmt(Kind k, Stmt* s1)
  : Stmt(k), Base(s1)
{ }


/// Represents binary statements.
class Binary_stmt : public Stmt, public Static_arity_node<Stmt, 2>
{
  using Base = Static_arity_node<Stmt, 2>;
protected:
  Binary_stmt(Kind k, Stmt* s1, Stmt* s2);
  /// Constructs the statement with the given arguments.

public:
  Node_range<Stmt> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Stmt const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Binary_stmt::Binary_stmt(Kind k, Stmt* s1, Stmt* s2)
  : Stmt(k), Base(s1, s2)
{ }


/// Represents k-ary statement.
class Kary_stmt : public Stmt, public Dynamic_arity_node<Stmt>
{
  using Base = Dynamic_arity_node<Stmt>;
protected:
  Kary_stmt(Kind k, std::initializer_list<Stmt*> list);
  /// Constructs the expression with the given arguments.
  
  Kary_stmt(Kind k, std::vector<Stmt*> const& vec);
  /// Constructs the expression with the given arguments.
  
  Kary_stmt(Kind k, std::vector<Stmt*>&& vec);
  /// Constructs the expression with the given arguments.

public:
  Node_range<Stmt> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Stmt const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Kary_stmt::Kary_stmt(Kind k, std::initializer_list<Stmt*> list)
  : Stmt(k), Base(list)
{ }

inline
Kary_stmt::Kary_stmt(Kind k, std::vector<Stmt*> const& vec)
  : Stmt(k), Base(vec)
{ }

inline
Kary_stmt::Kary_stmt(Kind k, std::vector<Stmt*>&& vec)
  : Stmt(k), Base(std::move(vec))
{ }



// Statements

/// Represents statements of the form `skip`.
class Skip_stmt : public Nullary_stmt
{
public:
  Skip_stmt();
};

inline
Skip_stmt::Skip_stmt()
  : Nullary_stmt(skip_stmt)
{ }


/// Represents statements of the form `{ s1; s2; ... sn; }`
class Block_stmt : public Kary_stmt
{
public:
  Block_stmt(std::initializer_list<Stmt*> list);
  /// Constructs the statement `{ stmts }`.

  Block_stmt(std::vector<Stmt*> const& vec);
  /// Constructs the statement `{ stmts }`.

  Block_stmt(std::vector<Stmt*>&& vec);
  /// Constructs the statement `{ stmts }`.
};

inline
Block_stmt::Block_stmt(std::initializer_list<Stmt*> list)
  : Kary_stmt(block_stmt, list)
{ }

inline
Block_stmt::Block_stmt(std::vector<Stmt*> const& vec)
  : Kary_stmt(block_stmt, vec)
{ }

inline
Block_stmt::Block_stmt(std::vector<Stmt*>&& vec)
  : Kary_stmt(block_stmt, std::move(vec))
{ }


/// Represents statements of the form `if (e) s1 else s2.
class If_stmt : public Binary_stmt
{
public:
  If_stmt(Expr* c, Stmt* s1, Stmt* s2);

  Expr* get_condition() const { return m_cond; }
  /// Returns the condition.

  Stmt* get_true_statement() const { return get_child(0); }
  /// Returns statement to execute when the condition is true.

  Stmt* get_false_statement() const { return get_child(1); }
  /// Returns statement to execute when the condition is false.

private:
  Expr* m_cond;
};

inline
If_stmt::If_stmt(Expr* c, Stmt* s1, Stmt* s2)
  : Binary_stmt(if_stmt, s1, s2)
{ }


/// Represents statements of the form `while (e) s`.
class While_stmt : public Unary_stmt
{
public:
  While_stmt(Expr* e, Stmt* s1);

  Expr* get_condition() const { return m_cond; }
  /// Returns the condition.

  Stmt* get_body() const { return get_child(0); }
  /// Returns loop body.

private:
  Expr* m_cond;
};

inline
While_stmt::While_stmt(Expr* e, Stmt* s1)
  : Unary_stmt(while_stmt, s1), m_cond(e)
{ }


/// Represents statements of the form `break`.
class Break_stmt : public Nullary_stmt
{
public:
  Break_stmt();
};

inline
Break_stmt::Break_stmt()
  : Nullary_stmt(break_stmt)
{ }


/// Represents statements of the form `continue`.
class Cont_stmt : public Nullary_stmt
{
public:
  Cont_stmt();
};

inline
Cont_stmt::Cont_stmt()
  : Nullary_stmt(cont_stmt)
{ }


/// Represents statements of the form `return e`.
///
/// \todo Does it make sense to link to the return object?
class Ret_stmt : public Nullary_stmt
{
public:
  Ret_stmt(Expr* e);

  Expr* get_return_value() const { return m_ret; }
  /// Returns the return value.

private:
  Expr* m_ret;
};

inline
Ret_stmt::Ret_stmt(Expr* e)
  : Nullary_stmt(ret_stmt), m_ret(e)
{ }


/// Represents statements of the form `e;`.
class Expr_stmt : public Nullary_stmt
{
public:
  Expr_stmt(Expr* e);

  Expr* get_expression() const { return m_expr; }
  /// Returns the expression to evaluate.

private:
  Expr* m_expr;
  /// The expression to evaluate.
};

inline
Expr_stmt::Expr_stmt(Expr* e)
  : Nullary_stmt(expr_stmt), m_expr(e)
{ }


/// Represents statements of the form `d`.
class Decl_stmt : public Nullary_stmt
{
public:
  Decl_stmt(Decl* e);

  Decl* get_declaration() const { return m_decl; }
  /// Returns the expression to evaluate.

private:
  Decl* m_decl;
  /// The expression to evaluate.
};

inline
Decl_stmt::Decl_stmt(Decl* d)
  : Nullary_stmt(decl_stmt), m_decl(d)
{ }


// Operations

void print_stmt(Printer& p, Stmt const* s);
/// Print `s` using the given printer.

void debug_stmt(Printer& p, Stmt const* s);
/// Print debugging information about `s`.

std::ostream& operator<<(std::ostream& os, Stmt const& s);
/// Write `s` to the output stream.
