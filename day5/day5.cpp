#include <algorithm>
#include <cassert>
#include <fstream>
#include <vector>
#include <fmt/format.h>

using namespace std;

static int find_id(const string& line)
{
	int value = 0;
	for (auto letter : line)
	{
		switch (letter)
		{
		case 'B':
		case 'R':
			value = value << 1 | 1;
			break;
		case 'F':
		case 'L':
			value <<= 1;
			break;
		default:
			break;
		}
	}
	return value;
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

	assert(find_id("FBFBBFFRLR"s) == 357);
	assert(find_id("BFFFBBFRRR"s) == 567);
	assert(find_id("FFFBBBFRRR"s) == 119);
	assert(find_id("BBFFBBFRLL"s) == 820);

	vector<int> seats;
	string line;
	while (getline(input, line) && line.size())
	{
		seats.push_back(find_id(line));
	}
	input.close();

	sort(seats.begin(), seats.end(), std::greater<int>());
	int highest = -1;
	int expected = -1;
	auto it = seats.cbegin();
	if (it != seats.cend())
	{
		highest = *it;
		expected = highest - 1;
		for (++it; it != seats.cend(); ++it)
		{
			if (*it != expected)
			{
				break;
			}
			expected = *it - 1;
		}
	}

	fmt::print("Part1: {}\nPart2: {}\n", highest, expected);
	return 0;
}
