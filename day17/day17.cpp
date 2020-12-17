#include <algorithm>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct vec
{
	int pos[4];

	bool eq(const vec& other, size_t dim) const
	{
		while (dim-->0)
		{
			if (pos[dim] != other.pos[dim])
				return false;
		}
		return true;
	}
};

static vec operator+(vec a, vec b)
{
	vec r;
	for (int i = 0; i < 4; i++)
	{
		r.pos[i] = a.pos[i] + b.pos[i];
	}
	return r;
}

static bool next(vec& v, const vec& start, const vec& end, size_t dim)
{
	for (size_t i = 0; i < dim; i++)
	{
		v.pos[i]++;
		if (v.pos[i] < end.pos[i])
		{
			return true;
		}
		v.pos[i] = start.pos[i];
	}
	return false;
}

class Hypercube
{
public:
	Hypercube(int cycles, int len, size_t dim)
		: corner(len + 2 * (cycles+1)),
		  offset(cycles+1),
		  dim(dim),
		  tsize(1),
		  table(nullptr),
		  minv{INT_MAX, INT_MAX, INT_MAX, INT_MAX},
		  maxv{INT_MIN, INT_MIN, INT_MIN, INT_MIN}
	{
		while (dim-->0)
		{
			tsize *= corner;
		}
		table = new char[tsize]();
	}

	~Hypercube()
	{
		delete[] table;
	}

	Hypercube(const Hypercube&) = delete;
	Hypercube& operator=(const Hypercube&) = delete;
	Hypercube(Hypercube&&)= delete;
	Hypercube& operator=(Hypercube&&) = delete;

	size_t count_active() const
	{
		return count(table, table + tsize, 1);
	}

	size_t index(const vec& v) const
	{
		size_t idx = 0;
		size_t i = dim;
		while (i-->0)
		{
			idx = idx * corner + offset + v.pos[i];
		}
		assert(idx < tsize);
		return idx;
	}

	bool get(const vec& pos) const
	{
		return table[index(pos)];
	}

	void set(const vec& v, bool value)
	{
		size_t idx = index(v);
		if (value)
		{
			table[idx] = 1;
			for (size_t i = 0; i < dim; i++)
			{
				if (minv.pos[i] > v.pos[i]) minv.pos[i] = v.pos[i];
				if (maxv.pos[i] < v.pos[i]) maxv.pos[i] = v.pos[i];
			}
		}
		else
		{
			table[idx] = 0;
		}
	}

	size_t adjacent(const vec& pos) const
	{
		vec start = pos + vec{-1,-1,-1,-1};
		vec end = pos + vec{2, 2, 2, 2 };
		vec cur = start;
		size_t count = 0;
		do {
			if (!cur.eq(pos, dim))
			{
				count += get(cur);
			}
		} while (next(cur, start, end, dim));
		return count;
	}

	void cycle_from(const Hypercube& src)
	{
		minv = vec{INT_MAX, INT_MAX, INT_MAX, INT_MAX};
		maxv = vec{INT_MIN, INT_MIN, INT_MIN, INT_MIN};

		vec start = src.minv + vec{-1, -1, -1, -1};
		vec end = src.maxv + vec{2, 2, 2, 2};
		vec cur = start;
		do
		{
			size_t adj = src.adjacent(cur);
			if (src.get(cur))
			{
				set(cur, 2 <= adj && adj <= 3);
			}
			else
			{
				set(cur, adj == 3);
			}
		} while (next(cur, start, end, dim));
	}

private:
	int corner;
	int offset;
	size_t dim;
	size_t tsize;
	char *table;
	vec minv;
	vec maxv;

	friend ostream& operator<<(ostream& out, const Hypercube &c);
};

ostream& operator<<(ostream& out, const Hypercube &c)
{
	vec start = c.minv;
	vec end = c.maxv + vec{1,1,1};
	vec cur = start;
	do
	{
		if (c.dim == 3 && cur.pos[0] == start.pos[0] && cur.pos[1] == start.pos[1])
		{
			out << "z = " << cur.pos[2] << endl;
		}
		cout << (c.get(cur) ? '#' : '.');
		if (cur.pos[0] == end.pos[0]-1)
		{
			cout << endl;
		}
	} while (next(cur, start, end, c.dim));
	return out;
}

static int conway_cube(const vector<string>& lines, size_t cycles, size_t dim)
{
	Hypercube a(cycles, lines.size(), dim);
	Hypercube b(cycles, lines.size(), dim);
	Hypercube *cur = &a;
	Hypercube *tmp = &b;

	for (size_t y = 0; y < lines.size(); y++)
	{
		for (size_t x = 0; x < lines[y].size(); x++)
		{
			cur->set(vec{(int)x, (int)y, 0, 0}, lines[y][x] == '#');
		}
	}

	for (int i = 0; i < 6; i++)
	{
		tmp->cycle_from(*cur);
		swap(cur, tmp);
	}

	return cur->count_active();
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

	vector<string> lines;
	string line;
	while (getline(input, line))
	{
		lines.emplace_back(move(line));
	}
	input.close();
	size_t len = lines.size();
	if (!len)
	{
		cerr << "Cannot parse " << argv[1] << endl;
		return 1;
	}

	cout << "Part1: " << conway_cube(lines, 6, 3) << endl
	     << "Part2: " << conway_cube(lines, 6, 4) << endl;

	return 0;
}
