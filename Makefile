CC = gcc
CFLAGS = -Wall


all: main

run: all
	./main 

main: main.c
	$(CC) -o $@ $^

.PHONY: run clean setup

setup:
	mkdir -p ./data/Users_Todo/  
	touch ./data/.userdata


clean:
	rm -rf ./main
