CC = g++
CFLAGS = -Wall -Wextra -g
LDFLAGS = -mwindows -lcomctl32

SRCS = WinMain.cpp \
       Errors/Errors.cpp

HEADERS = TransportMap/TransportMap.hpp \
          Graph/Graph.hpp \
          Errors/Errors.hpp

OBJS = $(SRCS:.cpp=.o)

TARGET = lab3

.PHONY: all clean cleanall rebuild run rerun

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

cleanall:
	rm -f $(OBJS) $(TARGET)

rebuild: cleanall all

run:
	./$(TARGET)

rerun: rebuild run