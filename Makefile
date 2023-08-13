CXX = g++
CXXFLAGS = -I.

APPNAME = main

END = `pkg-config Qt5Widgets --cflags --libs`

SRCFILES = $(wildcard *.cpp)
OBJFILES = $(addprefix obj/, $(patsubst %.cpp, %.o, $(SRCFILES)))

obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@ $(END)

all: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $^ -o $(APPNAME) $(END)

clean:
	rm obj/*.o