// ----------------------------------- Tests -----------------------------------
//
#include "btest.h"
#include "catch2/catch.hpp"
#include "file_formats/tokens_file.h"
using namespace yaksha;
#define TOK_REQUIRE(POS, TEXT, TYPE)                                           \
  do {                                                                         \
    REQUIRE(t.tokens_[POS].token_ == (TEXT));                                  \
    REQUIRE(t.tokens_[POS].type_ == (TYPE));                                   \
  } while (0)
#define TEST_FILE(A, B, C)                                                     \
  do {                                                                         \
    std::ifstream code_file(A);                                                \
    if (code_file.good()) {                                                    \
      std::string code((std::istreambuf_iterator<char>(code_file)),            \
                       std::istreambuf_iterator<char>());                      \
      try {                                                                    \
        yaksha::tokenizer t(B, code);                                          \
        t.tokenize();                                                          \
        auto token_snapshot = yaksha::load_token_dump(C);                      \
        yaksha::save_token_dump(C, t.tokens_);                                 \
        REQUIRE(t.tokens_.size() == token_snapshot.size());                    \
        for (int i = 0; i < token_snapshot.size(); i++) {                      \
          auto parsed = t.tokens_[i];                                          \
          auto snapshot = token_snapshot[i];                                   \
          REQUIRE(parsed.file_ == snapshot.file_);                             \
          REQUIRE(parsed.line_ == snapshot.line_);                             \
          REQUIRE(parsed.pos_ == snapshot.pos_);                               \
          REQUIRE(parsed.token_ == snapshot.token_);                           \
          REQUIRE(parsed.type_ == snapshot.type_);                             \
        }                                                                      \
      } catch (parsing_error & e) {                                            \
        DBGPRINT(e.message_);                                                  \
        REQUIRE(false);                                                        \
      }                                                                        \
    } else {                                                                   \
      REQUIRE(false);                                                          \
    }                                                                          \
  } while (0)
TEST_CASE("tokenizer: Names: Basic") {
  yaksha::tokenizer t("test.py", "a banana b aaaaa ");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 4 + 1);
  REQUIRE(t.tokens_[0].token_ == "a");
  REQUIRE(t.tokens_[0].type_ == token_type::NAME);
  REQUIRE(t.tokens_[1].token_ == "banana");
  REQUIRE(t.tokens_[2].token_ == "b");
  REQUIRE(t.tokens_[3].token_ == "aaaaa");
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: Names: With symbols") {
  yaksha::tokenizer t("test.py", "a + banana + aaaaa");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 5 + 1);
  REQUIRE(t.tokens_[0].token_ == "a");
  REQUIRE(t.tokens_[0].type_ == token_type::NAME);
  REQUIRE(t.tokens_[1].token_ == "+");
  REQUIRE(t.tokens_[1].type_ == token_type::PLUS);
  REQUIRE(t.tokens_[2].token_ == "banana");
  REQUIRE(t.tokens_[2].type_ == token_type::NAME);
  REQUIRE(t.tokens_[3].token_ == "+");
  REQUIRE(t.tokens_[4].token_ == "aaaaa");
  REQUIRE(t.tokens_[4].type_ == token_type::NAME);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: Indentation") {
  yaksha::tokenizer t("test.py", "a\n    banana + aaaaa ");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 6 + 1);
  REQUIRE(t.tokens_[0].token_ == "a");
  REQUIRE(t.tokens_[0].type_ == token_type::NAME);
  REQUIRE(t.tokens_[1].token_ == "\n");
  REQUIRE(t.tokens_[1].type_ == token_type::NEW_LINE);
  REQUIRE(t.tokens_[2].token_ == "    ");
  REQUIRE(t.tokens_[2].type_ == token_type::INDENT);
  REQUIRE(t.tokens_[3].token_ == "banana");
  REQUIRE(t.tokens_[3].type_ == token_type::NAME);
  REQUIRE(t.tokens_[4].token_ == "+");
  REQUIRE(t.tokens_[4].type_ == token_type::PLUS);
  REQUIRE(t.tokens_[5].token_ == "aaaaa");
  REQUIRE(t.tokens_[5].type_ == token_type::NAME);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: Indentation: Mixed") {
  yaksha::tokenizer t("test.py", "a\n \t   banana + aaaaa ");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 6 + 1);
  REQUIRE(t.tokens_[0].token_ == "a");
  REQUIRE(t.tokens_[0].type_ == token_type::NAME);
  REQUIRE(t.tokens_[1].token_ == "\n");
  REQUIRE(t.tokens_[1].type_ == token_type::NEW_LINE);
  REQUIRE(t.tokens_[2].token_ == " \t   ");
  REQUIRE(t.tokens_[2].type_ == token_type::INDENT);
  REQUIRE(t.tokens_[3].token_ == "banana");
  REQUIRE(t.tokens_[3].type_ == token_type::NAME);
  REQUIRE(t.tokens_[4].token_ == "+");
  REQUIRE(t.tokens_[4].type_ == token_type::PLUS);
  REQUIRE(t.tokens_[5].token_ == "aaaaa");
  REQUIRE(t.tokens_[5].type_ == token_type::NAME);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: Symbols: Basic") {
  yaksha::tokenizer t("test.py", "@ : ( ) < = > | + - * / & ^ % , [ ] { }");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 20 + 1);
  TOK_REQUIRE(0, "@", token_type::AT);
  TOK_REQUIRE(1, ":", token_type::COLON);
  TOK_REQUIRE(2, "(", token_type::PAREN_OPEN);
  TOK_REQUIRE(3, ")", token_type::PAREN_CLOSE);
  TOK_REQUIRE(4, "<", token_type::LESS);
  TOK_REQUIRE(5, "=", token_type::EQ);
  TOK_REQUIRE(6, ">", token_type::GREAT);
  TOK_REQUIRE(7, "|", token_type::OR);
  TOK_REQUIRE(8, "+", token_type::PLUS);
  TOK_REQUIRE(9, "-", token_type::SUB);
  TOK_REQUIRE(10, "*", token_type::MUL);
  TOK_REQUIRE(11, "/", token_type::DIV);
  TOK_REQUIRE(12, "&", token_type::AND);
  TOK_REQUIRE(13, "^", token_type::XOR);
  TOK_REQUIRE(14, "%", token_type::MOD);
  TOK_REQUIRE(15, ",", token_type::COMMA);
  TOK_REQUIRE(16, "[", token_type::SQUARE_BRACKET_OPEN);
  TOK_REQUIRE(17, "]", token_type::SQUARE_BRACKET_CLOSE);
  TOK_REQUIRE(18, "{", token_type::CURLY_BRACKET_OPEN);
  TOK_REQUIRE(19, "}", token_type::CURLY_BRACKET_CLOSE);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: Symbols: Multi character symbols") {
  yaksha::tokenizer t("test.py",
                      "<= == >= |= += -= *= /= &= ^= %= ** **= // //= ... ->");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 17 + 1);
  TOK_REQUIRE(0, "<=", token_type::LESS_EQ);
  TOK_REQUIRE(1, "==", token_type::EQ_EQ);
  TOK_REQUIRE(2, ">=", token_type::GREAT_EQ);
  TOK_REQUIRE(3, "|=", token_type::OR_EQ);
  TOK_REQUIRE(4, "+=", token_type::PLUS_EQ);
  TOK_REQUIRE(5, "-=", token_type::SUB_EQ);
  TOK_REQUIRE(6, "*=", token_type::MUL_EQ);
  TOK_REQUIRE(7, "/=", token_type::DIV_EQ);
  TOK_REQUIRE(8, "&=", token_type::AND_EQ);
  TOK_REQUIRE(9, "^=", token_type::XOR_EQ);
  TOK_REQUIRE(10, "%=", token_type::MOD_EQ);
  TOK_REQUIRE(11, "**", token_type::POWER);
  TOK_REQUIRE(12, "**=", token_type::POWER_EQ);
  TOK_REQUIRE(13, "//", token_type::INT_DIV);
  TOK_REQUIRE(14, "//=", token_type::INT_DIV_EQ);
  TOK_REQUIRE(15, "...", token_type::ELLIPSIS);
  TOK_REQUIRE(16, "->", token_type::ARROW);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: String: Basic") {
  yaksha::tokenizer t("test.py", "a = \"banana\"");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 3 + 1);
  REQUIRE(t.tokens_[0].token_ == "a");
  REQUIRE(t.tokens_[0].type_ == token_type::NAME);
  REQUIRE(t.tokens_[1].token_ == "=");
  REQUIRE(t.tokens_[1].type_ == token_type::EQ);
  REQUIRE(t.tokens_[2].token_ == "banana");
  REQUIRE(t.tokens_[2].type_ == token_type::STRING);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: String: With escape") {
  yaksha::tokenizer t("test.py", R"(a = "\"ban\"ana\"")");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 3 + 1);
  REQUIRE(t.tokens_[0].token_ == "a");
  REQUIRE(t.tokens_[0].type_ == token_type::NAME);
  REQUIRE(t.tokens_[1].token_ == "=");
  REQUIRE(t.tokens_[1].type_ == token_type::EQ);
  REQUIRE(t.tokens_[2].token_ == "\\\"ban\\\"ana\\\"");
  REQUIRE(t.tokens_[2].type_ == token_type::STRING);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: String: End with escaped") {
  yaksha::tokenizer t("test.py", R"(a = "\"ban\"ana\")");
  t.tokenize();
  REQUIRE(!t.errors_.empty());
  REQUIRE(t.errors_[0].tok_.file_ == "test.py");
  REQUIRE(t.errors_[0].tok_.line_ == 0);
}
TEST_CASE("tokenizer: String: End with new line") {
  yaksha::tokenizer t("test.py", "a = \"\n");
  t.tokenize();
  REQUIRE(t.errors_.size() == 1);
  REQUIRE(t.errors_[0].message_ ==
          "Tokenizer Error : Invalid string, new line detected "
          "while parsing string");
}
TEST_CASE("tokenizer: String: Does not end") {
  yaksha::tokenizer t("test.py", "a = \"");
  t.tokenize();
  REQUIRE(t.errors_.size() == 1);
  REQUIRE(t.errors_[0].message_ == "Tokenizer Error : Invalid end of file");
}
TEST_CASE("tokenizer: String: Does not end after another string") {
  yaksha::tokenizer t("test.py", R"(a = "\"ban\"ana\""")");
  t.tokenize();
  DBGPRINT(t.tokens_.size());
  DBGPRINT(t.tokens_[0].token_);
  DBGPRINT(t.tokens_[1].token_);
  DBGPRINT(t.tokens_[2].token_);
  REQUIRE(!t.errors_.empty());
}
TEST_CASE("tokenizer: String: Empty string") {
  yaksha::tokenizer t("test.py", "a = \"\"");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 3 + 1);
  REQUIRE(t.tokens_[2].token_.empty());
  REQUIRE(t.tokens_[2].type_ == token_type::STRING);
}
TEST_CASE("tokenizer: Numbers: single number") {
  yaksha::tokenizer t("test.py", "123");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 1 + 1);
  REQUIRE(t.tokens_[0].token_ == "123");
  REQUIRE(t.tokens_[0].type_ == token_type::INTEGER_DECIMAL);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: Numbers: Bunch of numbers") {
  yaksha::tokenizer t("test.py",
                      "0xABCDEF0 0b1111001 "
                      "0o123456 1.2e3 123 1.2e+3 1.2e-3 1f 1.2f 1.2e1f");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 10 + 1);
  REQUIRE(t.tokens_[0].token_ == "0xABCDEF0");
  REQUIRE(t.tokens_[0].type_ == token_type::INTEGER_HEX);
  REQUIRE(t.tokens_[1].token_ == "0b1111001");
  REQUIRE(t.tokens_[1].type_ == token_type::INTEGER_BIN);
  REQUIRE(t.tokens_[2].token_ == "0o123456");
  REQUIRE(t.tokens_[2].type_ == token_type::INTEGER_OCT);
  REQUIRE(t.tokens_[3].token_ == "1.2e3");
  REQUIRE(t.tokens_[3].type_ == token_type::DOUBLE_NUMBER);
  REQUIRE(t.tokens_[4].token_ == "123");
  REQUIRE(t.tokens_[4].type_ == token_type::INTEGER_DECIMAL);
  REQUIRE(t.tokens_[5].token_ == "1.2e+3");
  REQUIRE(t.tokens_[5].type_ == token_type::DOUBLE_NUMBER);
  REQUIRE(t.tokens_[6].token_ == "1.2e-3");
  REQUIRE(t.tokens_[6].type_ == token_type::DOUBLE_NUMBER);
  REQUIRE(t.tokens_[7].token_ == "1f");
  REQUIRE(t.tokens_[7].type_ == token_type::FLOAT_NUMBER);
  REQUIRE(t.tokens_[8].token_ == "1.2f");
  REQUIRE(t.tokens_[8].type_ == token_type::FLOAT_NUMBER);
  REQUIRE(t.tokens_[9].token_ == "1.2e1f");
  REQUIRE(t.tokens_[9].type_ == token_type::FLOAT_NUMBER);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: Comment: Basic") {
  yaksha::tokenizer t("test.py", R"(a = "" # Hello World "")");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 4 + 1);
  REQUIRE(t.tokens_[3].token_ == " Hello World \"\"");
  REQUIRE(t.tokens_[3].type_ == token_type::COMMENT);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: Comment: Two lines") {
  yaksha::tokenizer t("test.py", "a = \"\" # Hello\n# World");
  t.tokenize();
  REQUIRE(t.tokens_.size() == 6 + 1);
  REQUIRE(t.tokens_[3].token_ == " Hello");
  REQUIRE(t.tokens_[3].type_ == token_type::COMMENT);
  REQUIRE(t.tokens_[4].token_ == "\n");
  REQUIRE(t.tokens_[4].type_ == token_type::NEW_LINE);
  REQUIRE(t.tokens_[5].token_ == " World");
  REQUIRE(t.tokens_[5].type_ == token_type::COMMENT);
  REQUIRE(t.errors_.empty());
}
TEST_CASE("tokenizer: Parse test_vector_add.py") {
  TEST_FILE("../test_data/test_vector_add.py", "test_vector_add.py",
            "../test_data/test_vector_add.py.tokens");
}
TEST_CASE("tokenizer: Parse unicode_test.py") {
  TEST_FILE("../test_data/unicode_test.py", "unicode_test.py",
            "../test_data/unicode_test.py.tokens");
}
TEST_CASE("tokenizer: Parse bad_test.py") {
  TEST_FILE("../test_data/bad_test.py", "bad_test.py",
            "../test_data/bad_test.py.tokens");
}
