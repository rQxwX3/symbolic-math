#include "expression.hpp"
#include <iostream>

int main() {
  Expression<int> x{"x"};
  Expression<int> four{4};

  Expression expr{(x ^ four) / four};

  std::cout << "Original expression: " << expr.toString() << "\n";

  auto derivativeExpr = expr.derivative("x");

  std::cout << "Derivative: " << derivativeExpr.toString() << "\n";
  std::cout << "Simplified: " << derivativeExpr.simplify().toString() << "\n";

  return 0;
}
