CXX = g++
CXXFLAGS = -I. `C:/msys64/usr/bin/pkg-config.exe --cflags --libs gtkmm-3.0`

APPNAME = main

SRCFILES = $(wildcard *.cpp)
OBJFILES = $(addprefix obj/, $(patsubst %.cpp, %.o, $(SRCFILES)))

obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

all: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $^ -o $(APPNAME)

clean:
	rm obj/*.o