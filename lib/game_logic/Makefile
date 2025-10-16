# ====== Makefile for game_logic ======
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude

SRC_DIR = src
OBJ_DIR = obj
BIN = game_logic_test

SRCS = $(SRC_DIR)/drawing.c
OBJS = $(OBJ_DIR)/drawing.o

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: clean
