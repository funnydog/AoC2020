#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return 1;
	}

	ifstream f(argv[1]);
	if (!f)
	{
		cerr << "Cannot open " << argv[1] << endl;
		return 1;
	}

	unordered_set<int> numbers;
	int number;
	while (f >> number)
	{
		numbers.emplace(number);
	}
	f.close();

	auto it = find_if(
		numbers.cbegin(),
		numbers.cend(),
		[&numbers](auto value) -> bool {
			return numbers.find(2020-value) != numbers.end();
		});
	if (it != numbers.cend())
	{
		cout << "Part1: " << *it * (2020 - *it) << endl;
	}

	for (auto l1 = numbers.cbegin(); l1 != numbers.cend(); ++l1)
	{
		for (auto l2 = next(l1); l2 != numbers.cend(); ++l2)
		{
			if (numbers.find(2020 - *l1 - *l2) != numbers.end())
			{
				cout << "Part2: " << *l1 * *l2 * (2020 - *l1 - *l2) << endl;
				goto out;
			}
		}
	}
out:

	return 0;
}
