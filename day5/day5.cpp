#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

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
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return 1;
	}

	ifstream input(argv[1]);
	if (!input)
	{
		cerr << "Cannot open " << argv[1] << endl;
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
	cout << "Part1: " << highest << endl
	     << "Part2: " << expected << endl;

	return 0;
}
