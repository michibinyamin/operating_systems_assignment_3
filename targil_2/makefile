CXX = g++
CXXFLAGS = -pg -O2

all: profile_deque profile_deque_matrix profile_list profile_list_matrix

profile_deque:
	$(CXX) $(CXXFLAGS) deque.cpp -o deque.out
	./deque.out < input.txt
	gprof deque.out gmon.out > analysis_deque.txt
	rm -f gmon.out

profile_deque_matrix:
	$(CXX) $(CXXFLAGS) deque_matrix.cpp -o deque_matrix.out
	./deque_matrix.out < input.txt
	gprof deque_matrix.out gmon.out > analysis_deque_matrix.txt
	rm -f gmon.out

profile_list:
	$(CXX) $(CXXFLAGS) list.cpp -o list.out
	./list.out < input.txt
	gprof list.out gmon.out > analysis_list.txt
	rm -f gmon.out

profile_list_matrix:
	$(CXX) $(CXXFLAGS) list_matrix.cpp -o list_matrix.out
	./list_matrix.out < input.txt
	gprof list_matrix.out gmon.out > analysis_list_matrix.txt
	rm -f gmon.out

clean:
	rm -f deque.out deque_matrix.out list.out list_matrix.out gmon.out analysis_*.txt
