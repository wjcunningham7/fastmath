BINDIR	:= ./bin
INCDIR	:= ./inc
SRCDIR	:= ./src
OBJDIR	:= ./obj
LIBDIR	:= ./lib
DATDIR	:= ./dat

CXX	:= /usr/bin/g++
INCD	:= -I $(INCDIR)
LIBS	:= -L $(LD_LIBRARY_PATH) -lm -lnint -lgsl -lgslcblas

FLAGS	:= -O3 -g

SOURCES	:= $(SRCDIR)/stopwatch.cpp $(SRCDIR)/ran2.cpp $(SRCDIR)/FastMath.cpp $(SRCDIR)/BenchFastMath.cpp
SOURCES2:= $(SRCDIR)/stopwatch.cpp $(SRCDIR)/ran2.cpp $(SRCDIR)/FastMath.cpp $(SRCDIR)/FastNumInt.cpp $(SRCDIR)/BenchFastNumInt.cpp
SOURCES3:= $(SRCDIR)/FastMath.cpp $(SRCDIR)/FastNumInt.cpp $(SRCDIR)/TestFastNumInt.cpp
OBJS	:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
OBJS2	:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES2))
OBJS3	:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES3))
EXCL	:= $(OBJDIR)/TestFastNumInt.o $(OBJDIR)/BenchFastMath.o $(OBJDIR)/BenchFastNumInt.o $(OBJDIR)/stopwatch.o $(OBJDIR)/ran2.o
LIBOBJS	:= $(filter-out $(EXCL), $(wildcard $(OBJDIR)/*.o))

LIB	:= $(LIBDIR)/libfastmath.a
BIN	:= $(BINDIR)/BenchFastMath
BIN2	:= $(BINDIR)/BenchFastNumInt
BIN3	:= $(BINDIR)/TestFastNumInt

all : $(OBJS) bin bin2 bin3

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(FLAGS) -c $(INCD) -o $@ $<

lib : $(LIBOBJS)
	ar rvs $(LIB) $(LIBOBJS)

bin : $(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(INCD) $(LIBS)

bin2 : $(OBJS2)
	$(CXX) -o $(BIN2) $(OBJS2) $(INCD) $(LIBS)

bin3 : $(OBJS3)
	$(CXX) -o $(BIN3) $(OBJS3) $(INCD) $(LIBS)

clean:
	rm -f $(BIN) $(OBJDIR)/*.o $(LIBDIR)/*.a ./*.log

cleandata :
	rm -f $(DATDIR)/*.dat
