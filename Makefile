CXX = g++
CXXFLAGS = -I.

APPNAME = main

SRCFILES = $(wildcard *.cpp)
OBJFILES = $(addprefix obj/, $(patsubst %.cpp, %.o, $(SRCFILES)))

obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

all: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $^ -o $(APPNAME)

clean:
	rm obj/*.o