#include <algorithm>
#include <fstream>
#include <iostream>

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
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return 1;
	}

	ifstream input(argv[1]);
	if (!input)
	{
		cerr << "Cannot open " << argv[1] << endl;
		return 1;
	}

	auto sums = sum_common_answers(input);
	input.close();

	cout << "Part1: " << sums.first << endl
	     << "Part2: " << sums.second << endl;

	return 0;
}
