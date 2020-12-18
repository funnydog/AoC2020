CXXFLAGS=-Wall -std=c++17 -O2

.PHONY: all clean

all: day18

day18: day18.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	@rm -f *.o day18
