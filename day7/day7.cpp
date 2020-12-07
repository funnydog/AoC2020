#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>

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
	regex pat1("(.*) bags contain");
	regex pat2("(\\d+) ([^.,]*) bags?[.,]");
	string line;
	while (getline(input, line))
	{
		smatch sm;
		if (!regex_search(line, sm , pat1))
		{
			continue;
		}

		int v1 = graph.add_vertex(sm[1]);

		auto end = sregex_iterator();
		for (auto it = sregex_iterator(line.begin(), line.end(), pat2);
		     it != end;
		     ++it)
		{
			sm = *it;
			int v2 = graph.add_vertex(sm[2]);
			graph.add_edge(v1, v2, stoi(sm[1]));
		}
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

	Graph graph;
	input >> graph;
	input.close();

	cout << "Part1: " << graph.containing_colors("shiny gold") << endl
	     << "Part2: " << graph.contained_bags("shiny gold") << endl;

	return 0;
}
