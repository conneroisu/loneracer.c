CC := gcc
CFLAGS := -Wall -Wextra
LDFLAGS := -lncurses

TARGET := loneracer2
SRC := loneracer2.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
