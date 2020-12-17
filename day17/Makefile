CXXFLAGS=-Wall -std=c++17 -O2

.PHONY: all clean

all: day17

day17: day17.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f *.o day17
