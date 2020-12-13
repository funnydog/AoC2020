#include <numeric>
#include <fstream>
#include <iostream>
#include <cinttypes>
#include <vector>

using namespace std;

static vector<intmax_t> parse_busses(const string& busses)
{
	vector<intmax_t> bids;
	size_t pos = 0, next;
	do {
		next = busses.find(',', pos);
		if (busses.substr(pos, next-pos) == "x")
		{
			bids.push_back(-1);
		}
		else
		{
			bids.push_back(stoll(busses.substr(pos, next-pos)));
		}
		pos = next + 1;
	} while (next != string::npos);
	return bids;
}

static intmax_t part1(intmax_t earliest, const vector<intmax_t>& bids)
{
	intmax_t wait = INTMAX_MAX;
	intmax_t bestid = 0;
	for (auto bid: bids)
	{
		if (bid != -1)
		{
			intmax_t time = bid-1 - ((earliest + bid - 1) % bid);
			if (wait > time)
			{
				wait = time;
				bestid = bid;
			}
		}
	}
	return bestid * wait;
}

static intmax_t bezout(intmax_t a, intmax_t b)
{
	intmax_t old_r = b;
	intmax_t r = a;
	intmax_t old_s = 1;
	intmax_t s = 0;

	while (r)
	{
		auto d = imaxdiv(old_r, r);
		swap(old_r, r);
		r = d.rem;

		swap(old_s, s);
		s -= d.quot * old_s;
	}

	if (old_s < 0)
	{
		old_s += a;
	}

	return old_s * b - 1;
}

static intmax_t part2(const vector<intmax_t>& bids)
{
	intmax_t fullcycle = accumulate(
		bids.begin(),
		bids.end(),
		1LL,
		[](auto a, auto b) {
			return b != -1 ? a * b : a;
		});

	intmax_t start = 0;
	intmax_t offset = 1;
	intmax_t prev = bids[0];
	for (size_t i = 1; i < bids.size(); i++)
	{
		if (bids[i] == -1)
		{
			offset++;
		}
		else
		{
			start += bezout(prev, fullcycle / prev) * offset;
			start %= fullcycle;
			prev *= bids[i];
			offset = 1;
		}
	}
	return start;
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

	size_t earliest;
	string busses;
	if (!(input >> earliest) || !getline(input, busses) || !getline(input, busses))
	{
		cerr << "Cannot parse the data" << endl;
		input.close();
		return 1;
	}
	input.close();
	auto bids = parse_busses(busses);

	cout << "Part1: " << part1(earliest, bids) << endl
	     << "Part2: " << part2(bids) << endl;
	return 0;
}
