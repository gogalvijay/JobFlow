CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -I./src


SRC = src/main.cpp src/JobQueue.cpp src/Worker.cpp src/Server.cpp src/ThreadPool.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = job_server

all: $(TARGET)

# Link final executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp into .o
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
