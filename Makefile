# Compiler
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -Werror=reorder -Werror=unused-function -Werror

# Expression builder path
LIBDIR = lib/x86_64-linux-gnu
LIBS = -L$(LIBDIR) -lexpression_parser -static

# Include path
INCLUDES = -I./src

# Output
TARGET = spreadsheet.out

# Sources
SRCS = src/CPosition.cpp src/CCell.cpp src/CExpression.cpp src/CSpreadsheet.cpp src/CTest.cpp src/main.cpp
HEADERS = src/CPosition.h src/CCell.h src/CExpression.h src/expression.h src/CSpreadsheet.h src/CTest.h src/includes.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Clang-Tidy
CLANG_TIDY_CONFIG_FILE = .clang-tidy
CLANG_TIDY = clang-tidy --config-file=$(CLANG_TIDY_CONFIG_FILE)

# Build rules
all: analyze tidy $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)
	rm -f $(OBJS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Run static analysis with scan-build
analyze:
	scan-build $(MAKE) tidy_only

# Run clang-tidy checks before building
tidy:
	$(CLANG_TIDY) $(SRCS) -- -std=c++20

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# A target to run only clang-tidy without building
tidy_only:
	$(MAKE) tidy
