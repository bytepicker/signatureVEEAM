CC=g++
CFLAGS= -c -std=c++11 -Wall
LDFLAGS = -lboost_system -lboost_thread-mt -lboost_filesystem
SOURCES=main.cpp Header.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=signatureVEEAM

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
