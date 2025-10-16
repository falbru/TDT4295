CC = gcc
CFLAGS = -O3 -Wall

UNITY = vendor/Unity/src/unity.c
TEST = test_game_logic.c
LOGIC_SRC = ../lib/game_logic/src/*.c  # include all game logic source files
OUT = test_game_logic

.PHONY: all clean test_run

all: $(OUT)

$(OUT): $(TEST) $(UNITY) $(LOGIC_SRC)
	$(CC) $(CFLAGS) $(TEST) $(UNITY) $(LOGIC_SRC) -o $(OUT)

test_run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
