CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -Ofast

# List of object files
OBJS_MAIN = main.o Player.o

# Output executables
MAIN_EXEC = main

# Default target (build both executables)
all: $(MAIN_EXEC)

# Rule for main executable
$(MAIN_EXEC): $(OBJS_MAIN)
	$(CXX) $(OBJS_MAIN) -o $(MAIN_EXEC)

# Rule for compiling main.o
main.o: main.cpp Simulator.h Player.h Blackjack.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Clean up build artifac
clean:
	rm -f $(OBJS_MAIN) $(MAIN_EXEC)