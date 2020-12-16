CXXFLAGS=-Wall -std=c++17 -O2

.PHONY: all clean

all: day16

day16: day16.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm *.o day16
