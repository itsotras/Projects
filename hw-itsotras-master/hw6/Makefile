CXX=g++
CXXFLAGS=-g -Wall -std=c++11 
GTESTINCL := -I /usr/include/gtest/  
GTESTLIBS := -lgtest -lgtest_main  -lpthread
# Uncomment for parser DEBUG
#DEFS=-DDEBUG

.PHONY: clean run-hash-check

all: ht-test str-hash-test graphiso-driver hash-check

ht-test: ht-test.cpp ht.h
	$(CXX) $(CXXFLAGS) $(DEFS) $< -o $@

str-hash-test: str-hash-test.cpp hash.h
	$(CXX) $(CXXFLAGS) $(DEFS) $< -o $@

graphiso.o: graphiso.cpp graphiso.h
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@

graphiso-driver: graphiso-driver.cpp graphiso.o
	$(CXX) $(CXXFLAGS) $(DEFS) $^ -o $@

hash-check: hash-check.cpp hash.h
	$(CXX) $(CXXFLAGS) $(DEFS) $(GTESTINCL) $< -o $@ $(GTESTLIBS)

run-hash-check: hash-check
	valgrind --tool=memcheck --leak-check=yes ./hash-check


clean:
	rm -f *~ *.o ht-test str-hash-test graphiso-driver hash-check
