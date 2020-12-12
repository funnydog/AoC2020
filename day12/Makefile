CXXFLAGS=-Wall -std=c++17 -O2

.PHONY: all clean

all: day12

day12: day12.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f *.o day12
