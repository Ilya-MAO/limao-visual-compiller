#pragma once
#include <sstream>
#include "../lexer/lexer.hpp"

namespace CLI::Error {
  class Lexer {
  public:
    std::string errors() const { return m_errors.str(); }

    void wrongTokenValue(std::string_view value, int line, int index);
    void absenceEndSymbol(int line);

  private:
    std::stringstream m_errors{};
  };
}

namespace CLI {
  class Printer {
  public:
    static void tokens(const Tokens& tokens);
    static void output(std::string_view output);
    static void errors(const CLI::Error::Lexer& lexerErrors);
  };
};