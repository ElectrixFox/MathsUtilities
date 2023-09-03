CXX = g++
CXXFLAGS = -I. -Winvalid-pch -includeqt_pch.h

APPNAME = main

# these are the defines from --cflags
END =`pkg-config Qt5Widgets --libs` -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
# add --cflags (before --libs) if not working

MOCS = $(wildcard *.h)

# the specific files to moc (reduces compile time a bit)
SMOCS = GraphicWindow.h MainWindow.h
OBJMOCS = $(addprefix obj/moc_, $(patsubst %.h, %.cpp, $(SMOCS)))

SRCFILES = $(wildcard *.cpp)
OBJFILES = $(addprefix obj/, $(patsubst %.cpp, %.o, $(SRCFILES)))
OBJFILES += $(OBJMOCS)

obj/moc_%.cpp: %.h
	moc $< -o $@

#obj/moc_MainWindow.cpp: MainWindow.h
#    moc MainWindow.h -o moc_MainWindow.cpp

obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@ $(END)

all: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $^ -o $(APPNAME) $(END)

# precompile that fat header
pch:
	$(CXX) -o qt_pch.h.gch -x c++-header qt_pch.h `pkg-config --cflags Qt5Widgets Qt5Gui Qt5Core`
# add the actual output of pkg-config to the final compile

clean:
	rm obj/*
