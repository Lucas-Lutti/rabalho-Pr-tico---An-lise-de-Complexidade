CXX ?= g++
CXXFLAGS ?= -std=c++17 -O3 -DNDEBUG -Wall -Wextra -Iinclude
LDFLAGS ?=

BUILD_DIR := build
BIN_DIR := bin
RESULTS_DIR := results

SOURCES := src/experimentos.cpp src/dsu_ingenuo.cpp src/dsu_uniao_por_rank.cpp \
	src/dsu_rank_com_compressao.cpp src/operacoes.cpp
OBJECTS := $(SOURCES:src/%.cpp=$(BUILD_DIR)/%.o)
TARGET := $(BIN_DIR)/benchmark

.PHONY: all clean run_benchmark dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR) $(RESULTS_DIR)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(RESULTS_DIR):
	mkdir -p $(RESULTS_DIR)

run_benchmark: $(TARGET)
	$(TARGET) --all

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
