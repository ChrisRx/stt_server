CC=gcc
CFLAGS=-O2 -Wall
INC=-I/usr/local/include/sphinxbase -I/usr/local/include/pocketsphinx -I/usr/local/include/lwan -I/usr/include -L/usr/lib
LIB=-lpocketsphinx -lsphinxbase -llwan -lz -ldl

BUILD_DIR = bin
TARGET = $(BUILD_DIR)/stt_server

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/*.h)

ALL: build

build: default $(TARGET)

default:
	@mkdir -p bin

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INC) $(LIB) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(INC) $(LIB) -o $@

clean:
	-rm -f src/*.o
	-rm -f $(TARGET)
