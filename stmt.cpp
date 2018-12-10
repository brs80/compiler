#include "stmt.hpp"

char const*
Stmt::get_kind_name() const
{
  switch (m_kind) {
  case skip_stmt: return "skip_stmt";
  case block_stmt: return "block_stmt";
  case if_stmt: return "if_stmt";
  case while_stmt: return "while_stmt";
  case break_stmt: return "break_stmt";
  case cont_stmt: return "cont_stmt";
  case ret_stmt: return "ret_stmt";
  case expr_stmt: return "expr_stmt";
  case decl_stmt: return "decl_stmt";
  }
}