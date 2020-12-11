#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

const pair<int, int> dir[] = {
	{0, -1},
	{1, -1},
	{1, 0},
	{1, 1},
	{0, 1},
	{-1, 1},
	{-1, 0},
	{-1, -1}
};

void next_part1(const vector<string>& cur, vector<string>& next)
{
	size_t height = cur.size();
	size_t width = cur[0].size();
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			if (cur[y][x] == '.')
			{
				continue;
			}

			// count the occupied adjacent places
			size_t count = 0;
			for (auto [dx, dy]: dir)
			{
				size_t nx = x + dx;
				size_t ny = y + dy;
				if (ny < height && nx < width && cur[ny][nx] == '#')
				{
					count++;
				}
			}

			// generate the next state for the element at (y,x)
			switch(cur[y][x])
			{
			case 'L':
				next[y][x] = count == 0 ? '#': 'L';
				break;
			case '#':
				next[y][x] = count >= 4 ? 'L': '#';
				break;
			}
		}
	}
}

void next_part2(const vector<string>& cur, vector<string>& next)
{
	size_t height = cur.size();
	size_t width = cur[0].size();
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			if (cur[y][x] == '.')
			{
				continue;
			}

			// count the occupied adjacent places
			size_t count = 0;
			for (auto [dx, dy]: dir)
			{
				size_t nx = x + dx;
				size_t ny = y + dy;
				while (ny < height && nx < width)
				{
					if (cur[ny][nx] == '#')
					{
						count++;
						break;
					}
					else if (cur[ny][nx] == 'L')
					{
						break;
					}
					else
					{
						nx += dx;
						ny += dy;
					}
				}
			}

			// generate the next state for the element at (y,x)
			switch(cur[y][x])
			{
			case 'L':
				next[y][x] = count == 0 ? '#': 'L';
				break;
			case '#':
				next[y][x] = count >= 5 ? 'L': '#';
				break;
			}
		}
	}
}

int count_stable(const vector<string>& seats,
		 void(*nextfn)(const vector<string>&, vector<string>&))
{
	vector<string> a(seats);
	vector<string> b(seats);
	vector<string>& cur = a;
	vector<string>& next = b;

	do {
		nextfn(cur, next);
		swap(cur, next);
	} while (a != b);

	return accumulate(
		cur.cbegin(),
		cur.cend(),
		0,
		[](size_t acc, const auto& row) -> size_t {
			return acc + count(row.begin(), row.end(), '#');
		});
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

	vector<string> seats;
	string line;
	while (getline(input, line))
	{
		seats.emplace_back(move(line));
	}
	input.close();

	cout << "Part1: " << count_stable(seats, next_part1) << endl
	     << "Part2: " << count_stable(seats, next_part2) << endl;

	return 0;
}
