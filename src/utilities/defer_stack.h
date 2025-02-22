// defer_stack.h
#ifndef DEFER_STACK_H
#define DEFER_STACK_H
#include "ast/ast.h"
#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>
namespace yaksha {
  struct defer_stack {
    explicit defer_stack(ast_type stack_type);
    virtual ~defer_stack();
    virtual void push(defer_stmt *df);
    virtual void write(stmt_visitor *statement_visitor);
    const ast_type stack_type_;

private:
    std::vector<defer_stmt *> del_stack_{};
  };
}// namespace yaksha
#endif
