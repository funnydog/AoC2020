#include <fstream>
#include <regex>
#include <fmt/format.h>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fmt::print(stderr, "Usage: {} <filename>\n", argv[0]);
		return 1;
	}

	ifstream file(argv[1]);
	if (!file)
	{
		fmt::print(stderr, "Cannot open {}\n", argv[1]);
		return 1;
	}

	regex pattern(R"(^(\d+)-(\d+) (\S): (.*)$)");
	string line;
	size_t valid1 = 0;
	size_t valid2 = 0;
	while (getline(file, line))
	{
		smatch sm;
		if (regex_match(line, sm, pattern))
		{
			size_t lowest = stoi(sm[1]);
			size_t highest = stoi(sm[2]);
			char letter = sm[3].str()[0];
			string password = sm[4];
			size_t lcount = count(
				password.cbegin(),
				password.cend(),
				letter);
			valid1 += lowest <= lcount && lcount <= highest;
			valid2 += (password[lowest-1] == letter)
				^ (password[highest-1] == letter);
		}
	}
	file.close();

	fmt::print("Part1: {}\nPart2: {}\n", valid1, valid2);
	return 0;
}
