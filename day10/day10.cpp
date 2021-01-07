#include <algorithm>
#include <fstream>
#include <vector>
#include <fmt/format.h>

using namespace std;

static int joltsmul(const vector<int>& jolts)
{
	int d1 = 1;
	int d3 = 1;
	for (size_t i = 1; i < jolts.size(); i++)
	{
		// check for value-1
		if (jolts[i] - 1 == jolts[i-1])
		{
			d1++;
		}
		else
		{
			// check for value-3
			for (size_t j = 1; j < 4; j++)
			{
				if (i >= j && jolts[i] - 3 == jolts[i - j])
				{
					d3++;
					break;
				}
			}
		}
	}
	return d1 * d3;
}

static uint64_t solutions(const vector<int>& jolts)
{
	// array with the last 4 solutions, index 0 is the last and is
	// set to 1 initially because for the last element we have
	// exactly 1 choice. The fifth element is a convenience one
	// which never changes from zero.
	uint64_t buf[5];
	fill(buf+1, buf+5, 0);
	buf[0] = 1;

	// NOTE: we stop at 1 because i - 1 == 0 otherwise we make an
	// empty inner loop and we overwrite buf[0]
	size_t i = jolts.size();
	while (i-->1)
	{
		for (size_t j = 1; j < 4; j++)
		{
			if (i >= j && jolts[i] - jolts[i-j] <= 3)
			{
				buf[j] += buf[0];
			}
		}

		// NOTE: shift the values left by one position and a
		// zero element is placed at index 3 because element
		// at index 4 is always zero.
		copy(buf+1, buf+5, buf);
	}
	return buf[0];
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fmt::print(stderr, "Usage: {} <filename>\n", argv[0]);
		return 1;
	}

	ifstream input(argv[1]);
	if (!input)
	{
		fmt::print(stderr, "Cannot open {}\n", argv[1]);
		return 1;
	}

	vector<int> jolts;
	int value;
	while (input >> value)
	{
		jolts.push_back(value);
	}
	input.close();

	sort(jolts.begin(), jolts.end());

	fmt::print("Part1: {}\n", joltsmul(jolts));

	jolts.insert(jolts.begin(), 0);
	jolts.push_back(jolts.back()+3);
	fmt::print("Part2: {}\n", solutions(jolts));

	return 0;
}
