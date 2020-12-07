#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>

using namespace std;

struct Vertex
{
	vector<pair<string,int>> edges;
	bool visited;
	int count;
};

struct Graph
{
	unordered_map<string, Vertex> vertices;
	vector<string> stack;

	Graph transpose() const
	{
		Graph newgraph;
		for (const auto& [key, value]: vertices)
		{
			newgraph.vertices.emplace(key, Vertex());
		}
		for (const auto& [name, vertex]: vertices)
		{
			for (const auto& [dst, weight]: vertex.edges)
			{
				newgraph.vertices[dst].edges.emplace_back(
					make_pair(name, weight));
			}
		}
		return newgraph;
	}

	void add_edge(const string& v1, const string& v2, int weight)
	{
		vertices[v1].edges.emplace_back(make_pair(v2, weight));
	}

	void dfs_reset(void)
	{
		for (auto& [key, val]: vertices)
		{
			val.visited = false;
			val.count = 0;
		}
		stack.clear();
	}

	void dfs_visit(const string& node)
	{
		vertices[node].visited = true;
		for (const auto& [col, num]: vertices[node].edges)
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
		newg.dfs_visit(node);
		return newg.stack.size()-1;
	}

	int contained_bags(const string& node)
	{
		dfs_reset();
		dfs_visit(node);
		for (auto& bag: stack)
		{
			int count = 0;
			for (const auto& [col, num]: vertices[bag].edges)
			{
				count += num * (vertices[col].count + 1);
			}
			vertices[bag].count = count;
		}
		return vertices[node].count;
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

		string name = sm[1];

		auto end = sregex_iterator();
		for (auto it = sregex_iterator(line.begin(), line.end(), pat2);
		     it != end;
		     ++it)
		{
			sm = *it;
			graph.add_edge(name, sm[2], stoi(sm[1]));
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
