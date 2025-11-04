
# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++17 -lgdi32 -mwindows

# Source files
SRCS = GomiSaver.c

# Executable name
TARGET = GomiSaver.exe

# Define the output directory for the Release build
BUILD_DIR = Release

# Generate object file names with the build directory prefix
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

.PHONY: all clean

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
#	@del $(BUILD_DIR)\*.o
#	@rm $(BUILD_DIR)/*.o

$(BUILD_DIR)/%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)