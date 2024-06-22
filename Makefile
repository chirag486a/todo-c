CC = gcc
CFLAGS = -Wall -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
DEPS = $(OBJ:%.o=%.d)

-include $(DEPS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f main $(OBJ) $(DEPS)