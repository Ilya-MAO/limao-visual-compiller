#include <algorithm>
#include <iostream>
#include <format>
#include "cli.hpp"
#include "token/token.hpp"

using std::cout, std::format;

namespace Color {
  inline constexpr const char* RESET      = "\e[0m";
  inline constexpr const char* ERROR      = "\e[1;31m";
  inline constexpr const char* IDENTIFIER = "\e[0;96m";
  inline constexpr const char* TYPE       = "\e[1;33m";
  inline constexpr const char* NUM_LIT    = "\e[1;34m";
  inline constexpr const char* STR_LIT    = "\e[1;32m";
  inline constexpr const char* KEY_WORD   = "\e[1;33m";
  inline constexpr const char* KEY_SYMB   = "\e[1;37m";
  inline constexpr const char* EXTRA      = "\e[1;35m";

  const char* token(TokenType type) {
    switch (type) {
      case TOK_I8 ... TOK_UMAP:           return TYPE;
      case TOK_NUMBER_LITERAL:            return NUM_LIT;
      case TOK_STRING_LITERAL:            return STR_LIT;
      case TOK_IDENTIFIER:                return IDENTIFIER;
      case TOK_FUNC ... TOK_RETURN:       return KEY_WORD;
      case TOK_PLUS ... TOK_RBRACKET:     return KEY_SYMB;
      case TOK_UNDEFINED:                 return ERROR;
      case TOK_END_LINE ... TOK_END_FILE: return EXTRA;
    }
  }
}

namespace CLI {
  inline constexpr const char* FORMAT_TOKEN         = "{}[{}]{} ";
  inline constexpr const char* FORMAT_COLORED_TOKEN = "{}{}![{}]{} ";

  void Printer::tokens(const Tokens& tokens) {
    cout << "\tLEXER: Tokens\n\n";

    if (tokens.empty()) return; // TODO

    struct {
      bool isFirst{true};
      void reset() noexcept { isFirst = true; }
    } subline;

    struct {
      bool contains{};
      bool isCurrentSubline{};
      void reset() noexcept { contains = isCurrentSubline = false; }
    } error;

    string lineBuffer{};
    string sublineBuffer{};

    const auto& finalizeSubline = [&]() {
      string prefix{"    | "};

      if (error.isCurrentSubline) {
        prefix = format("   {} | {}", Color::ERROR, Color::RESET);
      }

      lineBuffer += prefix + sublineBuffer + '\n';

      subline.isFirst = error.isCurrentSubline = false;
      sublineBuffer.clear();
    };

    const auto& finalizeLine = [&](int line) {
      string errorPrefix = Color::ERROR + format("{:>3}", line) + Color::RESET;
     
      error.contains
        ? lineBuffer.replace(0, 3, errorPrefix)
        : lineBuffer.replace(0, 3, format("{:>3}", line));

      cout << lineBuffer;

      subline.reset(); error.reset();
      lineBuffer.clear(); sublineBuffer.clear();
    };

    for (const auto& token : tokens) {
      const bool isError = (token.type() == TOK_UNDEFINED);

      if (isError) {
        error.contains = error.isCurrentSubline = true;
      }

      sublineBuffer += isError
        ? format(FORMAT_COLORED_TOKEN, Color::token(token.type()), token.index(), token.value(), Color::RESET)
        : format(FORMAT_TOKEN, Color::token(token.type()), token.value(), Color::RESET);

      if (token.type() == TokenType::TOK_END_LINE || token.type() == TokenType::TOK_END_FILE) {
        finalizeSubline();
        finalizeLine(token.line());
      }
      else if (token.index() % 10 == 0) {
        finalizeSubline();
      }
    }
    cout << "\n\n";
  }

  void Printer::output(std::string_view errors) {
    cout << format("\tGENERATOR: Output\n\n {} \n\n", errors);
  }

  void Printer::errors(const CLI::Error::Lexer& lexerErrors) {
    cout << format("\tISSUES: Lexer\n\n {} \n\n", lexerErrors.errors());
  }
}

namespace CLI::Error {
  void Lexer::wrongTokenValue(std::string_view value, int line, int index) {
    m_errors << format("\n[{:^3}:{:^3}]\tValue '{}' is a wrong token type", line, index, value);
  }
  
  void Lexer::absenceEndSymbol(int line) {
    m_errors << format("\n[{:^3}:   ]\tNeed to close a block in this line", line);
  }
}