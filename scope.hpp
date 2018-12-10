#pragma once
#include <utility>
#include <vector>
#include <unordered_map>
#include "parser.hpp"
#include "decl.hpp"


struct Scope : std::unordered_map<Symbol, Decl*>
{
  Decl* lookup(Symbol sym)
  {
    auto iter = find(sym);
    if (iter == end())
      return nullptr;
    return iter->second;
  }

  void declare(Decl* d)
  {
    //assert(std::count(d) == 0);
    //emplace(d->get_name(), d);
  }
};


struct Scope_stack : std::vector<Scope>
{
  Decl* lookup(Symbol sym)
  {
    for (auto iter = rbegin(); iter != rend(); ++iter) {
      if (Decl * d = iter->lookup(sym))
        return d;
    }
    return nullptr;
  }

  Scope back() 
  {
    return this->back(); 
  }
};
