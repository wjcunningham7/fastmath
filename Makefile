BINDIR	:= ./bin
INCDIR	:= ./inc
SRCDIR	:= ./src
ASMDIR	:= ./asm
OBJDIR	:= ./obj
LIBDIR	:= ./lib
DATDIR	:= ./dat

LOCAL_DIR ?= /usr/local

CXX	:= g++
INCD	:= -I $(INCDIR) -I $(LOCAL_DIR)/include
LIBS	:= -L $(LD_LIBRARY_PATH) -L $(LOCAL_DIR)/lib64 -lm -lnint -lgsl -lgslcblas

FLAGS	:= -O3 -g
ASMFLAGS:= 

SOURCES	:= $(SRCDIR)/stopwatch.cpp $(SRCDIR)/ran2.cpp $(SRCDIR)/FastMath.cpp $(SRCDIR)/FastNumInt.cpp $(SRCDIR)/BenchFastMath.cpp
SOURCES2:= $(SRCDIR)/stopwatch.cpp $(SRCDIR)/ran2.cpp $(SRCDIR)/FastMath.cpp $(SRCDIR)/FastNumInt.cpp $(SRCDIR)/BenchFastNumInt.cpp
SOURCES3:= $(SRCDIR)/FastMath.cpp $(SRCDIR)/FastNumInt.cpp $(SRCDIR)/TestFastNumInt.cpp
ASMS	:= $(patsubst $(SRCDIR)/%.cpp, $(ASMDIR)/%.s, $(SOURCES))
OBJS	:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
OBJS2	:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES2))
OBJS3	:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES3))
EXCL	:= $(OBJDIR)/TestFastNumInt.o $(OBJDIR)/BenchFastMath.o $(OBJDIR)/BenchFastNumInt.o
LIBOBJS	:= $(filter-out $(EXCL), $(wildcard $(OBJDIR)/*.o))

LIB	:= $(LIBDIR)/libfastmath.a
BIN	:= $(BINDIR)/BenchFastMath
BIN2	:= $(BINDIR)/BenchFastNumInt
BIN3	:= $(BINDIR)/TestFastNumInt

.PHONY : check-env dirs lib bin bin2 bin3 install clean cleandata

all : check-env dirs $(OBJS)

assemble : check-env dirs $(ASMS)

check-env :
	@ if test "$(LOCAL_DIR)" = "" ; then \
		echo "LOCAL_DIR not set!"; \
		exit 1; \
	fi

$(ASMS) : | dirs

$(ASMDIR)/%.s : $(SRCDIR)/%.cpp
	$(CXX) $(ASMFLAGS) -S $(INCD) -o $@ $<

dirs :
	@ mkdir -p $(OBJDIR); \
	mkdir -p $(ASMDIR); \
	mkdir -p $(BINDIR); \
	mkdir -p $(LIBDIR);

$(OBJS) : | dirs

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(FLAGS) -c $(INCD) -o $@ $<

lib : dirs $(LIBOBJS)
	ar rvs $(LIB) $(LIBOBJS)

bin : $(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(INCD) $(LIBS)

bin2 : $(OBJS2)
	$(CXX) -o $(BIN2) $(OBJS2) $(INCD) $(LIBS)

bin3 : $(OBJS3)
	$(CXX) -o $(BIN3) $(OBJS3) $(INCD) $(LIBS)

install : check-env lib
	mkdir -p $(LOCAL_DIR)/lib64
	mkdir -p $(LOCAL_DIR)/include/fastmath
	mkdir -p $(LOCAL_DIR)/src/fastmath
	cp $(LIB) $(LOCAL_DIR)/lib64/
	cp $(INCDIR)/fastapprox.h $(LOCAL_DIR)/include/fastmath/
	cp $(INCDIR)/FastMath.h $(LOCAL_DIR)/include/fastmath/
	cp $(INCDIR)/FastNumInt.h $(LOCAL_DIR)/include/fastmath/
	cp $(SRCDIR)/ran2.cpp $(LOCAL_DIR)/src/fastmath/
	cp $(SRCDIR)/stopwatch.cpp $(LOCAL_DIR)/src/fastmath/

clean:
	@ rm -rf $(BINDIR) $(OBJDIR) $(LIBDIR) $(ASMDIR) ./*.log

cleandata :
	@ rm -f $(DATDIR)/*.dat
