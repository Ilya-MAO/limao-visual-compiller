#include <algorithm>
#include <optional>
#include <cctype>
#include "lexer.hpp"
#include "token/token.hpp"

using std::string, std::string_view;

namespace {
  [[nodiscard]] constexpr std::optional<char> getSymbolIf(string_view text, size_t caret) noexcept {
    if (caret >= text.size()) {
      return std::nullopt;
    }
    return text[caret];
  }

  [[nodiscard]] bool nextSubtextIs(string_view subtext, string_view text, size_t caret) noexcept {
    if (caret >= text.size()) {
      return false;
    }
    const string_view rest = text.substr(caret);

    if (!rest.starts_with(subtext)) {
      return false;
    }

    if (rest.size() > subtext.size()) {
      const char nextChar = rest[subtext.size()];
      if (static_cast<bool>(std::isalnum(static_cast<unsigned char>(nextChar))) || nextChar == '_') {
        return false;
      }
    }
    return true;
  }

  void skipComment(string_view text, size_t& caret) noexcept {
    const size_t endComment = text.find('\n', caret);
    if (endComment == string_view::npos) {
      caret = text.size() - 1;
    }
    caret = endComment - 1;
  }

  [[nodiscard]] size_t readNextNumber(string_view text, size_t caret, Token& tokenBuffer) noexcept {
    tokenBuffer.setType(TOK_NUMBER_LITERAL);

    bool hasDot{};
    for (size_t i = caret; i < text.size(); ++i) {
      const char symbol = text[i];

      switch (symbol) {
        case ' ': case '\t': case '\n': case '\r': case ';': 
        case ',': case ')': case ']': case '}': case '+': 
        case '-': case '*': case '/': case '=': case '>':
        case '<':
          return i - 1;

        case '.':
          if (hasDot) return i - 1;
          hasDot = true;

        case '0' ... '9':
          tokenBuffer.pushSymbol(symbol);
          continue;
      }
    }
    return caret + 1;
  }

  [[nodiscard]] bool isEmptyType(Token& token) noexcept {
    return token.isEmpty() && token.type() == TOK_UNDEFINED;
  }
}

void Lexer::saveToken(Token& token) noexcept {
  if (token.isEmpty()) return;

  uint16_t index = token.index();
  m_tokens.push_back(std::move(token));

  token.clear();
  token.setIndex(index + 1);
}

void Lexer::saveKeyToken(string_view subtext, size_t& caret, Token& tokenBuffer) noexcept {
  tokenBuffer.setValue(subtext.data());
  tokenBuffer.setType(Token::valueToType(subtext));

  saveToken(tokenBuffer);
  caret += subtext.size() - 1;
};

[[nodiscard]] bool Lexer::saveKeyTokenIf(string_view subtext, string_view text, size_t& caret, Token& tokenBuffer) noexcept {
  if (!nextSubtextIs(subtext, text, caret)) return false;

  tokenBuffer.setValue(subtext.data());
  tokenBuffer.setType(Token::valueToType(subtext));

  saveToken(tokenBuffer);
  caret += subtext.size() - 1;
  return true;
};

void Lexer::saveExtraToken(string_view subtext, size_t& caret, Token& tokenBuffer) noexcept {
  saveToken(tokenBuffer);
  saveKeyToken(subtext, caret, tokenBuffer);
}

[[nodiscard]] bool Lexer::saveExtraTokenIf(string_view subtext, string_view text, size_t& caret, Token& tokenBuffer) noexcept {
  if (!nextSubtextIs(subtext, text, caret)) return false;
  
  saveToken(tokenBuffer);
  saveKeyToken(subtext, caret, tokenBuffer);
  return true;
}

void Lexer::saveEndLineToken(Token& tokenBuffer) noexcept {
  saveToken(tokenBuffer);
  if (!m_tokens.empty() && m_tokens.back().type() != TOK_END_LINE) {
    m_tokens.emplace_back("END_LINE", tokenBuffer.line(), tokenBuffer.index(), TOK_END_LINE);
  }

  tokenBuffer.setLine(tokenBuffer.line() + 1);
  tokenBuffer.setIndex(1);
}

void Lexer::saveTokenBetween(string_view text, size_t& caret, char endSymbol, Token& tokenBuffer) {
  saveToken(tokenBuffer);

  const size_t start = caret + 1;
  const size_t endSubtext = text.find(endSymbol, start);
  const size_t endLine = text.find('\n', start);

  if (endSubtext != string_view::npos && (endLine == string_view::npos || endSubtext < endLine)) {
    tokenBuffer.setValue(string(text.substr(start, endSubtext - start)));
    tokenBuffer.setType(TOK_STRING_LITERAL);
    saveToken(tokenBuffer);
    caret = endSubtext;
  }
  else {
    tokenBuffer.setValue(string(text.substr(start, endLine - start)));
    tokenBuffer.setType(TOK_UNDEFINED);
    saveToken(tokenBuffer);
    caret = endLine;
    saveEndLineToken(tokenBuffer);
  }
}

void Lexer::tokenize(std::string_view text) {
  Token tokenBuffer{};

  for (size_t caret{}; caret < text.size(); ++caret) {
    const char symbol = text[caret];
      
    // Keysymbols
    switch (symbol) {
      case '+': case '*': case '=': case '>': case '<':
      case ':': case ';': case ',': case '~': case '?':
      case '!': case '#': case '(': case ')': case '[':
      case ']': case '.':
        saveExtraToken(string{symbol}, caret, tokenBuffer);
        continue;

      case ' ':
      case '\t':
      case '\r':
        saveToken(tokenBuffer);
        continue;

      case '\n':
        saveEndLineToken(tokenBuffer);
        continue;

      case '"':
        saveTokenBetween(text, caret, '"', tokenBuffer);
        continue;

      case '-':
        if (saveExtraTokenIf("->", text, caret, tokenBuffer)) continue;
        saveExtraToken("-", caret, tokenBuffer);
        continue;

      case '/':
        if (nextSubtextIs("//", text, caret)) {
          saveToken(tokenBuffer);
          skipComment(text, caret);
          continue;
        }
        saveExtraToken("/", caret, tokenBuffer);
        continue;
    }

    if (isEmptyType(tokenBuffer)) {
      tokenBuffer.setType(TOK_IDENTIFIER);

      switch (symbol) {
        case 'i':
          if      (saveKeyTokenIf("i8",  text, caret, tokenBuffer)) continue;
          else if (saveKeyTokenIf("i16", text, caret, tokenBuffer)) continue;
          else if (saveKeyTokenIf("i32", text, caret, tokenBuffer)) continue;
          else if (saveKeyTokenIf("i64", text, caret, tokenBuffer)) continue;
          break;

        case 'u':
          if      (saveKeyTokenIf("u8",   text, caret, tokenBuffer)) continue;
          else if (saveKeyTokenIf("u16",  text, caret, tokenBuffer)) continue;
          else if (saveKeyTokenIf("u32",  text, caret, tokenBuffer)) continue;
          else if (saveKeyTokenIf("u64",  text, caret, tokenBuffer)) continue;
          else if (saveKeyTokenIf("uset", text, caret, tokenBuffer)) continue;
          else if (saveKeyTokenIf("umap", text, caret, tokenBuffer)) continue;
          break;

        case 'f':
          if      (saveKeyTokenIf("f32", text, caret, tokenBuffer)) continue;
          else if (saveKeyTokenIf("f64", text, caret, tokenBuffer)) continue;
          break;

        case 's':
          if      (saveKeyTokenIf("str", text, caret, tokenBuffer)) continue;
          else if (saveKeyTokenIf("set", text, caret, tokenBuffer)) continue;
          break;

        case 'm':
          if      (saveKeyTokenIf("map", text, caret, tokenBuffer)) continue;
          break;

        case '0' ... '9':
          caret = readNextNumber(text, caret, tokenBuffer);
          continue;
      }
    }
    tokenBuffer.pushSymbol(symbol);
  }

  if (!tokenBuffer.isEmpty()) saveToken(tokenBuffer);
  m_tokens.emplace_back("END_FILE", tokenBuffer.line(), tokenBuffer.index(), TOK_END_FILE);

  tokensChanged.emit(m_tokens);
}