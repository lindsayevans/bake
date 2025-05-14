CC = clang++
GCC_FLAGS = -Wall -std=c++2b
LDFLAGS = -ljansson
SRC_DIR = src
INCLUDE_DIR = lib
BUILD_DIR = bin
INSTALL_DIR = /usr/local/bin
EXE = $(BUILD_DIR)/bake

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(subst $(SRC_DIR), $(BUILD_DIR), $(SRCS:.cpp=.o))

all: GCC_FLAGS += -O3
all: $(OBJS) $(EXE)

debug: GCC_FLAGS += -DDEBUG -g -O1
debug: $(EXE)

$(EXE): $(OBJS) | $(BUILD_DIR)
	$(CC) $(GCC_FLAGS) -o $(EXE) -I $(INCLUDE_DIR) $(LDFLAGS) $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(GCC_FLAGS) -c -I $(INCLUDE_DIR) -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/util:
	mkdir -p $(BUILD_DIR)/util

clean:
	rm -rf $(BUILD_DIR)/*
	mkdir -p $(BUILD_DIR)/util

install:
	cp $(EXE) $(INSTALL_DIR)/
