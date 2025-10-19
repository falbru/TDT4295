# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -g -I../lib/game_logic/include

# Source files
SRC := ../lib/game_logic/src/drawing.c ../lib/game_logic/src/game_query.c ./vendor/Unity/src/unity.c test_game_logic.c
OUT := test_game_logic

# Default target
.PHONY: build run clean test

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	./$(OUT)

clean:
	rm -f $(OUT)

test: clean build run
