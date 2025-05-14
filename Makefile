CC = clang
GCC_FLAGS = -Wall
LDFLAGS = 
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = bin
INSTALL_DIR = /usr/local/bin
EXE = $(BUILD_DIR)/jache

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(subst $(SRC_DIR), $(BUILD_DIR), $(SRCS:.c=.o))

all: GCC_FLAGS += -O3
all: $(OBJS) $(EXE)

debug: GCC_FLAGS += -DDEBUG -g -O1
debug: $(EXE)

$(EXE): $(OBJS) | $(BUILD_DIR)
	$(CC) $(GCC_FLAGS) -o $(EXE) -I $(INCLUDE_DIR) $(LDFLAGS) $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(GCC_FLAGS) -c -I $(INCLUDE_DIR) -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*

install:
	cp $(EXE) $(INSTALL_DIR)/
