#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

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
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return 1;
	}

	ifstream input(argv[1]);
	if (!input)
	{
		cerr << "Cannot open " << argv[1] << endl;
		return 1;
	}

	string line;
	if (!getline(input, line))
	{
		cerr << "Cannot parse the file" << endl;
		return 1;
	}
	input.close();

	cout << "Part1: " << spoken_game(line, 2020) << endl
	     << "Part2: " << spoken_game(line, 30000000ULL) << endl;

	return 0;
}
