CC = g++
CWARNINGS =  -Wall -Wextra
COPTS = -std=c++17 -Ofast
CFLAGS = $(COPTS) $(CWARNINGS)
LINKERS = 
OBJECTS = minefield.o
STATICFLAGS = -static -static-libgcc -static-libstdc++

all: mines.cpp $(OBJECTS)
	$(CC) -o mines $(CFLAGS) $^ $(LINKERS)

static: mines.cpp $(OBJECTS)
	$(CC) -o mines $(CFLAGS) $^ $(LINKERS) $(STATICFLAGS)

%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $< $(LINKERS)

clean:
	rm -rf $(OBJECTS) mines
