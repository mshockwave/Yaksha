// codefiles.cpp
#include "codefiles.h"
#include "ast/parser.h"
#include "compiler/type_checker.h"
#include "tokenizer/block_analyzer.h"
#include "utilities/error_printer.h"
using namespace yaksha;
codefiles::codefiles()
    : current_path_(std::filesystem::current_path()), prefixes_(),
      path_to_fi_() {}
codefiles::~codefiles() {
  for (auto f : files_) {
    if (f->data_ != nullptr) {
      delete (f->data_->parser_);
      delete (f->data_->type_checker_);
      delete (f->data_->dsv_);
      delete (f->data_->block_analyzer_);
      delete (f->data_->tokenizer_);
    }
    delete (f->data_);
    delete (f);
  }
}
file_info *codefiles::scan_main(const std::string &filename) {
  std::error_code err{};
  auto path = std::filesystem::absolute(std::filesystem::path(filename), err);
  if (err) {
    print_file_not_found_error(path);
    return nullptr;
  }
  if (!std::filesystem::exists(path, err)) {
    print_file_not_found_error(path);
    return nullptr;
  }
  if (err) {
    print_file_not_found_error(path);
    return nullptr;
  }
  current_path_ = path.parent_path();
  if (path_to_fi_.find(path) != path_to_fi_.end()) {// not found
    return path_to_fi_[path];
  }
  auto p = parse(path);
  if (p == nullptr) {
    print_file_not_found_error(path);
    return nullptr;
  }
  auto fi = new file_info{path, "yy__", p};
  path_to_fi_.insert({path, fi});
  prefixes_.insert(std::string{"yy__"});
  files_.emplace_back(fi);
  for (auto imp : fi->data_->parser_->import_stmts_) {
    auto import_data = scan(imp);
    if (import_data == nullptr) { return nullptr; }
    imp->data_ = import_data;
  }
  return path_to_fi_[path];
}
file_info *codefiles::scan(import_stmt *st) {
  auto p = std::filesystem::path{current_path_};
  std::vector<token *> names = st->import_names_;
  for (auto name_tok : names) {
    auto name = name_tok->token_;
    p /= name; /* equal to path join with '/' */
  }
  p += ".yaka"; /* add .yaka file extension */
  std::error_code err{};
  auto path = std::filesystem::absolute(p, err);
  if (err) {
    print_file_not_found_error(path);
    return nullptr;
  }
  if (!std::filesystem::exists(path, err)) {
    print_file_not_found_error(path);
    return nullptr;
  }
  if (err) {
    print_file_not_found_error(path);
    return nullptr;
  }
  auto parsed_data = parse(path);
  if (parsed_data == nullptr) {
    print_file_not_found_error(path);
    return nullptr;
  }
  // if we have it in our map we return that
  if (path_to_fi_.find(path) != path_to_fi_.end()) { return path_to_fi_[path]; }
  auto nm = names.back();
  std::string prefix = "yy__" + nm->token_;
  std::string cur_prefix = prefix + "_";
  unsigned int count = 1;
  while (has_prefix(cur_prefix)) {
    cur_prefix = prefix + std::to_string(count) + "_";
    count++;
  }
  auto fi = new file_info{path, cur_prefix, parsed_data};
  prefixes_.insert(std::string{cur_prefix});
  path_to_fi_.insert({path, fi});
  files_.emplace_back(fi);
  for (auto imp : fi->data_->parser_->import_stmts_) {
    auto import_data = scan(imp);
    if (import_data == nullptr) { return nullptr; }
    imp->data_ = import_data;
  }
  return path_to_fi_[path];
}
bool codefiles::has_prefix(std::string &root) {
  return prefixes_.find(root) != prefixes_.end();
}
file_data *codefiles::parse(std::filesystem::path &file_name) {
  std::ifstream script_file(file_name);
  if (!script_file.good()) {
    std::cerr << "Failed to read file:" << file_name << "\n";
    return nullptr;
  }
  std::string data((std::istreambuf_iterator<char>(script_file)),
                   std::istreambuf_iterator<char>());
  auto *t = new tokenizer{file_name, data};
  t->tokenize();
  if (!t->errors_.empty()) {
    errors::print_errors(t->errors_);
    delete (t);
    return nullptr;
  }
  auto *b = new block_analyzer{t->tokens_};
  b->analyze();
  if (!b->errors_.empty()) {
    errors::print_errors(b->errors_);
    delete (t);
    delete (b);
    return nullptr;
  }
  auto *p = new parser(file_name, b->tokens_, &pool_);
  p->parse();
  if (!p->errors_.empty()) {
    errors::print_errors(p->errors_);
    delete (t);
    delete (b);
    delete (p);
    return nullptr;
  }
  return new file_data{p, t, b};
}
file_info *codefiles::get(const std::string &f) {
  if (path_to_fi_.find(f) == path_to_fi_.end()) return nullptr;
  return path_to_fi_[f];
}
void codefiles::print_file_not_found_error(const std::string &filepath) {
  std::cerr << "File not found or unable to read:" << filepath << "\n";
}
