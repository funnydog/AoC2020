#include <algorithm>
#include <fstream>
#include <vector>
#include <fmt/format.h>

using namespace std;

static void simulate(vector<unsigned>& v, vector<unsigned>& next, size_t count)
{
	auto [imin, imax] = minmax_element(v.begin(), v.end());
	int min = *imin;
	int max = *imax;
	int cur = 0;
	while (count-->0)
	{
		int a, b, c;
		int start, end;

		// pick up the next 3 numbers
		start = next[v[cur]];
		a = v[start];
		end = next[a];
		b = v[end];
		end = next[b];
		c = v[end];

		// new value
		int value = v[cur];
		do
		{
			value -= 1;
			if (value < min)
			{
				value = max;
			}
		} while (value == a || value == b || value == c);

		// update the links
		next[v[cur]] = next[v[end]];
		next[v[end]] = next[value];
		next[value] = start;
		cur = next[v[cur]];
	}
}

static vector<unsigned> from_pattern(const string& pattern)
{
	vector<unsigned> v;
	for (auto c: pattern)
	{
		if (isdigit(c))
		{
			v.push_back(c-'0');
		}
	}
	return v;
}

static string part1(const string& pattern)
{
	auto v = from_pattern(pattern);
	if (v.empty())
	{
		return "pattern error";
	}
	vector<unsigned> next(v.size()+1, 0);
	for (size_t i = 0; i < v.size()-1; i++)
	{
		next[v[i]] = i + 1;
	}

	// simulate 100 moves
	simulate(v, next, 100);

	// find the elements after 1
	string rv;
	int i = next[1];
	while (v[i] != 1)
	{
		rv.push_back('0' + v[i]);
		i = next[v[i]];
	}
	return rv;
}

static size_t part2(const string& pattern)
{
	auto v = from_pattern(pattern);
	if (v.empty())
	{
		return 0;
	}
	for (size_t i = v.size()+1; i <= 1000000ULL; i++)
	{
		v.push_back(i);
	}

	vector<unsigned> next(v.size()+1, 0);
	for (size_t i = 0; i < v.size()-1; i++)
	{
		next[v[i]] = i + 1;
	}

	// simulate ten millions moves
	simulate(v, next, 10000000ULL);

	int i = next[1];
	size_t a = v[i];
	i = next[a];
	size_t b = v[i];
	return a * b;
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
	string pattern;
	getline(input, pattern);
	input.close();
	if (input.fail())
	{
		fmt::print(stderr, "Cannot load the pattern\n");
		return 1;
	}

	fmt::print("Part1: {}\n", part1(pattern));
	fmt::print("Part2: {}\n", part2(pattern));
	return 0;

}
