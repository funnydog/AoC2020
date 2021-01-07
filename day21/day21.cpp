#include <algorithm>
#include <fstream>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <fmt/format.h>

using namespace std;

// NOTE: the ingredients are sorted by name
struct Food
{
	vector<string> ingredients;
	vector<string> allergens;
};

struct Dangerous
{
	string allergen;
	string ingredient;
};

istream& operator>>(istream& input, Food& food)
{
	string line;
	if (!getline(input, line))
	{
		return input;
	}
	food.ingredients.clear();
	food.allergens.clear();
	size_t pos = 0;
	size_t next;
	enum {INGREDIENT, ALLERGENIC} state = INGREDIENT;
	do
	{
		next = line.find(' ', pos);
		if (state == INGREDIENT)
		{
			if (line[pos] == '(')
			{
				state = ALLERGENIC;
			}
			else
			{
				food.ingredients.emplace_back(
					line.substr(pos, next-pos));
			}
		}
		else
		{
			auto l = line.substr(pos, next-pos);
			l.pop_back();
			food.allergens.emplace_back(l);
		}
		pos = next + 1;
	} while (next != string::npos);

	// sort ingredients by name
	sort(food.ingredients.begin(), food.ingredients.end());

	return input;
}

static vector<string> intersect(const vector<string>& s1, const vector<string>& s2)
{
	vector<string> result;
	for (auto& a: s1)
	{
		// NOTE: assume s2 is sorted (see the parse function)
		if (binary_search(s2.begin(), s2.end(), a))
		{
			result.push_back(a);
		}
	}
	return result;
}

static vector<Dangerous> find_allergens(const vector<Food>& foods)
{
	// find the intersection of ingredients for each allergen
	unordered_map<string, vector<string>> constraints;
	for (auto& f: foods)
	{
		for (auto& a: f.allergens)
		{
			auto it = constraints.find(a);
			if (it == constraints.end())
			{
				constraints[a] = f.ingredients;
			}
			else
			{
				it->second = intersect(it->second, f.ingredients);
			}
		}
	}

	// remove the certain allergens
	vector<Dangerous> allergens;
	while (!constraints.empty())
	{
		for (auto it = constraints.begin(); it != constraints.end(); )
		{
			if (it->second.empty())
			{
				it = constraints.erase(it);
			}
			else if (it->second.size() == 1)
			{
				auto value = it->second[0];
				allergens.emplace_back(
					Dangerous{it->first, value});
				for (auto& [al, ingr]: constraints)
				{
					// NOTE: this assumes the
					// ingredients are sorted
					auto [begin, end] = equal_range(
						ingr.begin(),
						ingr.end(),
						value);
					ingr.erase(begin, end);
				}
				it = constraints.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	// sort by allergen name
	sort(allergens.begin(), allergens.end(),
	     [](const auto& a, const auto& b){
		     return a.allergen < b.allergen;
	     });
	return allergens;
}

static size_t part1(const vector<Food>& foods, const vector<Dangerous> & allergens)
{
	size_t count = 0;
	for (auto& f: foods)
	{
		for (auto& i: f.ingredients)
		{
			if (!any_of(allergens.begin(),
				    allergens.end(),
				    [i](const auto &a) {
					    return i == a.ingredient;
				    }))
			{
				count++;
			}
		}
	}
	return count;
}

static string part2(const vector<Dangerous>& allergens)
{
	string line = accumulate(
		allergens.begin(),
		allergens.end(),
		""s,
		[](const auto& acc, const auto& d)
		{
			return acc + d.ingredient + ",";
		});
	if (!line.empty())
	{
		line.pop_back();
	}
	return line;
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

	vector<Food> foods;
	Food food;
	while (input >> food)
	{
		foods.emplace_back(move(food));
	}
	input.close();

	auto allergens = find_allergens(foods);

	fmt::print("Part1: {}\n", part1(foods, allergens));
	fmt::print("Part2: {}\n", part2(allergens));
	return 0;
}
