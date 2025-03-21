#ifndef TESTS_HPP
#define TESTS_HPP

#include <string>

namespace tests {
template <typename T> void printResult(bool, const std::string&);

template <typename T> void constructors();

template <typename T> void operators();

template <typename T> void mathFunctions();

template <typename T> void substitution();

template <typename T> void evaluation();

template <typename T> void derivative();

template <typename T> void toString();

template <typename T> void all();
} // namespace tests

#include "../src/tests.tpp"

#endif // TESTS_HPP
