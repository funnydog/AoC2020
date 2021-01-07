#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <fmt/format.h>

using namespace std;

static int part1(const unordered_set<int>& numbers)
{
	auto it = find_if(
		numbers.cbegin(),
		numbers.cend(),
		[&numbers](auto value) -> bool {
			return numbers.find(2020-value) != numbers.end();
		});
	if (it == numbers.cend())
	{
		return -1;
	}
	return *it * (2020-*it);
}

static int part2(const unordered_set<int>& numbers)
{
	for (auto l1 = numbers.cbegin(); l1 != numbers.cend(); ++l1)
	{
		for (auto l2 = numbers.cbegin(); l2 != l1; ++l2)
		{
			if (numbers.find(2020 - *l1 - *l2) != numbers.end())
			{
				return *l1 * *l2 * (2020 - *l1 - *l2);
			}
		}
	}
	return -1;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fmt::print(stderr, "Usage: {} <filename>\n", argv[0]);
		return 1;
	}

	ifstream f(argv[1]);
	if (!f)
	{
		fmt::print(stderr, "Cannot open {}\n", argv[1]);
		return 1;
	}

	unordered_set<int> numbers;
	int number;
	while (f >> number)
	{
		numbers.emplace(number);
	}
	f.close();

	fmt::print("Part1: {}\n", part1(numbers));
	fmt::print("Part2: {}\n", part2(numbers));
	return 0;
}
