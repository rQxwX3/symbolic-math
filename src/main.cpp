#include "../include/tests.hpp"
#include "../include/expression.hpp"
#include <iostream>
#include <map>
#include <string>

template <typename T>
T evaluateExpr(const std::string& expr_str, const std::map<std::string, T>& context) {
  try {
    Expression<T> expr = Expression<T>::fromString(expr_str);

    return expr.evaluate(context);
  } catch (const std::exception& e) {
    std::cerr << "Error evaluating expression: " << e.what() << "\n";
    return T(0);
  }
}

template <typename T>
Expression<T> differentiateExpr(const std::string& expr_str, const std::string& var) {
  try {
    Expression<T> expr = Expression<T>::fromString(expr_str);


    return expr.derivative(var);
  } catch (const std::exception& e) {
    std::cerr << "Error differentiating expression: " << e.what() << "\n";
    return Expression<T>(0);
  }
}

int main(int argc, char* argv[]) {
  // tests::all<double>();

  if (argc < 2) {
    std::cerr << "Usage: differentiator [--eval 'expression' var1=value1 var2=value2 ...] or [--diff 'expression' --by var]\n";
    return 1;
  }

  std::string command = argv[1];

  if (command == "--eval") {
    if (argc < 4) {
      std::cerr << "Error: Missing expression or variable assignments\n";
      return 1;
    }

    std::string expr_str = argv[2];
    std::map<std::string, double> context;

    for (int i = 3; i < argc; ++i) {
      std::string var_assignment = argv[i];
      size_t equal_pos = var_assignment.find('=');
      if (equal_pos == std::string::npos) {
        std::cerr << "Error: Invalid variable assignment format\n";
        return 1;
      }

      std::string var_name = var_assignment.substr(0, equal_pos);
      double var_value = std::stod(var_assignment.substr(equal_pos + 1));
      context[var_name] = var_value;
    }

    double result = evaluateExpr(expr_str, context);
    std::cout << result << "\n";
  }

  else if (command == "--diff") {
    if (argc < 4) {
        std::cerr << "Error: Missing expression or differentiation variable\n";
        return 1;
    }

    std::string expr_str = argv[2];
    std::string var = argv[4];

    auto derivative = differentiateExpr<double>(expr_str, var);
    std::cout << derivative.toString() << "\n";
  }

  else {
    std::cerr << "Error: Unknown command\n";
    return 1;
  }

  return 0;
}
