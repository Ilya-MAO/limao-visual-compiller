#include "translator.hpp"
#include "../lexer/lexer.hpp"
#include <optional>

namespace {
  bool isEnd(TokenType type) {
    return type == TOK_END_LINE || type == TOK_END_FILE;
  }

  std::optional<Token> getToken(size_t index, Tokens& tokens) {
    if (index >= 0 && index < tokens.size()) {
      return tokens[index];
    }
    return std::nullopt;
  }

  std::vector<InitPair> getPrimitivePairs(size_t index, Tokens& tokens) {
    std::vector<InitPair> result{};
    std::string defaultValue{};
    
    if (auto type = getToken(index, tokens); type) {
      for (size_t ti = index + 1; ti < tokens.size(); ti += 3)
      {
        auto name = getToken(ti, tokens);
        auto op = getToken(ti + 1, tokens);
        auto value = getToken(ti + 2, tokens);

        // // if (!name) break; // Error

        // if (name && value && op && op->type == TOK_COLON || op->type == TOK_EQUALS) {
        //   result.push_back({ name->value, value->value });
        // }
        // else if (name) {
        //   result.push_back({ name->value, "{}" });
        // }

        // if (!op || !value) break;
      }
    }
    return result;
  }
}

void Translator::start(Tokens& tokens) {
  translationStarted.emit();

  for (size_t caret{}; caret < tokens.size(); ++caret)
  {
    // switch (tokens[caret].type) {
    //   case _TOK_PRIMITIVE ... _TOK_STRUCTURE:
    //     initPairsRequested.emit(tokens[caret].value, getPrimitivePairs(caret, tokens));
    //     break;
    // }
  }

  translationFinished.emit();
}