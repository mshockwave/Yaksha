// ast_printer.cpp
#include "ast/ast_printer.h"
#include "tokenizer/string_utils.h"
#include <iostream>
using namespace yaksha;
void ast_printer::visit_binary_expr(binary_expr *obj) {
  parenthesize(obj->opr_->token_, {obj->left_, obj->right_});
}
void ast_printer::visit_grouping_expr(grouping_expr *obj) {
  parenthesize("group", {obj->expression_});
}
void ast_printer::visit_literal_expr(literal_expr *obj) {
  if (obj->literal_token_->type_ == token_type::THREE_QUOTE_STRING ||
      obj->literal_token_->type_ == token_type::STRING) {
    text_ << string_utils::repr_string(
        string_utils::unescape(obj->literal_token_->token_));
  } else {
    text_ << obj->literal_token_->token_;
  }
}
void ast_printer::visit_unary_expr(unary_expr *obj) {
  parenthesize(obj->opr_->token_, {obj->right_});
}
void ast_printer::visit_return_stmt(return_stmt *obj) {
  if (obj->expression_ != nullptr) {
    this->parenthesize("return", {obj->expression_});
    return;
  }
  text_ << "(return None)";
}
void ast_printer::parenthesize(const std::string &name,
                               std::initializer_list<expr *> exprs) {
  text_ << "(" << name;
  for (auto e : exprs) {
    text_ << " ";
    e->accept(this);
  }
  text_ << ")";
}
void ast_printer::visit_expression_stmt(expression_stmt *obj) {
  parenthesize("expression", {obj->expression_});
}
std::string ast_printer::print_to_str(const std::vector<stmt *> &statements) {
  for (auto statement : statements) {
    statement->accept(this);
    text_ << "\n";
  }
  text_ << "\n";
  auto st = text_.str();
  text_.clear();
  return st;
}
void ast_printer::print(const std::vector<stmt *> &statements) {
  std::cout << print_to_str(statements);
}
void ast_printer::visit_variable_expr(variable_expr *obj) {
  text_ << obj->name_->token_;
}
void ast_printer::visit_let_stmt(let_stmt *obj) {
  text_ << "(let " << obj->name_->token_;
  text_ << ":" << obj->data_type_->as_string() << " ";
  if (obj->expression_ != nullptr) { obj->expression_->accept(this); }
  text_ << ")";
}
void ast_printer::visit_assign_expr(assign_expr *obj) {
  text_ << "("
        << "assign " << obj->name_->token_ << " ";
  obj->right_->accept(this);
  text_ << ")";
}
void ast_printer::visit_block_stmt(block_stmt *obj) {
  text_ << "(block";
  for (auto st : obj->statements_) {
    text_ << " ";
    st->accept(this);
  }
  text_ << ")";
}
void ast_printer::visit_if_stmt(if_stmt *obj) {
  text_ << "(if ";
  parenthesize("expr", {obj->expression_});
  text_ << " then ";
  obj->if_branch_->accept(this);
  if (obj->else_branch_ != nullptr) {
    text_ << " else ";
    obj->else_branch_->accept(this);
  }
  text_ << ")";
}
void ast_printer::visit_pass_stmt(pass_stmt *obj) { text_ << "(nop)"; }
void ast_printer::visit_logical_expr(logical_expr *obj) {
  parenthesize(obj->opr_->token_, {obj->left_, obj->right_});
}
void ast_printer::visit_while_stmt(while_stmt *obj) {
  text_ << "(while ";
  parenthesize("expr", {obj->expression_});
  text_ << " do ";
  obj->while_body_->accept(this);
  text_ << ")";
}
void ast_printer::visit_break_stmt(break_stmt *obj) { text_ << "(break)"; }
void ast_printer::visit_continue_stmt(continue_stmt *obj) {
  text_ << "(continue)";
}
void ast_printer::visit_fncall_expr(fncall_expr *obj) {
  text_ << "(call ";
  obj->name_->accept(this);
  for (auto st : obj->args_) {
    text_ << " ";
    st->accept(this);
  }
  text_ << ")";
}
void ast_printer::visit_def_stmt(def_stmt *obj) {
  if (obj->annotations_.native_) {
    // @native
    text_ << "(native_def";
  } else if (obj->annotations_.native_macro_) {
    // @nativemacro
    text_ << "(native_macro_def";
  } else {
    // normal functions
    text_ << "(def";
  }
  text_ << obj->name_->token_;
  text_ << " (";
  for (auto st : obj->params_) {
    text_ << " ";
    text_ << st.name_->token_ << ":" << st.data_type_->as_string();
  }
  text_ << ") ";
  obj->function_body_->accept(this);
  text_ << ")";
}
void ast_printer::visit_defer_stmt(defer_stmt *obj) {
  if (obj->expression_ != nullptr) {
    parenthesize("defer", {obj->expression_});
  } else {
    text_ << "(defer ";
    obj->del_statement_->accept(this);
    text_ << ")";
  }
}
void ast_printer::visit_class_stmt(class_stmt *obj) {
  text_ << "(class ";
  text_ << obj->name_->token_;
  text_ << " (";
  for (auto st : obj->members_) {
    text_ << " ";
    text_ << st.name_->token_ << ":" << st.data_type_->as_string();
  }
  text_ << "))";
}
void ast_printer::visit_del_stmt(del_stmt *obj) {
  parenthesize("del", {obj->expression_});
}
void ast_printer::visit_get_expr(get_expr *obj) {
  text_ << "(get " << obj->item_->token_;
  obj->lhs_->accept(this);
  text_ << ")";
}
void ast_printer::visit_set_expr(set_expr *obj) {
  text_ << "(set " << obj->item_->token_;
  obj->lhs_->accept(this);
  text_ << ")";
}
void ast_printer::visit_assign_member_expr(assign_member_expr *obj) {
  parenthesize("assign.", {obj->set_oper_, obj->right_});
}
void ast_printer::visit_square_bracket_access_expr(
    square_bracket_access_expr *obj) {
  parenthesize("get[]", {obj->name_, obj->index_expr_});
}
void ast_printer::visit_square_bracket_set_expr(square_bracket_set_expr *obj) {
  parenthesize("set[]", {obj->name_, obj->index_expr_});
}
void ast_printer::visit_assign_arr_expr(assign_arr_expr *obj) {
  parenthesize("assign[]", {obj->assign_oper_, obj->right_});
}
void ast_printer::visit_ccode_stmt(ccode_stmt *obj) {
  text_ << "(c "
        << string_utils::repr_string(
               string_utils::unescape(obj->code_str_->token_))
        << ")";
}
void ast_printer::visit_import_stmt(import_stmt *obj) {}
void ast_printer::visit_const_stmt(const_stmt *obj) {
  text_ << "(const " << obj->name_->token_;
  text_ << ":" << obj->data_type_->as_string() << " ";
  if (obj->expression_ != nullptr) { obj->expression_->accept(this); }
  text_ << ")";
}
void ast_printer::visit_nativeconst_stmt(nativeconst_stmt *obj) {
  text_ << "(nativeconst " << obj->name_->token_;
  text_ << ":" << obj->data_type_->as_string() << " ";
  if (obj->code_str_ != nullptr) {
    text_ << string_utils::repr_string(
        string_utils::unescape(obj->code_str_->token_));
  }
  text_ << ")";
}
void ast_printer::visit_runtimefeature_stmt(runtimefeature_stmt *obj) {
  text_ << "(feature "
        << string_utils::repr_string(
               string_utils::unescape(obj->feature_->token_))
        << ")";
}
