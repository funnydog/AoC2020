#include <algorithm>
#include <fstream>
#include <fmt/format.h>

using namespace std;

static pair<int, int> sum_common_answers(istream& input)
{
	int any = 0;
	int all = 0;
	int group_count = 0;
	int answers[26];
	fill(answers, answers+26, 0);

	string line;
	do
	{
		if (getline(input, line) && line.size())
		{
			for (auto letter: line)
			{
				if ('a' <= letter && letter <= 'z')
				{
					answers[letter-'a']++;
				}
			}
			group_count++;
		}
		else if (group_count)
		{
			for (auto a: answers)
			{
				any += a > 0;
				all += a == group_count;
			}
			fill(answers, answers+26, 0);
			group_count = 0;
		}
	} while (input);

	return make_pair(any, all);
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

	auto [part1, part2] = sum_common_answers(input);
	input.close();

	fmt::print("Part1: {}\nPart2: {}\n", part1, part2);
	return 0;
}
