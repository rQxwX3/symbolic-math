#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

// Forward declaration
template <typename T> class Expression;

enum class TokenType {
  Value,
  Variable,
  Operator,
  Function,
  ParenthesisLeft,
  ParenthesisRight,
};

struct Token {
  TokenType type;
  std::string strValue;

  Token(TokenType type, std::string strValue) : type {type}, strValue {strValue} {}
};

namespace parser {
std::vector<Token> tokenize(const std::string &strExpr);

Token nextToken(std::vector<Token>& tokens);

template <typename T>
Expression<T> parseExpression(std::vector<Token>& tokens);

template <typename T>
Expression<T> parseTerm(std::vector<Token>& tokens);

template <typename T>
Expression<T> parseFactor(std::vector<Token>& tokens);

template <typename T>
Expression<T> parsePrimary(std::vector<Token>& tokens);
}; // namespace parser

#include "../src/parser.tpp"

#endif 
