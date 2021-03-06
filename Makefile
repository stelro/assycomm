#Makefile 
#This file is part of Assy Comm !
#Stelmach Ro (xtasy) <stelmach.ro@gmail.com>

TARGET = assycomm
LIBS = -lmysqlcppconn
CC = g++
CFLAGS = -g -Wall -lmysqlcppconn

.PHONY: default all clean

default: $(TARGET)
all:default

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
