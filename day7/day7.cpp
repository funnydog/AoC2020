#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>

using namespace std;

struct Vertex
{
	vector<pair<string,int>> contains;
	vector<string> contained;
	bool visited;
	int count;
};

struct Graph
{
	unordered_map<string, Vertex> vertices;
	vector<string> stack;

	void add_edge(const string& v1, const string& v2, int weight)
	{
		vertices[v1].contains.emplace_back(make_pair(v2, weight));
		vertices[v2].contained.emplace_back(v1);
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

	void dfs_contains_visit(const string& node)
	{
		vertices[node].visited = true;
		for (const auto& [col, num]: vertices[node].contains)
		{
			if (!vertices[col].visited)
			{
				dfs_contains_visit(col);
			}
		}
		stack.push_back(node);
	}

	void dfs_contained_visit(const string& node)
	{
		vertices[node].visited = true;
		for (const auto& col: vertices[node].contained)
		{
			if (!vertices[col].visited)
			{
				dfs_contained_visit(col);
			}
		}
		stack.push_back(node);
	}

	int containing_colors(const string& node)
	{
		dfs_reset();
		dfs_contained_visit(node);
		return stack.size()-1;
	}

	int contained_bags(const string& node)
	{
		dfs_reset();
		dfs_contains_visit(node);
		for (auto& bag: stack)
		{
			int count = 0;
			for (const auto& [col, num]: vertices[bag].contains)
			{
				count += num * (vertices[col].count + 1);
			}
			vertices[bag].count = count;
		}
		return vertices[node].count;
	}
};

void load_graph(istream& input, Graph& graph)
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
	load_graph(input, graph);
	input.close();

	cout << "Part1: " << graph.containing_colors("shiny gold") << endl
	     << "Part2: " << graph.contained_bags("shiny gold") << endl;

	return 0;
}
