CC = g++
CWARNINGS =  -Wall -Wextra
COPTS = -std=c++17
CFLAGS = $(COPTS) $(CWARNINGS) -g
LINKERS = -Iextern/doctest -I.
OBJECTS = minefield.o
STATICFLAGS = -static -static-libgcc -static-libstdc++

all: mines.cpp $(OBJECTS)
	$(CC) -o mines $(CFLAGS) $^ $(LINKERS)

static: mines.cpp $(OBJECTS)
	$(CC) -o mines $(CFLAGS) $^ $(LINKERS) $(STATICFLAGS)

test: test/minefield $(OBJECTS)
	$(CC) $(CFLAGS) $(LINKERS) $(OBJECTS) -o test/minefield test/minefield.cpp

%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $< $(LINKERS)

clean:
	rm -rf $(OBJECTS) mines
