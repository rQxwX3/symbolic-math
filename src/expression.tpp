#ifndef EXPRESSION_TPP
#define EXPRESSION_TPP

#include "../include/parser.hpp"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <vector>

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
  if (m_isVal) return Expression<T>(-m_val);

  if (m_isVar)
    return Expression(
      std::make_shared<Expression<T>>(*this), '*',
      std::make_shared<Expression<T>>(T(-1))
    );

  return Expression(
    std::make_shared<Expression<T>>(*this), '*',
    std::make_shared<Expression<T>>(T(-1))
  );
}

template <typename T>
bool Expression<T>::operator==(const Expression<T> &other) const {
  if (this->m_isVal && other.m_isVal)
    return this->m_val == other.m_val;

  if (this->m_isVar && other.m_isVar)
    return this->m_var == other.m_var;

  if ((this->m_isVal && other.m_isVar) || (this->m_isVar && other.m_isVal)) {
    return false;
  }

  if (!this->m_isVal && !this->m_isVar) {
    if (this->m_operation != other.m_operation)
      return false;

    return (*this->m_leftExpr == *other.m_leftExpr) &&
           (*this->m_rightExpr == *other.m_rightExpr);
  }

  return false;
}

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

  if (m_isVar)
    return (m_var == var) ? Expression<T>(val) : *this;

  auto leftSubstitute{m_leftExpr->substitute(var, val)},
      rightSubstitute{m_rightExpr->substitute(var, val)};

  return Expression<T>(std::make_shared<Expression<T>>(leftSubstitute),
                       m_operation,
                       std::make_shared<Expression<T>>(rightSubstitute));
}

template <typename T>
T Expression<T>::evaluate(const std::map<std::string, T> &context) const {
  if (m_isVal) return m_val;

  if (m_isVar) {
    auto it {context.find(m_var)};

    if (it != context.end()) return it->second;

    throw std::runtime_error("Variable not found");
  }

  T leftVal {m_leftExpr->evaluate(context)},
    rightVal {m_rightExpr ? m_rightExpr->evaluate(context) : T(0)};

  switch (m_operation) {
    case '-':
      if (!m_rightExpr) return -leftVal;
      return leftVal - rightVal;

    case '+': return leftVal + rightVal;
    case '*': return leftVal * rightVal;
    case '/':
      if (rightVal != 0) return leftVal / rightVal;
      throw std::runtime_error("Division by zero");

    case '^': return std::pow(leftVal, rightVal);
    case 's': return std::sin(leftVal);
    case 'c': return std::cos(leftVal);
    case 'l':
      if (leftVal > 0) return std::log(leftVal);
      throw std::runtime_error("Invalid argument for ln()");

    case 'e': return std::exp(leftVal);
    default: throw std::runtime_error("Unknown operator");
  }
}

template <typename T> std::string Expression<T>::toString() const {
  if (m_isVal) {
    std::ostringstream ss;

    if (std::floor(m_val) == m_val) ss << static_cast<int>(m_val);
    else ss << m_val;

    return ss.str();
  }

  if (m_isVar) return m_var;

  std::string leftString{m_leftExpr->toString()},
              rightString{m_rightExpr ? m_rightExpr->toString() : ""};

  switch (m_operation) {
    case '+': return leftString + " + " + rightString;
    case '-': return leftString + " - " + rightString;
    case '*': return "(" + leftString + ")" + "*" + "(" + rightString + ")";
    case '/': return "(" + leftString + ")" + "/" + "(" + rightString + ")";
    case '^': return "(" + leftString + ")" + "^" + "(" + rightString + ")";
    case 's': return "sin(" + leftString + ")";
    case 'c': return "cos(" + leftString + ")";
    case 'l': return "ln(" + leftString + ")";
    case 'e': return "exp(" + leftString + ")";
    default: return "";
  }
}

template <typename T>
Expression<T> Expression<T>::derivative(const std::string &var) const {
  if (m_isVal) return Expression<T>(0);

  if (m_isVar) {
    if (m_var == var) return Expression<T>(1);
    return Expression<T>(0);
  }

  if (!m_leftExpr) throw std::runtime_error("Invalid operation");

  bool leftIsVar{m_leftExpr->m_isVar && m_leftExpr->m_var == var},
       leftIsVal{m_leftExpr->m_isVal}, leftIsExpr {m_leftExpr->m_leftExpr};

  Expression<T> leftDerivative{m_leftExpr->derivative(var)};

  switch (m_operation) {
    case '+':
      if (!m_rightExpr) return leftDerivative;
      return leftDerivative + m_rightExpr->derivative(var);

    case '-':
      if (!m_rightExpr) return -leftDerivative;
      return leftDerivative - m_rightExpr->derivative(var);

    case '*':
      if (!m_rightExpr) throw std::runtime_error("Missing operand for *");
      return leftDerivative * *m_rightExpr +
             *m_leftExpr * m_rightExpr->derivative(var);

    case '/':
      if (!m_rightExpr) throw std::runtime_error("Missing operand for /");
      return (leftDerivative * *m_rightExpr - *m_leftExpr *
              m_rightExpr->derivative(var)) / (*m_rightExpr * *m_rightExpr);

    case '^': {
      if (!m_rightExpr) throw std::runtime_error("Missing operand for ^");

      bool rightIsVar {m_rightExpr->m_isVar && m_rightExpr->m_var == var},
           rightIsVal {m_rightExpr->m_isVal},
           rightIsExpr {m_rightExpr->m_leftExpr};

      // const ^ const
      if (leftIsVal && rightIsVal) return Expression<T>(0);

      // x ^ const
      if (leftIsVar && (!rightIsVar && !rightIsExpr)) {
        Expression<T> exponent{*m_rightExpr - Expression<T>(1)};
        return *m_rightExpr * (*m_leftExpr ^ (exponent)) * leftDerivative;
      }

      // const ^ (x / expr)
      if ((!leftIsVar && !leftIsExpr) && (rightIsExpr || rightIsVar)) {
        return *this * m_rightExpr->derivative(var) * ln(*m_leftExpr);
      }

      // (expr/x) ^ (expr/x)
      if ((leftIsExpr || leftIsVar) && (rightIsExpr || rightIsVar))
        return *this * (m_rightExpr->derivative(var) * (ln(*m_leftExpr) +
               *m_rightExpr * leftDerivative / *m_leftExpr));

      throw std::runtime_error("Unknown operands for ^");
    }

    case 's':
      if (m_leftExpr) return cos(*m_leftExpr) * leftDerivative;
      throw std::runtime_error("Missing argument for sin");

    case 'c':
      if (m_leftExpr) return -sin(*m_leftExpr) * leftDerivative;
      throw std::runtime_error("Missing argument for cos");

    case 'l':
      if (m_leftExpr)  return Expression<T>(1) / leftDerivative;
      throw std::runtime_error("Missing argument for ln");

    case 'e':
      if (m_leftExpr) return exp(*m_leftExpr) * leftDerivative;
      throw std::runtime_error("Missing argument for exp");
  }

  throw std::runtime_error(
      "Derivative calculation failed due to unknown operation"
  );
}

template <typename T>
Expression<T> Expression<T>::fromString(const std::string &exprString) {
  std::vector<Token> tokens{parser::tokenize(exprString)};
  return parser::parseExpression<T>(tokens);
}

template <typename T> Expression<T> Expression<T>::simplify() const {
  if (m_isVal || m_isVar) return *this;

  if (!m_leftExpr || !m_rightExpr)
    return Expression<T>(m_leftExpr, m_operation, m_rightExpr);

  Expression<T> leftSimplified{m_leftExpr->simplify()},
      rightSimplified{m_rightExpr->simplify()};

  // Both operands are constants
  if (leftSimplified.m_isVal && rightSimplified.m_isVal) {
    T leftVal {leftSimplified.m_val}, rightVal {rightSimplified.m_val};

    switch (m_operation) {
      case '+': return Expression<T>(leftVal + rightVal);
      case '-': return Expression<T>(leftVal - rightVal);
      case '*': return Expression<T>(leftVal * rightVal);
      case '/':
        if (rightVal == 0) throw std::runtime_error("Dividing by zero\n");
        return Expression<T>(leftVal / rightVal);

      case '^':
        return Expression<T>(std::pow(leftVal, rightVal));

      default: break;
    }
  }

  // Operands are the same variable
  if (leftSimplified.m_isVar && rightSimplified.m_isVar &&
      rightSimplified.m_var == leftSimplified.m_var) {
    switch (m_operation) {
      case '+': return leftSimplified * Expression<T>(2);
      case '-': return Expression<T>(0);
      case '*': return leftSimplified ^ Expression<T>(2);
      case '/': return Expression<T>(1);
    }
  }

  // One of the operands is a constant, and the other is a variable
  switch (m_operation) {
  case '+':
    if (leftSimplified.m_isVal && leftSimplified.m_val == 0)
      return rightSimplified;
    if (rightSimplified.m_isVal && rightSimplified.m_val == 0)
      return leftSimplified;

    break;

  case '-':
    if (leftSimplified.m_isVal && leftSimplified.m_val == 0)
      return -rightSimplified;
    if (rightSimplified.m_isVal && rightSimplified.m_val == 0)
      return leftSimplified;

    break;

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
    if (rightSimplified.m_isVal)
      if (rightSimplified.m_val == 0)
        return Expression<T>(1);
    if (rightSimplified.m_val == 1)
      return leftSimplified;

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

#endif // EXPRESSION_TPP
