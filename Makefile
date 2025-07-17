CC = gcc
CFLAGS = -Wall -Wextra -std=c11   # compiler flags, no linker flags here
LDFLAGS = -lws2_32                # linker flags

OBJ = src/main.o src/socket_server.o src/client_handler.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)