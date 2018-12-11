#include "builder.hpp"
#include "name.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include "decl.hpp"

Name*
Builder::get_name(char const* str)
{
  return new Name(str);
}
