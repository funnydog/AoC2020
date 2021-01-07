#include <fstream>
#include <regex>
#include <unordered_map>
#include <vector>
#include <fmt/format.h>

using namespace std;

struct Vertex
{
	vector<pair<int,int>> edges;
	bool visited;
	int count;
};

struct Graph
{
	unordered_map<string, int> lookup;
	vector<Vertex> vertices;
	vector<int> stack;

	Graph transpose() const
	{
		Graph newgraph;
		newgraph.vertices.resize(vertices.size());
		for (size_t v1 = 0; v1 < vertices.size(); v1++)
		{
			for (auto [v2, weight]: vertices[v1].edges)
			{
				newgraph.add_edge(v2, v1, weight);
			}
		}
		return newgraph;
	}

	void clear()
	{
		lookup.clear();
		vertices.clear();
	}

	int add_vertex(const string& name)
	{
		auto it = lookup.find(name);
		if (it != lookup.end())
		{
			return it->second;
		}

		int v = vertices.size();
		vertices.emplace_back(Vertex());
		lookup[name] = v;
		return v;
	}

	void add_edge(int v1, int v2, int weight)
	{
		vertices[v1].edges.emplace_back(make_pair(v2, weight));
	}

	void dfs_reset(void)
	{
		for (auto& v: vertices)
		{
			v.visited = false;
			v.count = 0;
		}
		stack.clear();
	}

	void dfs_visit(int node)
	{
		vertices[node].visited = true;
		for (auto [col, num]: vertices[node].edges)
		{
			if (!vertices[col].visited)
			{
				dfs_visit(col);
			}
		}
		stack.push_back(node);
	}

	int containing_colors(const string& node)
	{
		Graph newg = transpose();
		newg.dfs_reset();
		newg.dfs_visit(lookup[node]);
		return newg.stack.size()-1;
	}

	int contained_bags(const string& node)
	{
		int v = lookup[node];
		dfs_reset();
		dfs_visit(v);
		for (auto& bag: stack)
		{
			int count = 0;
			for (auto [v2, weight]: vertices[bag].edges)
			{
				count += weight * (vertices[v2].count + 1);
			}
			vertices[bag].count = count;
		}
		return vertices[v].count;
	}
};

istream& operator>>(istream& input, Graph& graph)
{
	const regex pattern("(.*) bags contain|(\\d+) ([^.,]*) bag");

	graph.clear();
	string line;
	while (getline(input, line))
	{
		auto it = sregex_iterator(line.begin(), line.end(), pattern);
		auto end = sregex_iterator();
		if (it == end || !(*it)[1].length())
		{
			continue;
		}
		int v1 = graph.add_vertex((*it)[1]);

		for (++it; it != end; ++it)
		{
			if ((*it)[2].length())
			{
				int v2 = graph.add_vertex((*it)[3]);
				graph.add_edge(v1, v2, stoi((*it)[2]));
			}
		}
	}
	if (graph.vertices.size())
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

	Graph graph;
	input >> graph;
	input.close();
	if (input.fail())
	{
		fmt::print(stderr, "Unable to parse the data\n");
		return 1;
	}

	fmt::print("Part1: {}\n", graph.containing_colors("shiny gold"));
	fmt::print("Part2: {}\n", graph.contained_bags("shiny gold"));
	return 0;
}
