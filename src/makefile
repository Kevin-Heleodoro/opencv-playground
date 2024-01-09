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

photo: imgDisplay.o
	echo "Photo"
	$(CC) $^ -o $(BINDIR)/$@.exe $(LDLIBS)

vid: vidDisplay.o 
	$(CC) $^ -o $(BINDIR)/$@.exe $(LDLIBS)
#
#face: faceDetect.o filters.o
#	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)
#
#imod: imgMod.o
#	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)
#
#macbeth: macbeth.o
#	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o *~ 
