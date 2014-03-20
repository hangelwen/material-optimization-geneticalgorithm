################################################################################
# Makefile for HMM tagger
################################################################################

################################################################################
# 1) Choose C++ compiler.
################################################################################

CXX = g++

################################################################################
# 2) Set C++ flags.
#    a) DEBUG mode -- no optimizations, enable SafeVector checking, no inlining
#    b) PROFILE mode -- for gprof
#    c) RELEASE mode
################################################################################


CXXFLAGS = -O3 -W -Wall
# debug mode
#CXXFLAGS = -g

################################################################################
# 3) Dependencies
################################################################################

TARGETS = material-optimizer
DEPS =  comp_laminate.h material.h 
OBJS =  main.o

all : $(TARGETS)
material-optimizer :	$(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
main.o :main.cpp 
	$(CXX) -c -o $@ $< $(CXXFLAGS)


.PHONY : clean
clean:
	rm -f $(TARGETS) *.o *.h~ *.hpp~ *.cpp~ *.cc~ 
