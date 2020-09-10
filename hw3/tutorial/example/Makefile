CXX = clang++
CFLAGS = -Wall -std=c++11

bin = main.out

.PHONY: clean

all: $(bin)

$(bin): main.cpp ChildVisitor.o AdultVisitor.o
	$(CXX) $(CFLAGS) $^ -o $@ && ./$@

ChildVisitor.o: ChildVisitor.cpp visitor.hpp ChildVisitor.hpp
	$(CXX) $(CFLAGS) -c $< -o $@

AdultVisitor.o: AdultVisitor.cpp visitor.hpp AdultVisitor.hpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(bin) *.out *.o
