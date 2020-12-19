#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

struct Symbol
{
	bool terminal;
	int value;
};

using Production = vector<Symbol>;

class Grammar
{
public:
	Grammar() : patched(false), pos(0) {}

	bool is_valid(const string& txt, bool patched)
	{
		Grammar::patched = patched;
		pos = 0;
		return match(txt, 0) && pos == txt.size();
	}

	bool match(const string& txt, int pid)
	{
		if (pos >= txt.size())
			return false;

		size_t tmp = pos;
		for (auto& rule: productions[pid])
		{
			for (auto& sym: rule)
			{
				if (sym.terminal)
				{
					if (txt[pos] != sym.value) goto fail;
					pos++;
				}
				else if (patched && sym.value == 8)
				{
					if (!match(txt, 42)) goto fail;
				}
				else if (patched && sym.value == 11)
				{
					if (!match(txt, 42)) goto fail;
					int cnt = 0;
					while (match(txt, 42))
					{
						cnt++;
					}
					if (!match(txt, 31)) goto fail;
					while (match(txt, 31))
					{
						cnt--;
					}
					if (cnt < 0) goto fail;
				}
				else if (!match(txt, sym.value))
				{
					goto fail;
				}
			}
			return true;
		fail:
			// backtrack
			pos = tmp;
		}
		return false;
	}

private:
	unordered_map<int,vector<Production>> productions;
	bool patched;
	size_t pos;

	friend istream& operator>>(istream&, Grammar& g);
};

istream& operator>>(istream& input, Grammar& g)
{
	g.productions.clear();
	string line;
	while (getline(input, line) && line.size())
	{
		size_t pos = line.find(':');
		if (pos == string::npos)
		{
			continue;
		}

		int pid = stoi(line.substr(0, pos));
		pos++;
		size_t nextp;
		do
		{
			while (isspace(line[pos]))
				pos++;
			nextp = line.find('|', pos);

			Production p;
			size_t symstart = pos;
			size_t symnext;
			do
			{
				symnext = line.find(' ', symstart);
				if (symnext > nextp)
					break;

				Symbol r;
				if (isdigit(line[symstart]))
				{
					r.terminal = false;
					r.value = stoi(line.substr(symstart, symnext-symstart));
				}
				else
				{
					r.terminal = true;
					r.value = line[symstart+1];
				}
				p.emplace_back(move(r));
				symstart = symnext + 1;
			} while (symnext != string::npos);
			g.productions[pid].emplace_back(move(p));
			pos = nextp + 1;
		} while (nextp != string::npos);
	}
	if (g.productions.size())
	{
		input.clear();
	}
	return input;
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

	Grammar g;
	if (!(input >> g))
	{
		cerr << "Cannot parse the grammar rules" << endl;
		return 1;
	}

	size_t part1 = 0;
	size_t part2 = 0;
	string line;
	while (getline(input, line))
	{
		part1 += g.is_valid(line, false);
		part2 += g.is_valid(line, true);
	}
	input.close();

	cout << "Part1: " << part1 << endl
	     << "Part2: " << part2 << endl;

	return 0;
}
