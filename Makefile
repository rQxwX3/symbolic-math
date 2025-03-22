CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

SRC_DIR = src
INC_DIR = include

SRC = $(SRC_DIR)/main.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = differentiator

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

test:
	./differentiator --diff "ln(x) / cos(x)" --by x
	./differentiator --diff "x ^ x" --by x
	./differentiator --diff "x ^ y" --by y
	./differentiator --diff "x / 2" --by x
	./differentiator --eval "x / y" x=2 y=0
	./differentiator --eval "ln(x)" x=-1
	./differentiator --eval "ln(x)" x=0
	./differentiator --eval "ln(x)" x=3
	./differentiator --eval "x * y" x=10 y=12
	./differentiator --diff "x / sin(x)" --by x
	./differentiator --diff "x ^ 5" --by x
	./differentiator --diff "x * x" --by x
	./differentiator --diff "x * y" --by x
	./differentiator --diff "exp(x) * x ^ 3" --by x
	./differentiator --diff "ln(x ^ 3)" --by x
