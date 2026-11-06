#include <algorithm>
#include <array>
#include "token.hpp"  

Token::Token(string value, uint16_t line, uint16_t index, TokenType type) noexcept
  : m_value(std::move(value)), m_line(line), m_index(index), m_type(type)
{}

TokenType Token::valueToType(string_view value) noexcept {
  struct TokenMap {
    std::string_view key;
    TokenType type;
  };

  static constexpr std::array mappings = {
    TokenMap{"i8",  TokenType::TOK_I8},
    TokenMap{"i16", TokenType::TOK_I16},
    TokenMap{"i32", TokenType::TOK_I32},
    TokenMap{"i64", TokenType::TOK_I64},

    TokenMap{"u8",  TokenType::TOK_U8},
    TokenMap{"u16", TokenType::TOK_U16},
    TokenMap{"u32", TokenType::TOK_U32},
    TokenMap{"u64", TokenType::TOK_U64},

    TokenMap{"f32", TokenType::TOK_F32},
    TokenMap{"f64", TokenType::TOK_F64},

    TokenMap{"str",  TokenType::TOK_STR},
    TokenMap{"tupl", TokenType::TOK_TUPL},
    TokenMap{"set",  TokenType::TOK_SET},
    TokenMap{"uset", TokenType::TOK_USET},
    TokenMap{"map",  TokenType::TOK_MAP},
    TokenMap{"umap", TokenType::TOK_UMAP},

    TokenMap{"+",    TokenType::TOK_PLUS},
    TokenMap{"-",    TokenType::TOK_MINUS},
    TokenMap{"*",    TokenType::TOK_STAR},
    TokenMap{"/",    TokenType::TOK_SLASH},
    TokenMap{"=",    TokenType::TOK_EQUALS},
    TokenMap{">",    TokenType::TOK_MORE},
    TokenMap{"<",    TokenType::TOK_LESS},

    TokenMap{":",    TokenType::TOK_COLON},
    TokenMap{";",    TokenType::TOK_SEMICOLON},
    TokenMap{",",    TokenType::TOK_COMMA},
    TokenMap{".",    TokenType::TOK_DOT},
    TokenMap{"~",    TokenType::TOK_TILDE},
    TokenMap{"?",    TokenType::TOK_QMARK},
    TokenMap{"!",    TokenType::TOK_EMARK},

    TokenMap{"->",   TokenType::TOK_ARROW},
    TokenMap{"~>",   TokenType::TOK_CONNECT},
    TokenMap{"~!>",  TokenType::TOK_DISCONNECT},
    TokenMap{"(",    TokenType::TOK_LPAREN},
    TokenMap{")",    TokenType::TOK_RPAREN},
    TokenMap{"[",    TokenType::TOK_LBRACKET},
    TokenMap{"]",    TokenType::TOK_RBRACKET},
    TokenMap{"#",    TokenType::TOK_CONST}
  };

  auto it = std::find_if(mappings.begin(), mappings.end(),
    [value](const TokenMap& tm) { return tm.key == value; }
  );
  if (it != mappings.end()) return it->type;

  return TokenType::TOK_UNDEFINED;
}

void Token::setValue(string value) {
  if (m_value == value) return;

  m_value = std::move(value);
}

void Token::pushSymbol(char symbol) {
  m_value.push_back(symbol);
}

void Token::clear() noexcept {
  m_value.clear();
  m_type = TOK_UNDEFINED;
}