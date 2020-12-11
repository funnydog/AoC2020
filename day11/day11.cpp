#include <algorithm>
#include <fstream>
#include <iostream>
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

size_t count_part1(const vector<string>& seats, size_t x, size_t y)
{
	size_t count = 0;
	size_t height = seats.size();
	size_t width = seats[0].size();
	for (auto [dx, dy]: dir)
	{
		size_t nx = x + dx;
		size_t ny = y + dy;
		if (ny < height && nx < width && seats[ny][nx] == '#')
		{
			count++;
		}
	}
	return count;
}

size_t count_part2(const vector<string>& seats, size_t x, size_t y)
{
	size_t count = 0;
	size_t height = seats.size();
	size_t width = seats[0].size();
	for (auto [dx, dy]: dir)
	{
		size_t nx = x + dx;
		size_t ny = y + dy;
		while (ny < height && nx < width)
		{
			if (seats[ny][nx] == '#')
			{
				count++;
				break;
			}
			else if (seats[ny][nx] == 'L')
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
	return count;
}

int count_stable(const vector<string>& seats,
		 size_t(*countfn)(const vector<string>&, size_t, size_t),
		 size_t maxocc)
{
	vector<string> a(seats);
	vector<string> b(seats);
	vector<string>& cur = a;
	vector<string>& next = b;

	size_t height = seats.size();
	size_t width = seats[0].size();
	do {
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				switch (cur[y][x])
				{
				case '.':
					break;
				case 'L':
					next[y][x] = countfn(cur, x, y) == 0 ? '#': 'L';
					break;
				case '#':
					next[y][x] = countfn(cur, x, y) >= maxocc ? 'L': '#';
					break;
				}
			}
		}
		swap(cur, next);
	} while (a != b);

	size_t c = 0;
	for (auto& row: cur)
	{
		c += count(row.begin(), row.end(), '#');
	}
	return c;
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

	cout << "Part1: " << count_stable(seats, count_part1, 4) << endl
	     << "Part2: " << count_stable(seats, count_part2, 5) << endl;

	return 0;
}
