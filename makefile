# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11

# Directories
INCLUDE_DIR = include
LIB_DIR = lib

# External Libraries
LIBS := -lstdc++ -lopengl32 -lglew32 -lglfw3

# Source files
SRCS := $(wildcard *.cpp)

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable name
EXEC := raytracer

# Default target
all: $(EXEC)

# Linking
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LIBS)

# Compilation
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	del -f $(OBJS)