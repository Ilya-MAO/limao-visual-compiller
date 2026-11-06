#pragma once
#include "fwd.hpp"
#include "signal.hpp"
#include "token/token.hpp"

class Lexer {
  void saveToken(Token& token) noexcept;
  void saveKeyToken(string_view subtext, size_t& caret, Token& tokenBuffer) noexcept;
  bool saveKeyTokenIf(string_view subtext, string_view text, size_t& caret, Token& tokenBuffer) noexcept;
  void saveExtraToken(string_view subtext, size_t& caret, Token& tokenBuffer) noexcept;
  bool saveExtraTokenIf(string_view subtext, string_view text, size_t& caret, Token& tokenBuffer) noexcept;
  void saveEndLineToken(Token& tokenBuffer) noexcept;
  void saveTokenBetween(string_view text, size_t& caret, char endSymbol, Token& tokenBuffer);

public:
  void tokenize(string_view source);

  [[nodiscard]] Tokens tokens() noexcept { return m_tokens; }
  [[nodiscard]] const Tokens& tokens() const noexcept { return m_tokens; }

// ===== SIGNALS =====

  Signal<Tokens> tokensChanged{};

  Signal<int> lineOutOfRange{};
  Signal<int> indexOutOfRange{};

  Signal<std::string_view, int, int> wrongTokenValue{};
  Signal<int> absenceEndSymbol{};

private:
  Tokens m_tokens{};
};