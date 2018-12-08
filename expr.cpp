#include "expr.hpp"

char const* 
Expr::get_kind_name() const
{
  switch (m_kind) {
  case bool_lit: return "bool_lit";
  case int_lit: return "int_lit";
  case float_lit: return "float_lit";
  case id_expr: return "id_expr";
  case add_expr: return "add_expr";
  case sub_expr: return "sub_expr";
  case mul_expr: return "mul_expr";
  case div_expr: return "div_expr";
  case rem_expr: return "rem_expr";
  case neg_expr: return "neg_expr";
  case rec_expr: return "rec_expr";
  case eq_expr: return "eq_expr";
  case ne_expr: return "ne_expr";
  case lt_expr: return "lt_expr";
  case gt_expr: return "gt_expr";
  case le_expr: return "le_expr";
  case ge_expr: return "ge_expr";
  case cond_expr: return "cond_expr";
  case and_expr: return "and_expr";
  case or_expr: return "or_expr";
  case not_expr: return "not_expr";
  case assign_expr: return "assign_expr";
  case call_expr: return "call_expr";
  case value_conv: return "value_conv";
  };
}
