CXX := g++
CXXFLAGS := -g -Wall --std=c++11
GTESTINCL := -I /usr/include/gtest/  
GTESTLIBS := -lgtest -lgtest_main  -lpthread
VALGRIND := valgrind --tool=memcheck --leak-check=yes

all: rh llrec-test stack heap

# generic rule for .cpp files
board.o : board.cpp board.h vehicle.h 
	$(CXX) $(CXXFLAGS) -c $< -o $@ 
heur.o : heur.cpp heur.h board.h vehicle.h 
	$(CXX) $(CXXFLAGS) -c $< -o $@ 
move.o : move.cpp move.h board.h heur.h vehicle.h 
	$(CXX) $(CXXFLAGS) -c $< -o $@ 
solver.o : solver.cpp solver.h move.h board.h heur.h vehicle.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 
rh : rh.cpp board.o heur.o move.o solver.o
	$(CXX) $(CXXFLAGS) $^ -o $@ 

stack: stack_test.cpp stack.h
	$(CXX) $(CXXFLAGS) $^ -o $@

heap: heap.cpp heap.h
	$(CXX) $(CXXFLAGS) $^ -o $@


#-----------------------------------------------------
# ADD target(s) to build your llrec-test executable
#-----------------------------------------------------
llrec-test: llrec-test.cpp llrec-test.o
	$(CXX) $(CXXFLAGS) $(GTESTINCL) $^ -o $@ $(GTESTLIBS)
llrec-test.o: llrec.cpp llrec.h
	$(CXX) $(CXXFLAGS) -c $< -o $@



clean:
	rm -f *.o rh llrec-test stack *~

.PHONY: clean 