#include <fstream>
#include <numeric>
#include <regex>
#include <unordered_map>
#include <vector>
#include <fmt/format.h>

using namespace std;

static const regex MASKP("mask = ([01X]{36})");
static const regex MEMP("mem\\[(\\d+)\\] = (\\d+)");

static size_t part1(const vector<string>& program)
{
	unordered_map<size_t, size_t> mem;
	string mask;
	for (auto& line: program)
	{
		smatch sm;
		if (regex_match(line, sm, MASKP))
		{
			mask = sm[1];
		}
		else if (regex_match(line, sm, MEMP))
		{
			size_t address = stoull(sm[1]);
			size_t before = stoull(sm[2]);
			size_t after = 0;
			for (size_t i = 0; i < 36; i++)
			{
				switch (mask[i])
				{
				case 'X': after |= before & (1ULL<<(35-i)); break;
				case '1': after |= 1ULL<<(35-i); break;
				case '0': break;
				}
			}
			mem[address] = after;
		}
	}
	return accumulate(
		mem.cbegin(),
		mem.cend(),
		0ULL,
		[](auto acc, const auto& v) {
			return acc + v.second;
		});
}

static void gen(const string& pattern, size_t pos, string& cur, vector<size_t>& out)
{
	if (pos == pattern.size())
	{
		size_t val = 0;
		for (auto c: cur)
		{
			val |= c == '1';
			val <<= 1;
		}
		out.push_back(val);
	}
	else if (pattern[pos] != 'X')
	{
		cur[pos] = pattern[pos];
		gen(pattern, pos+1, cur, out);
	}
	else
	{
		cur[pos] = '0';
		gen(pattern, pos+1, cur, out);
		cur[pos] = '1';
		gen(pattern, pos+1, cur, out);
	}
}

static size_t part2(const vector<string>& program)
{
	unordered_map<size_t, size_t> mem;
	vector<size_t> addresses;
	string mask;
	for(auto& line: program)
	{
		smatch sm;
		if (regex_match(line, sm, MASKP))
		{
			mask = sm[1];
		}
		else if (regex_match(line, sm, MEMP))
		{
			size_t addr = stoull(sm[1]);
			size_t result = stoull(sm[2]);
			string pattern(36, ' ');
			for (size_t i = 0; i < 36; i++)
			{
				switch(mask[i])
				{
				case 'X':
				case '1':
					pattern[i] = mask[i];
					break;
				case '0':
					if (addr & (1ULL << 35))
					{
						pattern[i] = '1';
					}
					else
					{
						pattern[i] = '0';
					}
					break;
				}
				addr <<= 1;
			}
			addresses.clear();
			string tmp('0', 36);
			gen(pattern, 0, tmp, addresses);
			for (auto a: addresses)
			{
				mem[a] = result;
			}
		}
	}
	return accumulate(
		mem.cbegin(),
		mem.cend(),
		0ULL,
		[](auto acc, const auto& v) {
			return acc + v.second;
		});
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

	vector<string> program;
	string line;
	while (getline(input, line))
	{
		program.emplace_back(move(line));
	}
	input.close();

	fmt::print("Part1: {}\nPart2: {}\n", part1(program), part2(program));
	return 0;
}
