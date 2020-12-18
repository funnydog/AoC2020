#include <cctype>
#include <fstream>
#include <iostream>

using namespace std;

enum class TokenType
{
	NUM,
	ADD,
	MUL,
	LPAR,
	RPAR,
	END,
};

struct Token
{
	TokenType type;
	string value;
};

// expression = factor {"+" factor | "*" factor}*
// factor = number | "(" expression ")"
class Grammar1
{
public:
	Grammar1() : pos(0), cur{TokenType::END, ""} {}

	intmax_t eval(const string& txt)
	{
		pos = 0;
		scan_next(txt);
		return expression(txt);
	}

protected:
	void scan_next(const string& txt)
	{
		while (pos < txt.size() && isspace(txt[pos]))
		{
			pos++;
		}
		if (pos == txt.size())
		{
			cur.type = TokenType::END;
		}
		else if (isdigit(txt[pos]))
		{
			size_t start = pos;
			while (isdigit(txt[pos]))
			{
				pos++;
			}
			cur.type = TokenType::NUM;
			cur.value = txt.substr(start, pos-start);
		}
		else if (txt[pos] == '+')
		{
			cur.type = TokenType::ADD;
			pos++;
		}
		else if (txt[pos] == '*')
		{
			cur.type = TokenType::MUL;
			pos++;
		}
		else if (txt[pos] == '(')
		{
			cur.type = TokenType::LPAR;
			pos++;
		}
		else if (txt[pos] == ')')
		{
			cur.type = TokenType::RPAR;
			pos++;
		}
		else
		{
			cerr << "Unknown character: " << txt[pos] << endl;
			exit(1);
		}
	}

	// factor = number | "(" expression ")"
	intmax_t factor(const string& txt)
	{
		intmax_t v;
		if (cur.type == TokenType::NUM)
		{
			v = stoll(cur.value);
			scan_next(txt);
		}
		else if (cur.type == TokenType::LPAR)
		{
			scan_next(txt);
			v = expression(txt);
			if (cur.type != TokenType::RPAR)
			{
				cerr << "Error: found " << static_cast<int>(cur.type)
				     << " expected " << static_cast<int>(TokenType::RPAR)
				     << endl;
				exit(1);
			}
			scan_next(txt);
		}
		else
		{
			cerr << "Error: found " << static_cast<int>(cur.type)
			     << " expected " << static_cast<int>(TokenType::NUM)
			     << " or " << static_cast<int>(TokenType::LPAR)
			     << endl;
			exit(1);
		}
		return v;
	}

	// expression = factor {"+" factor | "*" factor}*
	virtual intmax_t expression(const string& txt)
	{
		intmax_t v = factor(txt);
		while (true)
		{
			if (cur.type == TokenType::ADD)
			{
				scan_next(txt);
				v += factor(txt);
			}
			else if (cur.type == TokenType::MUL)
			{
				scan_next(txt);
				v *= factor(txt);
			}
			else
			{
				break;
			}
		}
		return v;
	}

	size_t pos;
	Token cur;
};

// expression = term {"*" term}*
// term = factor {"+" factor}*
// factor = number | "(" expression ")"
class Grammar2: public Grammar1
{
protected:
	// term = factor {"+" factor}*
	intmax_t term(const string& txt)
	{
		intmax_t v = factor(txt);
		while (cur.type == TokenType::ADD)
		{
			scan_next(txt);
			v += factor(txt);
		}
		return v;
	}

	// expression = term {"*" term}*
	intmax_t expression(const string& txt) override
	{
		intmax_t v = term(txt);
		while (cur.type == TokenType::MUL)
		{
			scan_next(txt);
			v *= term(txt);
		}
		return v;
	}
};

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

	intmax_t part1 = 0;
	intmax_t part2 = 0;
	Grammar1 g1;
	Grammar2 g2;
	string line;
	while (getline(input, line))
	{
		part1 += g1.eval(line);
		part2 += g2.eval(line);
	}
	input.close();

	cout << "Part1: " << part1 << endl
	     << "Part2: " << part2 << endl;
	return 0;
}
