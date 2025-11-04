
# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++17 -lgdi32 -mwindows

# Source files
SRCS = GomiSaver.c

# Object files (generated from source files)
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = GomiSaver.exe

# Default target: build the executable
all: $(TARGET)

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Rule to compile each source file into an object file
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up rule: remove object files and the executable
clean:
	del $(OBJS) $(TARGET)