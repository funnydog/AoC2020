#include <iostream>
#include <fstream>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

struct Map
{
	vector<string> rows;
	unsigned width;
	unsigned height;

	unsigned count_trees(int dx, int dy) const
	{
		unsigned count = 0;
		unsigned x = 0;
		unsigned y = 0;
		for (;;)
		{
			x = (x + dx) % width;
			y = y + dy;
			if (y >= height)
			{
				break;
			}

			if (rows[y][x] == '#')
			{
				count++;
			}
		}
		return count;
	}
};

istream& operator>>(istream& input, Map& map)
{
	map.width = map.height = 0;
	map.rows.clear();
	string line;
	while (getline(input, line))
	{
		if (line.size()> 0)
		{
			map.width = line.size();
			map.rows.emplace_back(move(line));
		}
	}
	map.height = map.rows.size();
	return input;
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

	Map map;
	input >> map;
	input.close();

	cout << "Part1: " << map.count_trees(3, 1) << endl;

	const pair<int, int> SLOPES[] = {
		make_pair(1, 1),
		make_pair(3, 1),
		make_pair(5, 1),
		make_pair(7, 1),
		make_pair(1, 2),
	};
	cout << "Part2: "
	     << accumulate(
		     SLOPES,
		     SLOPES + sizeof(SLOPES)/sizeof(SLOPES[0]),
		     1ULL,
		     [&map](unsigned long long a, const pair<int,int>& p) {
			     return a * map.count_trees(p.first, p.second);
		     })
	     << endl;

	return 0;
}
