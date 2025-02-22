// delete_stack.h
#ifndef DELETE_STACK_H
#define DELETE_STACK_H
#include "ast/ast.h"
#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>
namespace yaksha {
  struct delete_stack {
    explicit delete_stack(ast_type stack_type);
    virtual ~delete_stack();
    virtual void push(const std::string &name, const std::string &free_code);
    virtual void remove(const std::string &name);
    virtual void write(std::stringstream &target, int indent_level);
    virtual void write(std::stringstream &target, int indent_level,
                       const std::string &return_name);
    const ast_type stack_type_;

private:
    static void write_indent(std::stringstream &target, int indent_level);
    static void write_end_statement(std::stringstream &target);
    std::vector<std::string> del_stack_{};
    std::map<std::string, std::string> del_map_{};
  };
}// namespace yaksha
#endif
