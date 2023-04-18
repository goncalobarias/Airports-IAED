CC=gcc
CFLAGS=-Wall -Wextra -Werror -ansi -pedantic
TARGET=proj

SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard src/*.h)

.PHONY: all fmt clean

all:
	$(CC) $(CFLAGS) src/*.c -o $(TARGET)
	./proj

fmt: $(SOURCES) $(HEADERS)
	clang-format -i $^

clean:
	rm $(TARGET)
