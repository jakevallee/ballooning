#Makefile

TARGET = dataCollect
CC = gcc
CFLAGS = -g -Wall
OBJS = dataCollect.o 
LFLAGS = -lm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
