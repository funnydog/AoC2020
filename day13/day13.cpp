#include <algorithm>
#include <fstream>
#include <climits>
#include <vector>
#include <fmt/format.h>

using namespace std;

static vector<int> parse_busses(const string& busses)
{
	vector<int> bids;
	size_t pos = 0, next;
	do {
		next = busses.find(',', pos);
		if (busses.substr(pos, next-pos) == "x")
		{
			bids.push_back(0);
		}
		else
		{
			bids.push_back(stoll(busses.substr(pos, next-pos)));
		}
		pos = next + 1;
	} while (next != string::npos);
	return bids;
}

static int part1(int earliest, const vector<int>& bids)
{
	int wait = INT_MAX;
	int bestid = 0;
	for (auto bid: bids)
	{
		if (bid)
		{
			int time = bid-1 - ((earliest + bid - 1) % bid);
			if (wait > time)
			{
				wait = time;
				bestid = bid;
			}
		}
	}
	return bestid * wait;
}

static intmax_t part2(const vector<int>& bids)
{
	vector<pair<int, int>> nums;
	for (int i = 0; i < (int)bids.size(); i++)
	{
		auto n = bids[i];
		if (n)
		{
			// NOTE: mathematically correct modulo of
			// negative numbers
			auto a = ((n - i) % n + n) % n;
			nums.emplace_back(make_pair(n, a));
		}
	}

	sort(nums.begin(), nums.end(), [](auto a, auto b) {
		return a.first > b.first;
	});
	intmax_t p = 1;
	intmax_t x = 0;
	for (auto [n, a]: nums)
	{
		while (x % n != a)
		{
			x += p;
		}
		p *= n;
	}
	return x;
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

	size_t earliest;
	string busses;
	input >> earliest;
	getline(input, busses);
	getline(input, busses);
	input.close();
	if (input.fail())
	{
		fmt::print(stderr, "Cannot parse the data\n");
		return 1;
	}

	auto bids = parse_busses(busses);

	fmt::print("Part1: {}\n", part1(earliest, bids));
	fmt::print("Part2: {}\n", part2(bids));
	return 0;
}
