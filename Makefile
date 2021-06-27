CC=g++
CFLAGS = -c -std=c++17 -Wall -Wextra -Werror -Wconversion -Wpadded -O2
CFLAGS_DEBUG = -c -std=c++17 -Wall -Wextra -Wconversion -Wpadded -Og -g3 -ggdb

HEADERDIR= ./
SOURCEDIR= ./

HEADER_FILES = disassembler.hpp
SOURCE_FILES = main.cpp disassembler.cpp

HEADERS_FP = $(addprefix $(HEADERDIR),$(HEADER_FILES))
SOURCE_FP = $(addprefix $(SOURCEDIR),$(SOURCE_FILES))

OBJECTS = $(SOURCE_FP:.c=.o)

EXECUTABLE = chip8

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)


%.o: %.c $(HEADERS_FP)
	$(CC) $(CFLAGS_DEBUG) -o $@ $<

clean:
	rm -rf ./*.o $(EXECUTABLE)