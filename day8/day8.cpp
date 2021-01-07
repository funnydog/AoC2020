#include <fstream>
#include <regex>
#include <vector>
#include <fmt/format.h>

using namespace std;

class System
{
public:
	enum class OpCode
	{
		ACC,
		JMP,
		NOP,
	};

	int run()
	{
		ip = 0;
		acc = 0;
		fill(visited.begin(), visited.end(), false);
		while (ip < instructions.size() && !visited[ip])
		{
			visited[ip] = true;
			auto [op, offset] = instructions[ip];
			switch(op)
			{
			case OpCode::ACC:
				acc += offset;
				ip++;
				break;

			case OpCode::JMP:
				ip += offset;
				break;

			case OpCode::NOP:
				ip++;
				break;

			default:
				break;
			}
		}
		return acc;
	}

	int analyze()
	{
		for (auto& instr: instructions)
		{
			switch(instr.first)
			{
			case OpCode::JMP:
				instr.first = OpCode::NOP;
				run();
				instr.first = OpCode::JMP;
				break;
			case OpCode::NOP:
				instr.first = OpCode::JMP;
				run();
				instr.first = OpCode::NOP;
				break;
			default:
				continue;
			}
			if (ip >= instructions.size())
			{
				return acc;
			}
		}
		return 0;
	}

private:
	vector<pair<OpCode, int>> instructions;
	vector<bool> visited;
	size_t ip = 0;
	int acc = 0;

	friend istream& operator>>(istream&, System&);
};

istream& operator>>(istream& input, System& sys)
{
	const regex pat("(acc|jmp|nop) ([+-]?\\d+)");

	sys.instructions.clear();
	string line;
	while (getline(input, line))
	{
		smatch sm;
		if (regex_match(line, sm, pat))
		{
			System::OpCode op;
			if (sm[1] == "acc")
			{
				op = System::OpCode::ACC;
			}
			else if (sm[1] == "jmp")
			{
				op = System::OpCode::JMP;
			}
			else if (sm[1] == "nop")
			{
				op = System::OpCode::NOP;
			}
			else
			{
				continue;
			}
			sys.instructions.emplace_back(make_pair(op, stoi(sm[2])));
		}
	}
	sys.visited.resize(sys.instructions.size());
	if (sys.instructions.size())
	{
		input.clear();
	}
	return input;
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

	System s;
	input >> s;
	input.close();
	if (!input)
	{
		fmt::print(stderr, "Cannot parse the data\n");
		return 1;
	}

	fmt::print("Part1: {}\n", s.run());
	fmt::print("Part2: {}\n", s.analyze());
	return 0;
}
