#include <fstream>
#include <sstream>
#include <string>
#include "cli/cli.hpp"
#include "generator/generator.hpp"
#include "lexer/lexer.hpp"
#include "translator/translator.hpp"

using std::vector, std::ifstream;

int main(int argc, char* argv[]) {
  std::stringstream buffer{};

  if (ifstream file{argv[1]}; file.is_open()) {
    buffer << file.rdbuf();
  }
  else return 1;

  Lexer lexer{};
  Translator translator{};
  Generator generator{};

  CLI::Error::Lexer lexerErrors{};

  // Connections
  lexer.tokensChanged.connect([&translator](Tokens tokens) { translator.start(tokens); });
  translator.translationStarted.connect([&generator]() { generator.start(); });
  translator.translationFinished.connect([&generator]() { generator.finish(); });
  translator.initPairsRequested.connect([&generator](std::string type, std::vector<InitPair> pairs) { generator.declarateVariables(std::move(type), std::move(pairs)); });

  lexer.wrongTokenValue.connect([&lexerErrors](std::string_view value, int line, int index) {lexerErrors.wrongTokenValue(value, line, index); });
  lexer.absenceEndSymbol.connect([&lexerErrors](int line) {lexerErrors.absenceEndSymbol(line); });

  lexer.tokenize(buffer.str());

  CLI::Printer::tokens(lexer.tokens());
  CLI::Printer::output(generator.output());
  CLI::Printer::errors(lexerErrors);

  return 0;
}