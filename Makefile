CC := clang
CFLAGS := -std=c99 -Wall -Wextra
LIBS := -lraylib -lm
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=bin/%.o)
TARGET = visualize

bin/$(TARGET): $(OBJS) | bin
	$(CC) $^ -o $@ $(LIBS)

bin/%.o: src/%.c | bin
	$(CC) $(CFLAGS) -c $< -o $@

bin:
	mkdir -p bin

clean:
	rm -rf ./bin/*
