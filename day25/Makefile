CXXFLAGS=-Wall -std=c++17 -Os
LDLIBS=-lfmt

.PHONY: all clean

all: day25

day25: day25.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f *.o day25
