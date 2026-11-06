#pragma once
#include <cstdint>
#include <string>

using std::string, std::string_view;

enum TokenType : uint8_t {
  TOK_I8, TOK_I16, TOK_I32, TOK_I64,
  TOK_U8, TOK_U16, TOK_U32, TOK_U64,
  TOK_F32, TOK_F64,
  TOK_STR, TOK_TUPL, TOK_SET, TOK_USET, TOK_MAP, TOK_UMAP,
  
  TOK_NUMBER_LITERAL, TOK_STRING_LITERAL, TOK_IDENTIFIER,
  TOK_FUNC, TOK_IF, TOK_ELSE_IF, TOK_ELSE, TOK_RETURN,

  TOK_PLUS, TOK_MINUS, TOK_STAR, TOK_SLASH, TOK_EQUALS, TOK_MORE, TOK_LESS,
  TOK_COLON, TOK_SEMICOLON, TOK_COMMA, TOK_DOT, TOK_TILDE, TOK_QMARK, TOK_EMARK,

  TOK_ARROW, TOK_CONNECT, TOK_DISCONNECT, TOK_CONST,
  TOK_LPAREN, TOK_RPAREN, TOK_LBRACKET, TOK_RBRACKET,

  TOK_UNDEFINED, TOK_END_LINE, TOK_END_FILE
};

class Token {
public:
  static constexpr uint16_t MAX_COORDS = 65'000;

  explicit Token() noexcept = default;
  explicit Token(string value, uint16_t line, uint16_t index, TokenType type) noexcept;

  [[nodiscard]] static TokenType valueToType(string_view value) noexcept;

  [[nodiscard]] bool isEmpty() const noexcept { return m_value.empty(); }
  [[nodiscard]] string_view value() const noexcept { return m_value; }
  [[nodiscard]] uint16_t line() const noexcept { return m_line; }
  [[nodiscard]] uint16_t index() const noexcept { return m_index; }
  [[nodiscard]] TokenType type() const noexcept { return m_type; }

  void setValue(string value);
  void pushSymbol(char symbol);
  void setLine(uint16_t line) noexcept { m_line = line; }
  void setIndex(uint16_t index) noexcept { m_index = index; }
  void setType(TokenType type) noexcept { m_type = type; }

  void clear() noexcept;

private:
  string m_value{};
  uint16_t m_line{1};
  uint16_t m_index{1};
  TokenType m_type{TOK_UNDEFINED};
};