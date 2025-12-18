# Makefile para Producer-Consumer

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread
TARGET = prod_cons
SRC = producer_consumer.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean

