#Makefile for FFT Test
# Kolijn Wolfaardt, kolijn.wolfaardt@gmail.com
# June 2013

CC=gcc
CFLAGS=-o Main.out
SOURCES=src/main.c src/fft.c src/fft.h -lm

all: $(SOURCES)
	@echo "Compiling"
	@$(CC) $(CFLAGS) $(SOURCES)
	@echo "Done"

clean:
	@echo "Cleaning build directory"
	@rm -rf *.out
