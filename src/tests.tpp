#ifndef TESTS_TPP
#define TESTS_TPP

#include "../include/expression.hpp"
#include "../include/tests.hpp"

template <typename T>
void tests::printResult(bool condition, const std::string &testName) {
  if (condition) std::cout << "[OK] " << testName << "\n";
  else std::cout << "[FAIL] " << testName << "\n";
}

template <typename T>
void tests::constructors() {
  Expression<T> expr2(3.14);
  printResult<T>(expr2.toString() == "3.14", "Constructor with value 3.14");

  Expression<T> expr3("x");
  printResult<T>(expr3.toString() == "x", "Constructor with variable x");

  Expression<T> expr4 {Expression<T>("x") + Expression<T>(2)};
  printResult<T>(
    expr4.toString() == "x + 2", "Constructor with expression x + 2"
  );
}

template <typename T>
void tests::operators() {
  Expression<T> expr1(3), expr2(5);

  Expression<T> result {expr1 + expr2};
  printResult<T>(result.toString() == "3 + 5", "Addition operator");

  result = expr1 - expr2;
  printResult<T>(result.toString() == "3 - 5", "Subtraction operator");

  result = expr1 * expr2;
  printResult<T>(result.toString() == "3*5", "Multiplication operator");

  result = expr1 / expr2;
  printResult<T>(result.toString() == "3/5", "Division operator");

  result = expr1 ^ expr2;
  printResult<T>(result.toString() == "3^5", "Exponentiation operator");

  result = -expr1;
  printResult<T>(result.toString() == "-3", "Unary minus operator");

  printResult<T>(expr1 == Expression<T>(3), "Equality operator (equal)");
  printResult<T>(!(expr1 == expr2), "Equality operator (not equal)");
}

template <typename T>
void tests::mathFunctions() {
  Expression<T> expr1 {Expression<T>::sin(Expression<T>("x"))};
  printResult<T>(expr1.toString() == "sin(x)", "sin(x)");

  Expression<T> expr2 {Expression<T>::cos(Expression<T>("x"))};
  printResult<T>(expr2.toString() == "cos(x)", "cos(x)");

  Expression<T> expr3 {Expression<T>::ln(Expression<T>("x"))};
  printResult<T>(expr3.toString() == "ln(x)", "ln(x)");

  Expression<T> expr4 {Expression<T>::exp(Expression<T>("x"))};
  printResult<T>(expr4.toString() == "exp(x)", "exp(x)");
}

template <typename T>
void tests::substitution() {
  Expression<T> expr1 {Expression<T>("x") + Expression<T>(2)};
  Expression<T> result {expr1.substitute("x", 5)};
  printResult<T>(result.toString() == "5 + 2", "Substitution of x with 5");
}

template <typename T>
void tests::evaluation() {
  Expression<T> expr1 {Expression<T>("x") + Expression<T>(2)};
  T result {expr1.evaluate({{"x", 3}})};
  printResult<T>(result == 5, "Evaluation with x=3");
}

template <typename T>
void tests::derivative() {
  Expression<T> expr1 {Expression<T>(3) * Expression<T>("x") + 
                       Expression<T>(2)};
  Expression<T> result {expr1.derivative("x").simplify()};
  printResult<T>(result.toString() == "3", "Derivative of 3x + 2");
}

template <typename T>
void tests::toString() {
  Expression<T> expr1 {Expression<T>("x") + Expression<T>(2)};
  printResult<T>(expr1.toString() == "x + 2", "toString for x + 2");

  Expression<T> expr2 {Expression<T>("x") * Expression<T>(5)};
  printResult<T>(expr2.toString() == "x*5", "toString for x * 5");
}

template <typename T>
void tests::all() {
  constructors<T>();
  operators<T>();
  mathFunctions<T>();
  substitution<T>();
  evaluation<T>();
  toString<T>();
  derivative<T>();

  std::cout << "All tests finished!\n";
}


#endif // TESTS_TPP
