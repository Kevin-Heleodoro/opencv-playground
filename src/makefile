# Compiler
CC = g++
CXX = $(CC)

# OSX include paths 
CFLAGS = -Wc++11-extensions -std=c++11 -I../include -DENABLE_PRECOMPILED_HEADERS=OFF $(shell pkg-config --cflags opencv4)

# Dwarf include paths
CXXFLAGS = $(CFLAGS)

# opencv libraries
LDLIBS = $(shell pkg-config --libs opencv4)

BINDIR = ../bin

photo: imgDisplay.o filter.o
	$(CC) $^ -o $(BINDIR)/$@.exe $(LDLIBS)

vid: vidDisplay.o filter.o faceDetect.o
	$(CC) $^ -o $(BINDIR)/$@.exe $(LDLIBS)

blur: timeBlur.o filter.o
	$(CC) $^ -o $(BINDIR)/$@.exe $(LDLIBS)

face: showFaces.o filter.o faceDetect.o
	$(CC) $^ -o $(BINDIR)/$@.exe $(LDFLAGS) $(LDLIBS)

fourier: fourier.o
	$(CC) $^ -o $(BINDIR)/$@.exe $(LDFLAGS) $(LDLIBS)

#imod: imgMod.o
#	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)
#
#macbeth: macbeth.o
#	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o *~ 
