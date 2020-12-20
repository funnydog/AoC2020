#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <deque>
#include <vector>
#include <cstring>
#include <cassert>

using namespace std;

// directions
enum
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
	FLIP,
};

struct Image
{
	explicit Image(size_t width, size_t height)
		: width(width),
		  height(height)
	{
		for (size_t y = 0; y < height; y++)
		{
			data.emplace_back(string(width, ' '));
		}
	}

	explicit Image(const vector<string>& rows)
		: data(rows),
		  width(rows[0].size()),
		  height(rows.size())
	{
	}

	Image(Image&& other) noexcept
		: data(move(other.data)),
		  width(other.width),
		  height(other.height)
	{
		for (size_t y = 0; y < height; y++)
		{
			other.data.emplace_back(string(width, ' '));
		}
	}

	Image& operator=(Image&& other) noexcept
	{
		if (this != &other)
		{
			swap(data, other.data);
			swap(width, other.width);
			swap(height, other.height);
		}
		return *this;
	}

	virtual ~Image() {}

	virtual void flip()
	{
		for (size_t y = 0; y < height; y++)
		{
			string tmp(data[y]);
			for (size_t x = 0; x < width; x++)
			{
				data[y][width-x-1] = tmp[x];
			}
		}
	}

	virtual void rotate()
	{
		vector<string> n;
		for (size_t x = 0; x < width; x++)
		{
			string tmp(height, ' ');
			for (size_t y = 0; y < height; y++)
			{
				tmp[y] = data[y][width-x-1];
			}
			n.emplace_back(move(tmp));
		}
		data = move(n);
		swap(width, height);
	}

	char get(size_t x, size_t y) const
	{
		assert(x < width && y < height);
		return data[y][x];
	}

	void set(size_t x, size_t y, char v)
	{
		assert(x < width && y < height);
		data[y][x] = v;
	}

	size_t count_pixels(char c) const
	{
		return accumulate(
			data.begin(),
			data.end(),
			0ULL,
			[c](const auto& acc, const string& row){
				return acc + count(row.begin(), row.end(), c);
			});
	}

	vector<string> data;
	size_t width;
	size_t height;
};

ostream& operator<<(ostream& out, const Image& img)
{
	for (auto&row: img.data)
	{
		out << row << endl;
	}
	return out;
}

struct Tile: public Image
{
	int id;
	int adj[4];
	int acnt;
	bool visited;
	int px;
	int py;
	unsigned borders[8];

	explicit Tile() : Image(10, 10),
			  id(0),
			  adj{},
			  acnt(0),
			  visited(false),
			  px(0),
			  py(0),
			  borders{}
	{}

	virtual ~Tile() {}

	Tile(Tile&&) = default;
	Tile& operator=(Tile&&) = default;

	void compute_borders()
	{
		for (size_t i = 0; i < 8; i++)
		{
			borders[i] = 0;
		}
		for (size_t i = 0; i < 10; i++)
		{
			borders[UP   ] |= data[0][i] == '#' ? (1U<<i) : 0;
			borders[RIGHT] |= data[i][9] == '#' ? (1U<<i) : 0;
			borders[DOWN ] |= data[9][i] == '#' ? (1U<<i) : 0;
			borders[LEFT ] |= data[i][0] == '#' ? (1U<<i) : 0;
			borders[FLIP + UP   ] |= data[0][9-i] == '#' ? (1U<<i) : 0;
			borders[FLIP + RIGHT] |= data[9-i][9] == '#' ? (1U<<i) : 0;
			borders[FLIP + DOWN ] |= data[9][9-i] == '#' ? (1U<<i) : 0;
			borders[FLIP + LEFT ] |= data[9-i][0] == '#' ? (1U<<i) : 0;
		}
	}

	void flip() override
	{
		Image::flip();
		swap(borders[LEFT], borders[RIGHT]);
		swap(borders[UP], borders[FLIP+UP]);
		swap(borders[DOWN], borders[FLIP+DOWN]);
	}

	void rotate() override
	{
		Image::rotate();
		compute_borders();
	}

	bool shares_border(const Tile& other)
	{
		for (auto ba: borders)
		{
			if (any_of(other.borders, other.borders + 4,
				   [ba](auto bb) {
					   return bb == ba;
				   }))
			{
				return true;
			}
		}
		return false;
	}

	int align_to(const Tile& other)
	{
		// align this tile to the other tile
		for (int i = 0; i < 8; i++)
		{
			rotate();
			if (i == FLIP) flip();
			for (int j = 0; j < 4; j++)
			{
				// check if the opposite borders match
				if (borders[(j+2)%4] == other.borders[j])
				{
					return j;
				}
			}
		}
		return -1;
	}
};

ostream& operator<<(ostream& out, const Tile& t)
{
	out << t.id << endl << static_cast<const Image&>(t) << endl;
	return out;
}

istream& operator>>(istream& input, Tile& t)
{
	string line;
	int y = 0;
	t.id = 0;
	while(getline(input, line))
	{
		if (line.rfind("Tile ", 0) == 0)
		{
			line.pop_back();
			t.id = stoi(line.substr(5));
		}
		else if (line.size() == 0)
		{
			break;
		}
		else if (line.size() >= 10 && y < 10)
		{
			for (size_t x = 0; x < 10; x++)
			{
				t.set(y, x, line[x]);
			}
			y++;
		}
	}
	if (y == 10)
	{
		input.clear();
	}
	return input;
}

static void find_adjacent(vector<Tile>& tiles)
{
	// for each tile find the adjacent tiles
	for (size_t i = 0; i < tiles.size(); i++)
	{
		tiles[i].compute_borders();
		for (size_t j = 0; j < i; j++)
		{
			if (tiles[i].shares_border(tiles[j]))
			{
				tiles[i].adj[tiles[i].acnt++] = j;
				tiles[j].adj[tiles[j].acnt++] = i;
			}
		}
	}
}

static size_t part1(const vector<Tile>& tiles)
{
	return accumulate(
		tiles.begin(),
		tiles.end(),
		1ULL,
		[](auto acc, const Tile& t) {
			return (t.acnt == 2) ? (acc * t.id) : (acc);
		});
}

static void map_dfs(vector<Tile>& tiles, size_t i)
{
	static const int dx[] = {0,1,0,-1};
	static const int dy[] = {-1,0,1,0};

	auto& a = tiles[i];
	a.visited = true;
	for (auto j: a.adj)
	{
		auto& b = tiles[j];
		if (!b.visited)
		{
			int d = b.align_to(tiles[i]);
			b.px = a.px + dx[d];
			b.py = a.py + dy[d];
			map_dfs(tiles, j);
		}
	}
}

static size_t part2(vector<Tile>& tiles)
{
	// explore the map starting from an arbitrary tile
	map_dfs(tiles, 0);

	// find the size of the map
	int xmin, ymin, xmax, ymax;
	xmin = ymin = xmax = ymax = 0;
	for (auto& t: tiles)
	{
		assert(t.visited);
		if (xmin > t.px) xmin = t.px;
		if (xmax < t.px) xmax = t.px;
		if (ymin > t.py) ymin = t.py;
		if (ymax < t.py) ymax = t.py;
	}
	int width = xmax - xmin + 1;
	int height = ymax - ymin + 1;

	// create the Image of the map from the tiles
	Image map(width*8, height*8);
	for (auto& t: tiles)
	{
		int sx = (t.px - xmin) * 8;
		int sy = (t.py - ymin) * 8;
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				map.set(sx+x, sy+y, t.get(x+1, y+1));
			}
		}
	}

	// mark the sea monsters in the map
	Image monster({
			"                  # ",
			"#    ##    ##    ###",
			" #  #  #  #  #  #   ",
		});

	for (int i = 0; i < 8; i++)
	{
		monster.rotate();
		if (i == FLIP) monster.flip();
		for (size_t y = 0; y < map.height - monster.height; y++)
		{
			for (size_t x = 0; x < map.width - monster.width; x++)
			{
				bool found = true;
				for (size_t my = 0; found && my < monster.height; my++)
				{
					for (size_t mx = 0; mx < monster.width; mx++)
					{
						if (monster.get(mx, my) == '#' &&
						    map.get(x+mx, y+my) != '#')
						{
							found = false;
							break;
						}
					}
				}
				if (found)
				{
					for (size_t my = 0; my < monster.height; my++)
					{
						for (size_t mx = 0; mx < monster.width; mx++)
						{
							if (monster.get(mx, my) == '#')
							{
								map.set(x+mx, y+my, 'O');
							}
						}
					}
				}
			}
		}
	}

	return map.count_pixels('#');
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

	vector<Tile> tv;
	Tile t;
	while (input>>t)
	{
		tv.emplace_back(move(t));
	}
	input.close();

	find_adjacent(tv);
	cout << "Part1: " << part1(tv) << endl
	     << "Part2: " << part2(tv) << endl;
	return 0;
}
