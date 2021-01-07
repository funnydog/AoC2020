#include <fstream>
#include <functional>
#include <regex>
#include <unordered_map>
#include <vector>
#include <fmt/format.h>

using namespace std;

using Validator = pair<string, function<bool(const string&)>>;

const vector<Validator> VPART1 = {
	{ "byr", [](const string&){ return true; } },
	{ "iyr", [](const string&){ return true; } },
	{ "eyr", [](const string&){ return true; } },
	{ "hgt", [](const string&){ return true; } },
	{ "hcl", [](const string&){ return true; } },
	{ "ecl", [](const string&){ return true; } },
	{ "pid", [](const string&){ return true; } },
};

const regex pass_pat(R"(^(.*):(.*)$)");
const regex year_pat(R"(^\d{4}$)");
const regex hgt_pat(R"(^(\d+)(in|cm)$)");
const regex hcl_pat(R"(^#[0-9a-f]{6}$)");
const regex ecl_pat(R"(^(amb|blu|brn|gry|grn|hzl|oth)$)");
const regex pid_pat(R"(^\d{9}$)");

const vector<Validator> VPART2 = {
	{
		"byr",
		[](const string& s) {
			if (!regex_match(s, year_pat))
			{
				return false;
			}
			int value = stoi(s);
			return 1920 <= value && value <= 2002;
		}
	},
	{
		"iyr",
		[](const string& s) {
			if (!regex_match(s, year_pat))
			{
				return false;
			}
			int value = stoi(s);
			return 2010 <= value && value <= 2020;
		}
	},
	{
		"eyr",
		[](const string& s) {
			if (!regex_match(s, year_pat))
			{
				return false;
			}
			int value = stoi(s);
			return 2020 <= value && value <= 2030;
		}
	},
	{
		"hgt",
		[](const string& s) {
			smatch sm;
			if (!regex_match(s, sm, hgt_pat))
			{
				return false;
			}
			int value = stoi(sm[1]);
			if (sm[2] == "cm")
			{
				return 150 <= value && value <= 193;
			}
			else
			{
				return 59 <= value && value <= 76;
			}
		}
	},
	{
		"hcl",
		[](const string& s) {
			return regex_match(s, hcl_pat);
		}
	},
	{
		"ecl",
		[](const string& s) {
			return regex_match(s, ecl_pat);
		}
	},
	{
		"pid",
		[](const string& s) {
			return regex_match(s, pid_pat);
		}
	},
};

struct Passport
{
	unordered_map<string,string> fields;

	void clear()
	{
		fields.clear();
	}

	void add_field(const string& record)
	{
		smatch sm;
		if (regex_match(record, sm, pass_pat))
		{
			fields[sm[1]] = sm[2];
		}
	}

	bool is_valid(const vector<Validator>& validators) const
	{
		for (auto& v: validators)
		{
			auto it = fields.find(v.first);
			if (!(it != fields.end() && v.second(it->second)))
			{
				return false;
			}
		}
		return true;
	}
};

istream& operator>>(istream& in, Passport& pass)
{
	bool read = false;
	pass.clear();
	string line;
	while (getline(in, line) && line.size() != 0)
	{
		size_t pos = 0;
		size_t next;
		while ((next = line.find(' ', pos)) != string::npos)
		{
			pass.add_field(line.substr(pos, next - pos));
			pos = next + 1;
		}
		pass.add_field(line.substr(pos));
		read = true;
	}
	// NOTE: hack to allow to read the last element without
	// discarding it because of the EOF signalling
	if (read) in.clear();
	return in;
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
		fmt::print(stderr, "Cannot open {}", argv[1]);
		return 1;
	}

	int part1 = 0;
	int part2 = 0;
	Passport pass;
	while (input >> pass)
	{
		part1 += pass.is_valid(VPART1);
		part2 += pass.is_valid(VPART2);
	}
	input.close();

	fmt::print("Part1: {}\nPart2: {}\n", part1, part2);
	return 0;
}
