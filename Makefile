# Makefile for High-Performance Multithreaded University ERP System
# Authenticated System with Admin and Student Portals

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 -pthread
DEBUGFLAGS = -g -DDEBUG

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = .
DATA_DIR = data

# Source files
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Student.cpp \
          $(SRC_DIR)/Course.cpp \
          $(SRC_DIR)/Grade.cpp \
          $(SRC_DIR)/Branch.cpp \
          $(SRC_DIR)/EnrollmentRecord.cpp \
          $(SRC_DIR)/ERPSystem.cpp \
          $(SRC_DIR)/SortingManager.cpp \
          $(SRC_DIR)/CSVHandler.cpp \
          $(SRC_DIR)/Auth.cpp \
          $(SRC_DIR)/Database.cpp \
          $(SRC_DIR)/CourseCatalog.cpp \
          $(SRC_DIR)/SemesterManager.cpp \
          $(SRC_DIR)/AdminPortal.cpp \
          $(SRC_DIR)/StudentPortal.cpp \
          $(SRC_DIR)/Colors.cpp \
          $(SRC_DIR)/OutputFormatter.cpp

# Generator utility (creates demo_students.csv with specified number of records)
GENERATOR_SRC = $(SRC_DIR)/generate_demo.cpp

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Header files
HEADERS = $(wildcard $(SRC_DIR)/*.h)

# Target executable
TARGET = $(BIN_DIR)/erp_system

# Default target
all: $(TARGET)

# Create necessary directories
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(DATA_DIR):
	@mkdir -p $(DATA_DIR)
	@mkdir -p $(DATA_DIR)/syllabus

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	@echo "\033[36m[*] Compiling\033[0m $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Link executable
$(TARGET): $(OBJECTS) | $(DATA_DIR)
	@echo "\033[35m[LINK] Linking University ERP system...\033[0m"
	@$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@
	@echo "\033[1;32m[OK] University ERP system built successfully:\033[0m $(TARGET)"

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean all

# Run the system
run: $(TARGET)
	@echo "\033[1;36m▶ Running University ERP system...\033[0m"
	@./$(TARGET)

# Build and run the demo CSV generator
.PHONY: generate-demo
generate-demo:
	@echo "\033[33m[*] Generating demo_students.csv with 3000 records using Python script...\033[0m"
	@python3 scripts/generate_demo.py
	@echo "\033[32m[OK] Generator finished. demo_students.csv updated.\033[0m"

# Clean build artifacts
clean:
	@echo "\033[33m[CLEAN] Cleaning build artifacts...\033[0m"
	@rm -rf $(BUILD_DIR)
	@rm -f $(TARGET)
	@echo "\033[32m[OK] Clean complete\033[0m"

# Clean everything including data
clean-all: clean
	@echo "\033[31m🗑️  Cleaning data files...\033[0m"
	@rm -rf $(DATA_DIR)
# 	@rm -f *.csv *.txt *.db transcript_*.txt
	@echo "\033[32m[OK] Complete clean done\033[0m"

# Help target
help:
	@echo "University ERP System - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all           - Build University ERP system (default)"
	@echo "  run           - Build and run the system"
	@echo "  debug         - Build with debug symbols"
	@echo "  clean         - Remove build artifacts"
	@echo "  clean-all     - Remove build artifacts and data files"
	@echo "  help          - Show this help message"
	@echo ""
	@echo "Executable:"
	@echo "  $(TARGET) - University ERP system with authentication"

# Phony targets
.PHONY: all run debug clean clean-all help

# Dependencies
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(HEADERS)
$(BUILD_DIR)/main_auth.o: $(SRC_DIR)/main_auth.cpp $(HEADERS)
$(BUILD_DIR)/Student.o: $(SRC_DIR)/Student.cpp $(SRC_DIR)/Student.h
$(BUILD_DIR)/Course.o: $(SRC_DIR)/Course.cpp $(SRC_DIR)/Course.h
$(BUILD_DIR)/Grade.o: $(SRC_DIR)/Grade.cpp $(SRC_DIR)/Grade.h
$(BUILD_DIR)/Branch.o: $(SRC_DIR)/Branch.cpp $(SRC_DIR)/Branch.h
$(BUILD_DIR)/EnrollmentRecord.o: $(SRC_DIR)/EnrollmentRecord.cpp $(SRC_DIR)/EnrollmentRecord.h
$(BUILD_DIR)/ERPSystem.o: $(SRC_DIR)/ERPSystem.cpp $(SRC_DIR)/ERPSystem.h
$(BUILD_DIR)/SortingManager.o: $(SRC_DIR)/SortingManager.cpp $(SRC_DIR)/SortingManager.h
$(BUILD_DIR)/CSVHandler.o: $(SRC_DIR)/CSVHandler.cpp $(SRC_DIR)/CSVHandler.h
$(BUILD_DIR)/MenuSystem.o: $(SRC_DIR)/MenuSystem.cpp $(SRC_DIR)/MenuSystem.h
$(BUILD_DIR)/Auth.o: $(SRC_DIR)/Auth.cpp $(SRC_DIR)/Auth.h
$(BUILD_DIR)/Database.o: $(SRC_DIR)/Database.cpp $(SRC_DIR)/Database.h
$(BUILD_DIR)/CourseCatalog.o: $(SRC_DIR)/CourseCatalog.cpp $(SRC_DIR)/CourseCatalog.h
$(BUILD_DIR)/SemesterManager.o: $(SRC_DIR)/SemesterManager.cpp $(SRC_DIR)/SemesterManager.h
$(BUILD_DIR)/AdminPortal.o: $(SRC_DIR)/AdminPortal.cpp $(SRC_DIR)/AdminPortal.h
$(BUILD_DIR)/StudentPortal.o: $(SRC_DIR)/StudentPortal.cpp $(SRC_DIR)/StudentPortal.h
