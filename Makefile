# CC = gcc
# CFLAGS = -Wall -Iinclude
# SRC = $(wildcard src/*.c)
# OBJ = $(SRC:src/%.c=build/%.o)
# DEPS = $(OBJ:%.o=%.d)

# -include $(DEPS)

# build/%.o: src/%.c
# 	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

# main: $(OBJ)
# 	$(CC) -o $@ $^ $(CFLAGS)

# .PHONY: clean run
# run: main
# 	./main
# clean:
# 	rm -f main $(OBJ) $(DEPS)

CC = gcc
CFLAGS = -Wall -Iinclude
SRCDIR = src
BUILDDIR = build

# Find all .c files in SRCDIR and its subdirectories
SRC = $(shell find $(SRCDIR) -name '*.c')
# Generate .o file paths in BUILDDIR with the same structure as SRCDIR
OBJ = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# List all header dependencies
DEPS = $(shell find include -name '*.h')


all: main

main: $(OBJ)
	$(CC) -o $@ $^

# Rule to compile .c files to .o files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run clean initDep
initDep:
	touch ./data/.userdata
run: initDep all
	./main 

clean:
	rm -rf $(BUILDDIR)/*.o ./main
