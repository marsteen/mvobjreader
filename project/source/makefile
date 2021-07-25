#**********************************************************************
#
# Obj Reader - makefile
#
# written 2010 by Martin Steen
# martin@martin-steen.de
#
#**********************************************************************
#
# C++ : Name of C++ Compiler
#
C++       = g++
#
#
include exeapp.mk
#
INCPATH       = ../include
LIBBASE       = ../lib
#
#
#
.SUFFIXES : .c .cpp .o
#
#
#
CPPFILES = main.cpp \
           CObjReader.cpp \
           CObjReaderWriteJS.cpp \
           $(LIBBASE)/CFileIO.cpp \
           $(LIBBASE)/CStringTool.cpp \
           $(LIBBASE)/NStringTool.cpp
#
#
CCFLAGS = -O2 -I$(INCPATH)
#
TARGET   = objreader$(EXEAPP)
#
default : $(TARGET)
#
#
.cpp.o:
	$(C++) -c $(CCFLAGS) -o$@ $<
#
#
#
.c.o:
	gcc -c $(CCFLAGS) -o$@ $<


#
clean:
	rm -fv *.o
	rm -fv $(LIBBASE)/*.o
	rm -fv $(TARGET)
#
OBJECTS1 = $(CPPFILES:.cpp=.o)
#
LDFLAGS = 
#
#
# Making the target-exe
	#
$(TARGET) : $(OBJECTS1) $(OBJECTS2)
	$(C++) -o $(TARGET) $(OBJECTS1) $(LDFLAGS) $(CCFLAGS)
	strip $(TARGET)
#
#

