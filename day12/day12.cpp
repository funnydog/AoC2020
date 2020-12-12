#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct vec
{
	int x;
	int y;
};

static vec xmul(const vec v1, const vec v2)
{
	return {
		v1.x * v2.x - v1.y * v2.y,
		v1.x * v2.y + v1.y * v2.x
	};
}

static int manhattan(const vec v)
{
	return (v.x > 0 ? v.x : -v.x) +	(v.y > 0 ? v.y : -v.y);
}

static vec operator+(const vec v1, const vec v2)
{
	return { v1.x + v2.x, v1.y + v2.y};
}

static vec operator*(const vec v1, int n)
{
	return { v1.x * n, v1.y * n };
}

static const vec north = {0, -1 };
static const vec south = {0, 1 };
static const vec east = {1, 0};
static const vec west = {-1, 0};
static const vec rotleft = {0, -1 };
static const vec rotright = {0, 1};

static int navigate_part1(const vector<string>& instructions)
{
	vec pos = {0, 0};
	vec dir = east;
	for (auto& ins: instructions)
	{
		int val = stoi(ins.substr(1));
		switch (ins[0])
		{
		case 'N': pos = pos + north * val; break;
		case 'S': pos = pos + south * val; break;
		case 'E': pos = pos + east * val; break;
		case 'W': pos = pos + west * val; break;
		case 'F': pos = pos + dir * val; break;
		case 'L':
			val /= 90;
			while (val-- > 0)
			{
				dir = xmul(dir, rotleft);
			}
			break;
		case 'R':
			val /= 90;
			while (val-- > 0)
			{
				dir = xmul(dir, rotright);
			}
			break;
		}
	}

	return manhattan(pos);
}

static int navigate_part2(const vector<string>& instructions)
{
	vec pos = {0, 0};
	vec wp = east * 10 + north * 1;
	for (auto& ins: instructions)
	{
		int val = stoi(ins.substr(1));
		switch (ins[0])
		{
		case 'N': wp = wp + north * val; break;
		case 'S': wp = wp + south * val; break;
		case 'E': wp = wp + east * val; break;
		case 'W': wp = wp + west * val; break;
		case 'F': pos = pos + wp * val; break;
		case 'L':
			val /= 90;
			while (val-- > 0)
			{
				wp = xmul(wp, rotleft);
			}
			break;
		case 'R':
			val /= 90;
			while (val-- > 0)
			{
				wp = xmul(wp, rotright);
			}
			break;
		}
	}

	return manhattan(pos);
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

	vector<string> instructions;
	string line;
	while (getline(input, line))
	{
		instructions.emplace_back(move(line));
	}
	input.close();

	cout << "Part1: " << navigate_part1(instructions) << endl
	     << "Part2: " << navigate_part2(instructions) << endl;
	return 0;
}
