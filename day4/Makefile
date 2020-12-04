CXXFLAGS=-Wall -std=c++17 -O2

.PHONY: all clean

all: day4

day4: day4.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f day4 *.o
