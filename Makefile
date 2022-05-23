TARGET = int
CLIBS = -lpthread -lm
CFLAGS = -Wall -MMD
CC = gcc
HEADERS = pthread_cpu.h
OBJECTS = pthread_cpu.o main.o

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(CLIBS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $< $(CLIBS) $(CFLAGS) -c -o $@

clean:
	rm -f $(TARGET) *.d *.o

-include *.d