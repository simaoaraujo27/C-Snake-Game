CC = gcc
CFLAGS = -Wall -I.
LDFLAGS = -lSDL2 -lSDL2_ttf
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
EXEC = SnakeGame

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(EXEC)

clean:
	rm -f src/*.o $(EXEC)

.PHONY: all run clean
