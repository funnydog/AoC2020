#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>

using namespace std;

struct rule
{
	int a;
	int b;
	int c;
	int d;

	int check_value(int v) const
	{
		return (a <= v && v <= b) || (c <= v && v <= d);
	}
};

struct ticket
{
	vector<int> values;

	ticket() = default;

	explicit ticket(string& str)
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

	bool is_valid(const vector<rule>& rules) const
	{
		for (auto v: values)
		{
			if (!any_of(rules.begin(), rules.end(), [&v](auto &r){
				return r.check_value(v);
			}))
			{
				return false;
			}
		}
		return true;
	}
};

struct problem
{
	vector<string> names;
	vector<rule> rules;
	vector<ticket> nearby;
	ticket my_ticket;
};

struct constraint
{
	size_t col;
	vector<size_t> rules;
};

istream& operator>>(istream& input, problem& p)
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
			if (line == "your ticket:"s)
			{
				state = TICKET;
				continue;
			}

			smatch sm;
			if (regex_match(line, sm, RULE))
			{
				p.names.emplace_back(sm[1]);
				p.rules.emplace_back(rule{
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
				p.my_ticket = ticket(line);
			}
		}
		else if (state == NEARBY)
		{
			if (line.size())
			{
				p.nearby.emplace_back(ticket(line));
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

static size_t part1(const problem& p)
{
	size_t error = 0;
	for (auto& t: p.nearby)
	{
		for (auto v: t.values)
		{
			if (!any_of(p.rules.begin(), p.rules.end(), [&v](auto& r) {
				return r.check_value(v);
			}))
			{
				error += v;
			}
		}
	}
	return error;
}

static vector<constraint> constraints(const problem& p)
{
	vector<constraint> c;

	for (size_t col = 0; col < p.my_ticket.values.size(); col++)
	{
		vector<size_t> columns;
		for (size_t j = 0; j < p.rules.size(); j++)
		{
			auto& rule = p.rules[j];
			if (all_of(p.nearby.begin(), p.nearby.end(),
				   [&rule, col](const auto& t) {
					   return rule.check_value(t.values[col]);
				   }))
			{
				columns.push_back(j);
			}
		}
		c.emplace_back(constraint{col, move(columns)});
	}
	return c;
}

static size_t part2(problem& p)
{
	// filter out the wrong nearby data
	p.nearby.erase(
		remove_if(p.nearby.begin(), p.nearby.end(), [&p](auto &t) {
			return !t.is_valid(p.rules);
		}),
		p.nearby.end());

	// get the constraints
	vector<constraint> con = constraints(p);

	// constraint elimination
	vector<size_t> result(p.rules.size());
	while (con.size())
	{
		auto it = min_element(con.begin(), con.end(), [](auto& a, auto& b) {
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
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return 1;
	}

	ifstream input(argv[1]);
	if (!input)
	{
		cerr << "Cannot open " << argv[1] << endl;
		return 1;
	}

	problem p;
	input >> p;
	input.close();
	if (input.fail())
	{
		cerr << "Cannot parse " << argv[1] << endl;
		return 1;
	}

	cout << "Part1: " << part1(p) << endl
	     << "Part2: " << part2(p) << endl;

	return 0;
}
