# Dog Race Game Makefile

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Platform detection
ifeq ($(OS),Windows_NT)
    PLATFORM = Windows
    TARGET_EXT = .exe
    RM = del /Q
    RUN_PREFIX = 
else
    PLATFORM = Unix/Linux
    TARGET_EXT = 
    RM = rm -f
    RUN_PREFIX = ./
endif

# Target executable
TARGET = dograce

# Source files
SRCS = main.cpp game.cpp dog.cpp track.cpp

# Header files
HEADERS = game.h dog.h track.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)
	@echo "Compiled for $(PLATFORM) platform"

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean intermediate files and executable
clean:
	$(RM) $(OBJS) $(TARGET)$(TARGET_EXT)

# Run the game (cross-platform)
run: $(TARGET)
ifeq ($(OS),Windows_NT)
	$(TARGET)$(TARGET_EXT)
else
	# 在Unix/Linux环境中，使用单引号确保只执行一次程序
	bash -c '$(RUN_PREFIX)$(TARGET)$(TARGET_EXT)'
endif

# Run the game (Windows specific)
run-win: $(TARGET)
	$(TARGET)$(TARGET_EXT)

# Run the game (Unix/Linux specific)
run-unix: $(TARGET)
	# 使用bash -c来确保只执行一次程序
	bash -c '$(RUN_PREFIX)$(TARGET)$(TARGET_EXT)'

# Phony targets declaration
.PHONY: all clean run run-win run-unix 