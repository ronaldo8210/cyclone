CC=g++
CFLAGS=-g -Wall -std=c++11 -I/usr/include/boost -I./include

SRCS=event_loop.cpp main.cpp

LIBS=-L/usr/lib -lboost_thread -lpthread

OBJS=$(SRCS:.cpp=.o)

EXECUTABLE=my_server

all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRCS) $(LIBS)

clean:
	rm -rf $(OBJS) my_server
