# Compiler
CC = g++
CXX = $(CC)

# OSX include paths 
CFLAGS = -Wc++11-extensions -std=c++11 -I../include -DENABLE_PRECOMPILED_HEADERS=OFF $(shell pkg-config --cflags opencv4)
# CFLAGS = -Wc++11-extensions -std=c++11 -I/usr/local/Cellar/opencv/4.8.1_5/include/opencv4 -I../include -DENABLE_PRECOMPILED_HEADERS=OFF

# Dwarf include paths
CXXFLAGS = $(CFLAGS)

# Dwarf Library paths
# LDFLAGS = -L/usr/local/Cellar/opencv/4.8.1_5/lib/opencv4 -L/usr/local/Cellar/opencv/4.8.1_5/lib

# opencv libraries
LDLIBS = $(shell pkg-config --libs opencv4)
# LDLIBS = -ltiff -lpng -ljpeg -llapack -lblas -lz  -lwebp -framework AVFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices -framework CoreGraphics -framework AppKit -framework OpenCL  -lopencv_core -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect

BINDIR = ../bin

photo: imgDisplay.o
	echo "Photo"
	$(CC) $^ -o $(BINDIR)/$@ $(LDLIBS)
#	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

vid: vidDisplay.o 
	$(CC) $^ -o $(BINDIR)/$@ $(LDLIBS)
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
