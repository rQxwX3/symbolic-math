#include "expression.hpp"
#include <cctype>
#include <cmath>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>

// Constructors
template <typename T>
Expression<T>::Expression(T val) : m_val{val}, m_isVal{true}, m_isVar{false} {}

template <typename T>
Expression<T>::Expression(const std::string &var)
    : m_var{var}, m_isVal{false}, m_isVar{true} {}

template <typename T>
Expression<T>::Expression(std::shared_ptr<Expression<T>> leftExpr,
                          char operation,
                          std::shared_ptr<Expression<T>> rightExpr)
    : m_isVal{false}, m_isVar{false}, m_leftExpr{leftExpr},
      m_rightExpr{rightExpr}, m_operation{operation} {}

template <typename T>
Expression<T>::Expression(std::shared_ptr<Expression<T>> leftExpr,
                          char operation)
    : m_isVal{false}, m_isVar{false}, m_leftExpr{leftExpr},
      m_rightExpr{nullptr}, m_operation{operation} {}

// Operators
template <typename T>
Expression<T> Expression<T>::operator+(const Expression<T> &other) const {
  return Expression(std::make_shared<Expression<T>>(*this), '+',
                    std::make_shared<Expression<T>>(other));
}

template <typename T>
Expression<T> Expression<T>::operator-(const Expression<T> &other) const {
  return Expression(std::make_shared<Expression<T>>(*this), '-',
                    std::make_shared<Expression<T>>(other));
}

template <typename T>
Expression<T> Expression<T>::operator*(const Expression<T> &other) const {
  return Expression(std::make_shared<Expression<T>>(*this), '*',
                    std::make_shared<Expression<T>>(other));
}

template <typename T>
Expression<T> Expression<T>::operator/(const Expression<T> &other) const {
  return Expression(std::make_shared<Expression<T>>(*this), '/',
                    std::make_shared<Expression<T>>(other));
}

template <typename T>
Expression<T> Expression<T>::operator^(const Expression<T> &other) const {
  return Expression(std::make_shared<Expression<T>>(*this), '^',
                    std::make_shared<Expression<T>>(other));
}

template <typename T> Expression<T> Expression<T>::operator-() const {
  if (m_isVar)
    return *this;
  if (m_isVal)
    return Expression<T>(-m_val);

  return Expression(std::make_shared<Expression<T>>(*this), '-', nullptr);
}

// Functions
template <typename T>
Expression<T> Expression<T>::sin(const Expression<T> &expr) {
  return Expression(std::make_shared<Expression<T>>(expr), 's', nullptr);
}

template <typename T>
Expression<T> Expression<T>::cos(const Expression<T> &expr) {
  return Expression(std::make_shared<Expression<T>>(expr), 'c', nullptr);
}

template <typename T>
Expression<T> Expression<T>::ln(const Expression<T> &expr) {
  return Expression(std::make_shared<Expression<T>>(expr), 'l', nullptr);
}

template <typename T>
Expression<T> Expression<T>::exp(const Expression<T> &expr) {
  return Expression(std::make_shared<Expression<T>>(expr), 'e', nullptr);
}

template <typename T>
Expression<T> Expression<T>::substitute(const std::string &var, T val) const {
  if (m_isVal)
    return m_val;

  if (m_isVar) {
    if (m_var == var)
      return Expression<T>(val);

    return *this;
  }

  auto leftSubstitute{m_leftExpr->substitute(var, val)},
      rightSubstitute{m_rightExpr->substitute(var, val)};

  return Expression<T>(std::make_shared<Expression<T>>(leftSubstitute),
                       m_operation,
                       std::make_shared<Expression<T>>(rightSubstitute));
}

template <typename T>
T Expression<T>::evaluate(const std::map<std::string, T> &context) const {
  if (m_isVal)
    return m_val;

  if (m_isVar) {
    auto it = context.find(m_var);
    if (it != context.end()) {
      return it->second;
    }
    throw std::runtime_error("Variable not found\n");
  }

  T leftVal{m_leftExpr->evaluate(context)},
      rightVal{m_rightExpr ? m_rightExpr->evaluate(context) : T(0)};

  switch (m_operation) {
  case '+':
    return leftVal + rightVal;
  case '-':
    return leftVal - rightVal;
  case '*':
    return leftVal * rightVal;
  case '/':
    return leftVal / rightVal;
  case '^':
    return std::pow(leftVal, rightVal);
  case 's':
    return std::sin(leftVal);
  case 'c':
    return std::cos(leftVal);
  case 'l':
    return std::log(leftVal);
  case 'e':
    return std::exp(leftVal);
  default:
    throw std::runtime_error("Unknown operator\n");
  }
}

template <typename T> std::string Expression<T>::toString() const {
  if (m_isVal)
    return std::to_string(m_val);
  if (m_isVar)
    return m_var;

  std::string leftString{m_leftExpr->toString()},
      rightString{m_rightExpr ? m_rightExpr->toString() : ""};

  switch (m_operation) {
  case '+':
    return "(" + leftString + "+" + rightString + ")";
  case '-':
    return "(" + leftString + "-" + rightString + ")";
  case '*':
    return "(" + leftString + "*" + rightString + ")";
  case '/':
    return "(" + leftString + "/" + rightString + ")";
  case '^':
    return "(" + leftString + "^" + rightString + ")";
  case 's':
    return "sin(" + leftString + ")";
  case 'c':
    return "cos(" + leftString + ")";
  case 'l':
    return "ln(" + leftString + ")";
  case 'e':
    return "exp(" + leftString + ")";
  default:
    return "";
  }
}

template <typename T>
Expression<T> Expression<T>::derivative(const std::string &var) const {
  if (m_isVal)
    return Expression<T>(0);

  if (m_isVar) {
    if (m_var == var)
      return Expression<T>(1);
    else
      return Expression<T>(0);
  }

  switch (m_operation) {
  case '+':
    return m_leftExpr->derivative(var) + m_rightExpr->derivative(var);
  case '-':
    return m_leftExpr->derivative(var) - m_rightExpr->derivative(var);
  case '*':
    return m_leftExpr->derivative(var) * *m_rightExpr +
           *m_leftExpr * m_rightExpr->derivative(var);
  case '/':
    return (m_leftExpr->derivative(var) * *m_rightExpr -
            *m_leftExpr * m_rightExpr->derivative(var)) /
           (*m_rightExpr * *m_rightExpr);
  case '^':
    if (m_leftExpr->m_isVar && m_leftExpr->m_var == var) {
      return *m_rightExpr * *m_leftExpr ^ (*m_rightExpr - Expression<T>(1));
    }
  case 's':
    return cos(*m_leftExpr).derivative(var);
  case 'c':
    return -sin(*m_leftExpr).derivative(var);
  case 'l':
    return Expression<T>(1) / *m_leftExpr;
  case 'e':
    return exp(*m_leftExpr).derivative(var);
  default:
    throw std::runtime_error("Unsupported operation for derivative\n");
  }
}

template <typename T> Expression<T> Expression<T>::simplify() const {
  if (m_isVal || m_isVar)
    return *this;

  // Unary operations simplification is currently unsupported
  if (!m_leftExpr || !m_rightExpr)
    return Expression<T>(m_leftExpr, m_operation, m_rightExpr);

  Expression<T> leftSimplified{m_leftExpr->simplify()},
      rightSimplified{m_rightExpr->simplify()};

  // Both operands are constants
  if (leftSimplified.m_isVal && rightSimplified.m_isVal) {
    T leftVal{leftSimplified.m_val}, rightVal{rightSimplified.m_val};

    switch (m_operation) {
    case '+':
      return Expression<T>(leftVal + rightVal);
    case '-':
      return Expression<T>(leftVal - rightVal);
    case '*':
      return Expression<T>(leftVal * rightVal);
    case '/':
      if (rightVal != 0)
        return Expression<T>(leftVal / rightVal);
      else
        throw std::runtime_error("Dividing by zero\n");
    case '^':
      return Expression<T>(std::pow(leftVal, rightVal));
    default:
      break;
    }
  }

  // Operands are the same variable
  if (leftSimplified.m_isVar && rightSimplified.m_isVar &&
      rightSimplified.m_var == leftSimplified.m_var) {
    switch (m_operation) {
    case '+':
      return leftSimplified * Expression<T>(2);
    case '-':
      return Expression<T>(0);
    case '*':
      return leftSimplified ^ Expression<T>(2);
    case '/':
      return Expression<T>(1);
    }
  }

  // One of the operands is a constant, and the other is a variable
  switch (m_operation) {

  case '+':
    if (leftSimplified.m_isVal && leftSimplified.m_val == 0)
      return rightSimplified;
    if (rightSimplified.m_isVal && rightSimplified.m_val == 0)
      return leftSimplified;

  case '-':
    if (leftSimplified.m_isVal && leftSimplified.m_val == 0)
      return rightSimplified;
    if (rightSimplified.m_isVal && rightSimplified.m_val == 0)
      return leftSimplified;

  case '*':
    if (leftSimplified.m_isVal) {
      if (leftSimplified.m_val == 1)
        return rightSimplified;
      if (leftSimplified.m_val == 0)
        return Expression<T>(0);
    }
    if (rightSimplified.m_isVal) {
      if (rightSimplified.m_val == 1)
        return leftSimplified;
      if (rightSimplified.m_val == 0)
        return Expression<T>(0);
    }
    break;

  case '/':
    if (leftSimplified.m_isVal && leftSimplified.m_val == 0)
      return Expression<T>(0);
    if (rightSimplified.m_isVal && rightSimplified.m_val == 1)
      return leftSimplified;
    break;

  case '^':
    if (rightSimplified.m_isVal) {
      if (rightSimplified.m_val == 0)
        return Expression<T>(1);
      if (rightSimplified.m_val == 1)
        return leftSimplified;
    }
    if (leftSimplified.m_isVal) {
      if (leftSimplified.m_val == 0)
        return Expression<T>(0);
      if (leftSimplified.m_val == 1)
        return Expression<T>(1);
    }
  }

  return Expression<T>(std::make_shared<Expression<T>>(leftSimplified),
                       m_operation,
                       std::make_shared<Expression<T>>(rightSimplified));
}

int precedence(char op) {
  switch (op) {
  case '+':
  case '-':
    return 1;
  case '*':
  case '/':
    return 2;
  case '^':
    return 3;
  default:
    return -1;
  }
}

/*
template <typename T>
Expression<T> exprFromString(const std::string &exprString) {
  std::stack<std::shared_ptr<Expression<T>>> operands;
  std::stack<char> operators;

  std::istringstream stream(exprString);
  char c;

  while (stream >> c) {
    if (isspace(c))
      continue;

    // Constant operand
    if (isdigit(c) || c == '.') {
      std::string num;
      num += c;

      while (stream.get(c) && (isdigit(c) || c == '.')) {
        num += c;
      }

      stream.putback(c);
      operands.push(std::make_shared<Expression<T>>(std::stod(num)));
    }

    // Variable operand
    else if (isalpha(c)) {
      operands.push(std::make_shared<Expression<T>>(std::string(1, c)));
    }

    // Operators
    else if (c == '+' || c == '-' || c == '*' || c == '/') {
      while (!operators.empty() &&
             precedence(c) <= precedence(operators.top())) {
        char op = operators.top();
        operators.pop();
        auto right = operands.top();
        operands.pop();
        auto left = operands.top();
        operands.pop();
        operands.push(std::make_shared<Expression<T>>(left, op, right));
      }
      operators.push(c);
    }

    // Parentheses
    else if (c == '(')
      operators.push(c);

    else if (c == ')') {
      while (operators.top() != '(') {
        char op = operators.top();
        operators.pop();
        auto right = operands.top();
        operands.pop();
        auto left = operands.top();
        operands.pop();
        operands.push(std::make_shared<Expression<T>>(left, op, right));
      }
      operators.pop(); // Discard '('
    }
  }

  // Process any remaining operators
  while (!operators.empty()) {
    char op = operators.top();
    operators.pop();
    auto right = operands.top();
    operands.pop();
    auto left = operands.top();
    operands.pop();
    operands.push(std::make_shared<Expression<T>>(left, op, right));
  }

  // The last remaining operand is the final result
  return *operands.top();
} */
