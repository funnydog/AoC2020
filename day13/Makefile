CXXFLAGS=-Wall -std=c++17 -Os

.PHONY: all clean

all: day13

day13: day13.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f *.o day13
