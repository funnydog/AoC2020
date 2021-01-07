#include <cassert>
#include <fstream>
#include <vector>
#include <fmt/format.h>

using namespace std;

static int spoken_game(const string& seq, size_t count)
{
	vector<int> spoken(count);

	size_t i = 0;
	size_t pos = 0;
	int last;
	for (;;)
	{
		size_t next = seq.find(',', pos);
		last = stoi(seq.substr(pos, next-pos));
		spoken[last] = ++i;
		if (next == string::npos)
		{
			break;
		}
		pos = next + 1;
	}

	for (; i < count; i++)
	{
		int next = spoken[last] ? (i - spoken[last]) : 0;
		spoken[last] = i;
		last = next;
	}

	return last;
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

	string line;
	getline(input, line);
	input.close();
	if (input.fail())
	{
		fmt::print(stderr, "Cannot parse the file\n");
		return 1;
	}

	fmt::print("Part1: {}\n", spoken_game(line, 2020));
	fmt::print("Part2: {}\n", spoken_game(line, 30000000ULL));
	return 0;
}
