# Makefile

CFLAGS = -Wall -g

all: server subscriber

# Compileaza server.cpp
server: server.cpp
	g++ -std=c++11 server.cpp -o server

# Compileaza subscriber.cpp
subscriber: subscriber.cpp
	g++ -std=c++11 subscriber.cpp -o subscriber

clean:
	rm -f server subscriber
