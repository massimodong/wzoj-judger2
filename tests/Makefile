SRCS := $(wildcard *.cc)
OBJS := $(SRCS:cc=o) comparer.o

test: $(OBJS)
	g++ $(OBJS) -lgtest_main  -lgtest -lpthread -lglog -o test

comparer.o: ../src/comparer.cc
	g++ -c ../src/comparer.cc

%.o: %.cc
	g++ -c $< -I../src

.PHONY: clean
clean:
	rm $(OBJS) test
