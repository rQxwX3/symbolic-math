# symbolic-math

C++ library designed for symbolic mathematical computations. Primary use of the library is calculating derivatives of symbolic expressions.

## Features

- **Representation**: Expressions are passed to program in a raw user-friendly form
- **Differentiation**: Program computes symbolic derivatives of expressions
- **Simplification**: Upon calculation, derivatives are simplified (to a certain extent)


## Installation

Clone the repository & build the project:

```bash
git clone https://github.com/rQxwX3/symbolic-math.git
cd symbolic-math 
make
```

## Usage

```bash
./differentiator --diff "ln(x) / cos(x)" --by x
./differentiator --diff "y ^ y" --by y
```
