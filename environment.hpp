#pragma once

#include <string>
#include <unordered_map>
#include <map>

struct FunctionNode;

struct Environment {
  std::unordered_map<std::string, int> vars;
  std::map<std::string, FunctionNode> funcs;
};
