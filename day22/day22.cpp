#include <fstream>
#include <iostream>
#include <deque>
#include <set>

using namespace std;

struct Deck
{
	deque<size_t> cards;

	Deck() {}

	Deck(deque<size_t>::const_iterator begin, deque<size_t>::const_iterator end)
		: cards(begin, end)
	{
	}

	bool empty() const
	{
		return cards.empty();
	}

	Deck subdeck() const
	{
		return Deck(cards.begin()+1, cards.begin()+1+cards[0]);
	}

	bool has_subdeck() const
	{
		return cards[0] < cards.size();
	}

	size_t front() const
	{
		return cards.front();
	}

	size_t pop()
	{
		auto v = cards.front();
		cards.pop_front();
		return v;
	}

	void push(size_t v)
	{
		cards.push_back(v);
	}

	size_t get_score() const
	{
		size_t score = 0;
		size_t i = 1;
		for (auto it = cards.rbegin(); it != cards.rend(); ++it, ++i)
		{
			score += *it * i;
		}
		return score;
	}

	// for the set
	bool operator<(const Deck& other) const
	{
		if (cards.size() != other.cards.size())
		{
			return cards.size() < other.cards.size();
		}

		for (auto c1 = cards.begin(), c2 = other.cards.begin();
		     c1 != cards.end();
		     ++c1, ++c2)
		{
			if (*c1 != *c2)
			{
				return *c1 < *c2;
			}
		}
		return false;
	}
};

pair<Deck,Deck> load(istream& input)
{
	Deck p1, p2;
	Deck *cur = &p1;
	string line;
	while (getline(input, line))
	{
		if (line.empty())
		{
			cur = &p2;
		}
		else if (line.rfind("Player", 0) == 0)
		{
			// nothing
		}
		else
		{
			cur->cards.push_back(stoi(line));
		}
	}
	return make_pair(move(p1), move(p2));
}

bool game(Deck& p1, Deck& p2, bool r);

bool first_wins(const Deck& p1, const Deck& p2, bool r)
{
	if (!r)
	{
		return p1.front() > p2.front();
	}
	else if (!p1.has_subdeck() || !p2.has_subdeck())
	{
		return p1.front() > p2.front();
	}
	else
	{
		Deck s1 = p1.subdeck();
		Deck s2 = p2.subdeck();
		return game(s1, s2, r);
	}
}

bool game(Deck& p1, Deck& p2, bool recursive)
{
	set<Deck> hp1;
	while (!p1.empty() && !p2.empty())
	{
		if (recursive)
		{
			if (hp1.find(p1) != hp1.end())
			{
				return true;
			}
			hp1.insert(p1);
		}

		if (first_wins(p1, p2, recursive))
		{
			p1.push(p1.pop());
			p1.push(p2.pop());
		}
		else
		{
			p2.push(p2.pop());
			p2.push(p1.pop());
		}
	}
	return !p1.empty();
}

int get_score(const Deck& p1, const Deck& p2, bool r)
{
	Deck a = p1;
	Deck b = p2;
	if (game(a, b, r))
	{
		return a.get_score();
	}
	else
	{
		return b.get_score();
	}
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
	auto p = load(input);
	input.close();

	cout << "Part1: " << get_score(p.first, p.second, false) << endl
	     << "Part2: " << get_score(p.first, p.second, true) << endl;

	return 0;
}
