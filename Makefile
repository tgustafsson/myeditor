CC=g++
#CFLAGS=-O1 -I. -std=c++11 -Iz:/boost_1_60_0/boost_1_60_0 -Lz:/boost_1_60_0/boost_1_60_0/stage/lib
CFLAGS=-I. -std=c++11 -g
LIBS=-lncursesw -lboost_filesystem -lboost_system -lboost_program_options
OBJ=AttributedString.o Clipboard.o CommandHistory.o Control.o CppMode.o CursesControl.o CursesView.o Debug.o FileSelectMode.o FundamentalMode.o IncrementalSearch.o IncrementalSearchMode.o Macro.o Mode.o Model.o Selection.o SelectionMode.o SoftwrapAttributedString.o SoftwrapControl.o myeditor.o LatexMode.o DummyView.o Cords.o CommandLine.o Commands.o Utilities.o TabsMode.o

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp %.h
	$(CC) -c -o $@ $< $(CFLAGS)

myeditor: $(OBJ)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -rf *.o






