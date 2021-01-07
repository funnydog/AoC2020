#include <fstream>
#include <regex>
#include <unordered_map>
#include <vector>
#include <fmt/format.h>

using namespace std;

struct Rule
{
	int a;
	int b;
	int c;
	int d;

	bool is_valid(int v) const noexcept
	{
		return (a <= v && v <= b) || (c <= v && v <= d);
	}
};

struct Ticket
{
	vector<int> values;

	Ticket() = default;

	explicit Ticket(string& str)
	{
		size_t pos = 0;
		size_t next;
		while ((next = str.find(',', pos)) != string::npos)
		{
			values.push_back(stoi(str.substr(pos, next-pos)));
			pos = next + 1;
		}
		values.push_back(stoi(str.substr(pos)));
	}

	bool is_valid(const vector<Rule>& rules) const
	{
		for (auto v: values)
		{
			if (!any_of(rules.begin(), rules.end(), [v](const Rule &r){
				return r.is_valid(v);
			}))
			{
				return false;
			}
		}
		return true;
	}
};

struct Problem
{
	vector<string> names;
	vector<Rule> rules;
	vector<Ticket> nearby;
	Ticket my_ticket;
};

struct Constraint
{
	size_t col;
	vector<size_t> rules;
};

istream& operator>>(istream& input, Problem& p)
{
	const regex RULE("^(.*): (\\d+)-(\\d+) or (\\d+)-(\\d+)$");

	p.names.clear();
	p.rules.clear();
	p.nearby.clear();
	string line;
	enum { RULES, TICKET, NEARBY } state = RULES;
	while (getline(input, line))
	{
		if (state == RULES)
		{
			if (line == "your ticket:")
			{
				state = TICKET;
				continue;
			}

			smatch sm;
			if (regex_match(line, sm, RULE))
			{
				p.names.emplace_back(sm[1]);
				p.rules.emplace_back(Rule{
						stoi(sm[2]),
						stoi(sm[3]),
						stoi(sm[4]),
						stoi(sm[5]),
					});
			}
		}
		else if (state == TICKET)
		{
			if (line == "nearby tickets:")
			{
				state = NEARBY;
			}
			else if (line.size())
			{
				p.my_ticket = Ticket(line);
			}
		}
		else if (state == NEARBY)
		{
			if (line.size())
			{
				p.nearby.emplace_back(Ticket(line));
			}
		}
		else
		{
			abort();
		}
	}
	if (p.rules.size())
	{
		input.clear();
	}
	return input;
}

static size_t part1(const Problem& p)
{
	size_t error = 0;
	for (auto& t: p.nearby)
	{
		for (auto v: t.values)
		{
			if (!any_of(p.rules.begin(), p.rules.end(), [v](const Rule& r) {
				return r.is_valid(v);
			}))
			{
				error += v;
			}
		}
	}
	return error;
}

static vector<Constraint> constraints(const Problem& p)
{
	vector<Constraint> c;

	for (size_t col = 0; col < p.my_ticket.values.size(); col++)
	{
		vector<size_t> columns;
		for (size_t j = 0; j < p.rules.size(); j++)
		{
			auto& rule = p.rules[j];
			if (all_of(p.nearby.begin(), p.nearby.end(),
				   [&rule, col](const Ticket& t) {
					   return rule.is_valid(t.values[col]);
				   }))
			{
				columns.push_back(j);
			}
		}
		c.emplace_back(Constraint{col, move(columns)});
	}
	return c;
}

static size_t part2(Problem& p)
{
	// filter out the wrong nearby data
	p.nearby.erase(
		remove_if(p.nearby.begin(), p.nearby.end(), [&p](const Ticket& t) {
			return !t.is_valid(p.rules);
		}),
		p.nearby.end());

	// get the constraints
	vector<Constraint> con = constraints(p);

	// constraint elimination
	vector<size_t> result(p.rules.size());
	while (con.size())
	{
		auto it = min_element(con.begin(), con.end(), [](const Constraint& a, const Constraint& b) {
			return a.rules.size() < b.rules.size();
		});
		size_t rule = it->rules[0];
		result[rule] = it->col;
		con.erase(it);
		for (auto& c: con)
		{
			c.rules.erase(
				remove(c.rules.begin(), c.rules.end(), rule),
				c.rules.end()
				);
		}

	}

	// multiply the fields starting with departure
	size_t m = 1;
	for (size_t i = 0; i < p.names.size(); i++)
	{
		if (p.names[i].rfind("departure", 0) == 0)
		{
			m *= p.my_ticket.values[result[i]];
		}
	}
	return m;
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

	Problem p;
	input >> p;
	input.close();
	if (input.fail())
	{
		fmt::print(stderr, "Cannot parse the data\n");
		return 1;
	}

	fmt::print("Part1: {}\n", part1(p));
	fmt::print("Part2: {}\n", part2(p));
	return 0;
}
