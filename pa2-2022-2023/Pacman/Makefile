TARGET = agaevnat
SOURCES = $(shell find ./src -type f -name *.cpp)
HEADERS = $(shell find ./src -type f -name *.h)
OBJECTS = $(.:.cpp=.o)
DEPENDS = Makefile.depend
INCLUDES = -I./src
CPPFLAGS = -std=c++17 -O2 -Wall $(INCLUDES)

all: $(TARGET) 

$(TARGET): 
	g++ -c -std=c++17 ./src/data.cpp;
	g++ -c -std=c++17 ./src/entity.cpp;
	g++ -c -std=c++17 ./src/exit.cpp;
	g++ -c -std=c++17 ./src/game.cpp;
	g++ -c -std=c++17 ./src/main.cpp;
	g++ -c -std=c++17 ./src/manual.cpp;
	g++ -c -std=c++17 ./src/menu.cpp;
	g++ -c -std=c++17 ./src/namespacer.cpp;
	g++ -c -std=c++17 ./src/options.cpp;

run: 
	./compile;

.PHONY: clean

compile: 
	g++ namespacer.o data.o options.o exit.o manual.o game.o menu.o entity.o main.o -lncurses -o $@;

clean: 
	rm -f data.o;
	rm -f entity.o;
	rm -f exit.o;
	rm -f game.o;
	rm -f main.o;
	rm -f manual.o;
	rm -f menu.o;
	rm -f namespacer.o;
	rm -f options.o;
	rm -f compile;
	clear;
	

-include $(DEPENDS)
