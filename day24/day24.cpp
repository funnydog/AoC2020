#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_set>

using namespace std;

struct Vec
{
	int q;
	int r;

	Vec operator+(const Vec& other) const noexcept
	{
		return Vec{q+other.q, r+other.r};
	}

	Vec& operator+=(const Vec& other) noexcept
	{
		q += other.q;
		r += other.r;
		return *this;
	}

	bool operator==(const Vec& other) const noexcept
	{
		return q == other.q && r == other.r;
	}
};

struct HashVec
{
	size_t operator()(const Vec& v) const noexcept
	{
		return ((v.q<<24)^(v.r));
	}
};

// Mapping of the hexagonal coordinates
// to the cartesian coordinates
//
//      (0, -1) [NW]  [NE] (1,-1)
//               |   /
//               |  /
// (-1,0) [ W]--[O ]--[ E] (1,0)
//             / |
//            /  |
// (-1,1) [SW]  [SE] (0, 1)
//
static const struct {
	string desc;
	Vec dir;
} DIRECTIONS[] = {
	{"e",  Vec{1, 0}},	// EAST
	{"se", Vec{0, 1}},	// SOUTH-EAST
	{"sw", Vec{-1, 1}},	// SOUTH-WEST
	{"w",  Vec{-1, 0}},	// WEST
	{"nw", Vec{0, -1}},	// NORTH-WEST
	{"ne", Vec{1, -1}},	// NORTH-EAST
};

class Floor
{
public:
	size_t count_black_tiles() const noexcept
	{
		return tiles.size();
	}

	void simulate(size_t days) noexcept
	{
		// find the area of the simulation
		Vec min{INT_MAX, INT_MAX};
		Vec max{INT_MIN, INT_MIN};
		for (const auto& t: tiles)
		{
			if (min.q > t.q) min.q = t.q;
			if (max.q < t.q) max.q = t.q;
			if (min.r > t.r) min.r = t.r;
			if (max.r < t.r) max.r = t.r;
		}
		min += Vec{-1,-1};
		max += Vec{1, 1};

		unordered_set<Vec, HashVec> other;
		while (days-->0)
		{
			// NOTE: perform a simulation step
			min += Vec{-1,-1};
			max += Vec{1, 1};

			Vec nmin{INT_MAX, INT_MAX};
			Vec nmax{INT_MIN, INT_MIN};
			other.clear();
			for (int q = min.q; q <= max.q; q++)
			{
				for (int r = min.r; r <= max.r; r++)
				{
					Vec pos{q, r};

					// count neighbours
					size_t neigh = count_if(
						DIRECTIONS,
						DIRECTIONS + 6,
						[this,pos](const auto& p) {
							return tiles.find(pos+p.dir) != tiles.end();
						});

					// apply the rules to the current tile
					auto black = tiles.find(pos) != tiles.end();
					if (black && (neigh == 0 || neigh > 2))
					{
						other.erase(pos);
					}
					else if ((!black && neigh == 2) || black)
					{
						other.insert(pos);
						if (nmin.q > pos.q) nmin.q = pos.q;
						if (nmax.q < pos.q) nmax.q = pos.q;
						if (nmin.r > pos.r) nmin.r = pos.r;
						if (nmax.r < pos.r) nmax.r = pos.r;
					}
				}
			}
			swap(tiles, other);
			swap(min, nmin);
			swap(max, nmax);
		}
	}

private:
	unordered_set<Vec, HashVec> tiles;
	friend istream& operator>>(istream& in, Floor& f);
};

istream& operator>>(istream& in, Floor& f)
{
	f.tiles.clear();
	string line;
	while (getline(in, line))
	{
		if (!line.size())
		{
			continue;
		}

		// find the coordinates of the tile
		size_t spos = 0;
		Vec pos{0, 0};
		bool found = false;
		do
		{
			found = false;
			for (auto& d: DIRECTIONS)
			{
				if (line.rfind(d.desc, spos) == spos)
				{
					pos += d.dir;
					spos += d.desc.size();
					found = true;
					break;
				}
			}
		} while(found);

		// flip the tile at pos
		auto it = f.tiles.find(pos);
		if (it != f.tiles.end())
		{
			f.tiles.erase(it);
		}
		else
		{
			f.tiles.insert(pos);
		}
	}
	if (f.tiles.size())
	{
		in.clear();
	}
	return in;
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
	Floor f;
	if (!(input >> f))
	{
		cerr << "Cannot parse the instructions." << endl;
		input.close();
		return 1;
	}
	input.close();

	cout << "Part1: " << f.count_black_tiles() << endl;
	f.simulate(100);
	cout << "Part2: " << f.count_black_tiles() << endl;
	return 0;
}
