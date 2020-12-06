#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

static int sum_common_answers(const vector<string>& lines, bool intersect)
{
	int sum = 0;
	int group_count = 0;
	int answers[26];
	fill(answers, answers+26, 0);

	function<bool(int)> predicate = [intersect, &group_count](int value)
	{
		return intersect ? value == group_count : value != 0;
	};

	for (const auto& line: lines)
	{
		if (line.size())
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
			sum += count_if(answers, answers+26, predicate);
			fill(answers, answers+26, 0);
			group_count = 0;
		}
	}
	if (group_count)
	{
		sum += count_if(answers, answers+26, predicate);
	}
	return sum;
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

	vector<string> lines;
	string line;
	while (getline(input, line))
	{
		lines.emplace_back(line);
	}
	input.close();

	cout << "Part1: " << sum_common_answers(lines, false) << endl
	     << "Part2: " << sum_common_answers(lines, true) << endl;
	return 0;
}
