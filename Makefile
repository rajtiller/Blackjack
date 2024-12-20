CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -Ofast

# List of object files
OBJS_MAIN = main.o Player.o Simulator.o Blackjack.o

# Output executable
MAIN_EXEC = main

# Default target (build both executables)
all: $(MAIN_EXEC)

# Rule for main executable
$(MAIN_EXEC): $(OBJS_MAIN)
	$(CXX) $(OBJS_MAIN) -o $(MAIN_EXEC)

# Rule for compiling main.o
main.o: main.cpp Simulator.h Player.h Blackjack.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Rule for compiling Player.o
Player.o: Player.cpp Player.h
	$(CXX) $(CXXFLAGS) -c Player.cpp -o Player.o

# Rule for compiling Simulator.o
Simulator.o: Simulator.cpp Simulator.h Player.h Blackjack.h
	$(CXX) $(CXXFLAGS) -c Simulator.cpp -o Simulator.o

# Rule for compiling Blackjack.o
Blackjack.o: Blackjack.cpp Blackjack.h
	$(CXX) $(CXXFLAGS) -c Blackjack.cpp -o Blackjack.o

# Clean up build artifacts
clean:
	rm -f $(OBJS_MAIN) $(MAIN_EXEC)

# Declare non-file targets
.PHONY: all clean
