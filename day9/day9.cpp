#include <fstream>
#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>

using namespace std;

class Decoder
{
public:
	explicit Decoder(size_t prelen)
		: prelen(prelen),
		  mat(new int[prelen*prelen]),
		  nums(new int[prelen]),
		  numcount(0)
	{
	}

	~Decoder()
	{
		delete[] nums;
		delete[] mat;
	}

	Decoder(const Decoder&) = delete;
	Decoder& operator=(const Decoder&) = delete;
	Decoder(Decoder&&) = delete;
	Decoder& operator=(Decoder&&) = delete;

	void process(int value)
	{
		// removal
		if (numcount == prelen)
		{
			copy(nums + 1, nums + prelen, nums);
			// NOTE: shift the matrix 1 row up and 1 col left
			for (size_t i = 1; i < prelen-1; i++)
			{
				for (size_t j = 0; j < i; j++)
				{
					mat[i*prelen+j] = mat[(i+1)*prelen+j+1];
				}
			}
			numcount--;
		}

		// insertion
		for (size_t i = 0; i < numcount; i++)
		{
			mat[prelen * numcount + i] = nums[i] + value;
		}
		nums[numcount] = value;
		numcount++;
	}

	bool is_valid(int value)
	{
		if (numcount < prelen)
		{
			return true;
		}

		for (size_t i = 1; i < prelen; i++)
		{
			for (size_t j = 0; j < i; j++)
			{
				if (mat[i * prelen + j] == value)
				{
					return true;
				}
			}
		}

		return false;
	}

	pair<int, int> find_solutions(const vector<int>& numbers)
	{
		int outlier = -1;
		for (auto v: numbers)
		{
			if (is_valid(v))
			{
				process(v);
			}
			else
			{
				outlier = v;
				break;
			}
		}

		for (size_t i = 0; i < numbers.size(); i++)
		{
			int sum = numbers[i];
			int max = sum;
			int min = sum;
			for (size_t j = i+1; j < numbers.size(); j++)
			{
				if (max < numbers[j]) max = numbers[j];
				if (min > numbers[j]) min = numbers[j];
				sum += numbers[j];
				if (sum == outlier)
				{
					return make_pair(outlier, min+max);
				}
				else if (sum > outlier)
				{
					break;
				}
			}
		}
		return make_pair(outlier, -1);
	}

private:
	size_t prelen;
	int *mat;
	int *nums;
	size_t numcount;
};

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cerr << "Usage: " << argv[0] << " <length> <filename>" << endl;
		return 1;
	}

	ifstream input(argv[2]);
	if (!input)
	{
		cerr << "Cannot open " << argv[2] << endl;
		return 1;
	}

	vector<int> numbers;
	int value;
	while (input >> value)
	{
		numbers.push_back(value);
	}
	input.close();

	Decoder d(stoi(argv[1]));
	auto [outlier, key] = d.find_solutions(numbers);
	cout << "Part1: " << outlier << endl
	     << "Part2: " << key << endl;

	return 0;
}
