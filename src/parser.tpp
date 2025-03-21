#ifndef PARSER_TPP
#define PARSER_TPP

#include "../include/expression.hpp"
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iostream>

std::vector<Token> parser::tokenize(const std::string &strExpr) {
  std::vector<Token> tokens;
  std::istringstream stream(strExpr);
  char c;

  while (stream >> c) {
    if (isspace(c)) continue;

    if (isdigit(c) || c == '.') {
      std::string strValue(1, c);
      while (stream >> c && (isdigit(c) || c == '.'))
        strValue += c;
      stream.putback(c);
      tokens.push_back(Token(TokenType::Value, strValue));
    }

    else if (isalpha(c) || c == '_') {
      std::string strValue(1, c);
      while (stream >> c && (isalpha(c) || c == '_'))
        strValue += c;
      stream.putback(c);

      if (strValue == "sin" || strValue == "cos" ||
           strValue == "ln" || strValue == "exp")
      {
        tokens.push_back(Token(TokenType::Function, strValue));
      } else {
        tokens.push_back(Token(TokenType::Variable, strValue));
      }
    }

    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
      tokens.push_back(Token(TokenType::Operator, std::string(1, c)));
    }

    else if (c == '(') {
      tokens.push_back(Token(TokenType::ParenthesisLeft, std::string(1, c)));
    } else if (c == ')') {
      tokens.push_back(Token(TokenType::ParenthesisRight, std::string(1, c)));
    }
  }

  return tokens;
}

Token parser::nextToken(std::vector<Token>& tokens) {
  if (tokens.empty()) throw std::runtime_error("Unexpected end of expression");

  Token token {tokens.front()};
  tokens.erase(tokens.begin());
  return token;
}

template <typename T>
Expression<T> parser::parseExpression(std::vector<Token>& tokens) {
  Expression<T> leftOperand {parseTerm<T>(tokens)};

  while (!tokens.empty()) {
    Token operatorToken {tokens.front()};

    if (operatorToken.type == TokenType::Operator &&
      (operatorToken.strValue == "+" || operatorToken.strValue == "-")) {
      nextToken(tokens);
      Expression<T> rightOperand {parseTerm<T>(tokens)};

      if (operatorToken.strValue == "+")
        leftOperand = leftOperand + rightOperand;
      else
        leftOperand = leftOperand - rightOperand;
    } else break;
  }

  return leftOperand;
}

template <typename T>
Expression<T> parser::parseTerm(std::vector<Token>& tokens) {
  Expression<T> leftOperand {parseFactor<T>(tokens)};

  while (!tokens.empty()) {
    Token operatorToken {tokens.front()};

    if (operatorToken.type == TokenType::Operator && 
      (operatorToken.strValue == "*" || operatorToken.strValue == "/")) {

      nextToken(tokens);
      Expression<T> rightOperand {parseFactor<T>(tokens)};

      if (operatorToken.strValue == "*")
        leftOperand = leftOperand * rightOperand;
      else leftOperand = leftOperand / rightOperand;
    } else  break;
  }

  return leftOperand;
}

template <typename T>
Expression<T> parser::parseFactor(std::vector<Token>& tokens) {
  Expression<T> leftOperand {parsePrimary<T>(tokens)};

  while (!tokens.empty()) {
    Token operatorToken {tokens.front()};

    if ((operatorToken.type == TokenType::Operator) && 
        (operatorToken.strValue == "^")) {
      nextToken(tokens);
      Expression<T> rightOperand {parsePrimary<T>(tokens)};
      leftOperand = leftOperand ^ rightOperand;
    } else break;
  }

  return leftOperand;
}

template <typename T>
Expression<T> parser::parsePrimary(std::vector<Token>& tokens) {
  Token token {nextToken(tokens)};

  if (token.strValue == "-") {
    Expression<T> expr {parsePrimary<T>(tokens)};
    return -expr;
  }

  if (token.type == TokenType::Function) {
    std::string functionName {token.strValue};

    Token openParen {nextToken(tokens)};
    if (openParen.strValue != "(") {
      throw std::runtime_error(
        "Expected opening parenthesis after function name"
      );
    }

    Expression<T> argument {parseExpression<T>(tokens)};

    Token closeParen {nextToken(tokens)};
    if (closeParen.strValue != ")") {
      throw std::runtime_error(
        "Expected closing parenthesis after function arguments"
      );
    }

    if (functionName == "sin") return Expression<T>::sin(argument);
    else if (functionName == "cos") return Expression<T>::cos(argument);
    else if (functionName == "ln") return Expression<T>::ln(argument);
    else if (functionName == "exp") return Expression<T>::exp(argument);
    else throw std::runtime_error("Unknown function: " + functionName);
  }

  if (token.type == TokenType::Value)
    return Expression<T>(std::stod(token.strValue));

  if (token.type == TokenType::Variable)
    return Expression<T>(token.strValue);

  if (token.strValue == "(") {
    Expression<T> expr {parseExpression<T>(tokens)};
    Token closeParen {nextToken(tokens)};

    if (closeParen.strValue != ")")
      throw std::runtime_error("Expected closing parenthesis");

    return expr;
  }

  throw std::runtime_error("Unexpected token " + token.strValue);
}

#endif // PARSER_TPP
