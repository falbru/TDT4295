CC = gcc
CFLAGS = -O3 -Wall

UNITY = vendor/Unity/src/unity.c
TEST = test_example.c
OUT = test_example

.PHONY: all clean test_run

all: $(OUT)

$(OUT): $(TEST) $(UNITY)
	$(CC) $(CFLAGS) $(TEST) $(UNITY) -o $(OUT)

test_run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
