#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <map>
#include <memory>
#include <string>

template <typename T> class Expression {
public:
  // Constructors
  Expression() = default;
  Expression(T val);
  Expression(const std::string &var);
  Expression(std::shared_ptr<Expression<T>> leftExpr, char op,
             std::shared_ptr<Expression<T>> rightExpr);
  Expression(std::shared_ptr<Expression<T>> leftExpr, char op);

  // Operators
  Expression<T> operator+(const Expression<T> &other) const;
  Expression<T> operator-(const Expression<T> &other) const;
  Expression<T> operator*(const Expression<T> &other) const;
  Expression<T> operator/(const Expression<T> &other) const;
  Expression<T> operator^(const Expression<T> &other) const;
  Expression<T> operator-() const;

  // Functions
  static Expression<T> sin(const Expression<T> &expr);
  static Expression<T> cos(const Expression<T> &expr);
  static Expression<T> ln(const Expression<T> &expr);
  static Expression<T> exp(const Expression<T> &expr);

  Expression<T> substitute(const std::string &var, T val) const;
  T evaluate(const std::map<std::string, T> &context) const;
  Expression<T> derivative(const std::string &var) const;
  Expression<T> simplify() const;

  std::string toString() const;

private:
  T m_val;
  std::string m_var;
  bool m_isVal;
  bool m_isVar;
  std::shared_ptr<Expression<T>> m_leftExpr;
  std::shared_ptr<Expression<T>> m_rightExpr;
  char m_operation;
};

// template <typename T>
// Expression<T> exprFromString(const std::string &exprString);

#include "expression.tpp"

#endif
