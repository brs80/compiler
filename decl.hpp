#pragma once

#include "tree.hpp"

class Name;
class Type;
class Ref_type;
class Fn_type;
class Expr;
class Stmt;
class Decl;
class Value_decl;
class Var_decl;
class Printer;


/// Represents the base class of all declarations.
class Decl
{
public:
  enum Kind
  {
    prog_decl,
    var_decl,
    fn_decl,
  };

protected:
  Decl(Kind k);

public:
  // Queries
  
  Kind get_kind() const { return m_kind; }
  /// Returns the kind of declaration.

  char const* get_kind_name() const;
  /// Returns the spelling of the kind.

  bool is_variable() const { return m_kind == var_decl; }
  /// Returns true if this is a variable (i.e., an object or reference).

  bool is_object() const;
  /// Returns true if this declares an object.
  
  bool is_reference() const;
  /// Returns true if this declares a reference.
  
  bool is_function() const { return m_kind == fn_decl; }
  /// Returns true if this declares a function.

  virtual Name* get_name() const { return nullptr; }
  /// Returns the name of the declaration, if any.
  
  virtual Type* get_type() const { return nullptr; }  
  /// Returns the type of the declaration, if any.

  // Children

  virtual Node_range<Decl> get_children() = 0;
  /// Returns the children of the expression.

  virtual Node_range<Decl const> get_children() const = 0;
  /// Returns the children of the expression.

  // Casting

  Value_decl* get_as_value();
  /// Returns this cast as a value declaration or null.

  Value_decl const* get_as_value() const;
  /// Returns this cast as a value declaration or null.

  // Debugging

  void debug() const;

private:
  Kind m_kind;
  /// The kind of declaration.
};

inline
Decl::Decl(Kind k)
  : m_kind(k)
{ }


// Helper classes

/// A nullary declaration has no children.
class Nullary_decl : public Decl, public Static_arity_node<Decl, 0>
{
  using Base = Static_arity_node<Decl, 0>;
protected:
  Nullary_decl(Kind k);

public:
  Node_range<Decl> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Decl const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Nullary_decl::Nullary_decl(Kind k)
  : Decl(k), Base()
{ }


/// A Kary declaration has k children.
class Kary_decl : public Decl, public Dynamic_arity_node<Decl>
{
  using Base = Dynamic_arity_node<Decl>;
protected:
  Kary_decl(Kind k);
  /// Constructs the declaration with no arguments.

  Kary_decl(Kind k, std::initializer_list<Decl*> list);
  /// Constructs the declaration with the given arguments.
  
  Kary_decl(Kind k, std::vector<Decl*> const& vec);
  /// Constructs the declaration with the given arguments.
  
  Kary_decl(Kind k, std::vector<Decl*>&& vec);
  /// Constructs the declaration with the given arguments.

public:
  Node_range<Decl> get_children() { return Base::get_children(); }
  /// Returns the children of the expression.

  Node_range<Decl const> get_children() const { return Base::get_children(); }
  /// Returns the children of the expression.
};

inline
Kary_decl::Kary_decl(Kind k)
  : Decl(k), Base()
{ }

inline
Kary_decl::Kary_decl(Kind k, std::initializer_list<Decl*> list)
  : Decl(k), Base(list)
{ }

inline
Kary_decl::Kary_decl(Kind k, std::vector<Decl*> const& vec)
  : Decl(k), Base(vec)
{ }

inline
Kary_decl::Kary_decl(Kind k, std::vector<Decl*>&& vec)
  : Decl(k), Base(std::move(vec))
{ }


/// The base class of all declarations that can have values.
class Value_decl
{
protected:
  Value_decl(Name* n, Type* t);

public:
  Name* get_name() const { return m_name; }
  /// Returns the name of the declaration.

  Type* get_type() const { return m_type; }
  /// Returns the type of the declaration.

private:
  Name* m_name;
  /// The name of the declaration.

  Type* m_type;
  /// The type of the declaration.
};

inline
Value_decl::Value_decl(Name* n, Type* t)
  : m_name(n), m_type(t)
{ }


/// Represents variable (object and reference) declarations.
///
/// \todo We might want to store the declarator so we can add more
/// interesting forms of variable declaration.
class Var_decl : public Nullary_decl, public Value_decl
{
public:
  Var_decl(Name* n, Type* t);
  /// Construct a variable with the given name and type.

  Expr* get_initializer() const { return m_init; }
  /// Returns the initializer of the declaration.

  void set_initializer(Expr* e);
  /// Sets the initializer of the variable.

  Name* get_name() const override { return Value_decl::get_name(); }
  /// Returns the name of the declaration, if any.

  Type* get_type() const override { return Value_decl::get_type(); }  
  /// Returns the type of the declaration, if any.

private:
  Expr* m_init;
  /// The initializer of the declaration.
};

inline 
Var_decl::Var_decl(Name* n, Type* t)
  : Nullary_decl(var_decl), Value_decl(n, t), m_init()
{ }

inline void
Var_decl::set_initializer(Expr* e)
{
  assert(!m_init);
  m_init = e;
}


/// Represents declarations of the form `fun x (<decl-seq>) -> t s`.
///
/// A function is inherently declares a record-like object. Each parameter,
/// the return value, and each local variable contributes to the layout of
/// function's frame. When a function is activated, storage is allocated for
/// each local object in the frame.
class Fn_decl : public Kary_decl, public Value_decl
{
public:
  Fn_decl(Name* n, Type* t);
  /// Constructs the function with the given arguments.
  
  Name* get_name() const override { return Value_decl::get_name(); }
  /// Returns the name of the declaration, if any.

  Type* get_type() const override { return Value_decl::get_type(); }  
  /// Returns the type of the declaration, if any.

  Fn_type* get_function_type() const;
  /// Returns the type of this declaration.

  // Parameters

  std::size_t get_num_parameters() const;
  /// Returns the number of parameters.

  Node_range<Decl> get_parameters();
  /// Returns the parameters of the function.

  Node_range<Decl const> get_parameters() const;
  /// Returns the parameters of the function.

  void set_parameters(std::vector<Decl*> const& parms);
  /// Sets the parameters of the function.

  void set_parameters(std::initializer_list<Decl*> parms);
  /// Sets the parameters of the function.

  void add_parameter(Decl* d);
  /// Adds the parameter `d`.

  // Return value

  Decl* get_return();
  /// Returns the return object of the function.

  Decl const* get_return() const;
  /// Returns the return object of the function.

  void set_return(Decl* d);
  /// Sets the return object of the function.

  // Body

  Stmt* get_body() const { return m_body; }
  /// Returns the body of the function.

  void set_body(Stmt* s);
  /// Sets the body of the function.

private:
  Stmt* m_body;
  /// The body of the function.
};

inline
Fn_decl::Fn_decl(Name* n, Type* t)
  : Kary_decl(fn_decl), Value_decl(n, t), m_body()
{ }

inline  void
Fn_decl::set_body(Stmt* s)
{
  assert(!m_body);
  m_body = s;
}


/// A program declaration is a list of declarations.
class Prog_decl : public Kary_decl
{
public:
  Prog_decl(std::initializer_list<Decl*> list);
  /// Constructs a program from the given declarations.
  
  Prog_decl(std::vector<Decl*> const& vec);
  /// Constructs a vector from the given declarations.
  
  Prog_decl(std::vector<Decl*>& vec);
  /// Constructs a program from the given declarations.
};

inline
Prog_decl::Prog_decl(std::initializer_list<Decl*> list)
  : Kary_decl(prog_decl, list)
{ }
  
inline
Prog_decl::Prog_decl(std::vector<Decl*> const& vec)
  : Kary_decl(prog_decl, vec)
{ }
  
inline
Prog_decl::Prog_decl(std::vector<Decl*>& vec)
  : Kary_decl(prog_decl, vec)
{ }


// Operations

void print_decl(Printer& p, Decl const* d);
/// Print `d` using the given printer.

void debug_decl(Printer& p, Decl const* d);
/// Debug `d` using the given printer.

std::ostream& operator<<(std::ostream& os, Decl const& d);
/// Write `d` to the output stream.

