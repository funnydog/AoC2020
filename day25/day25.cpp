#include <climits>
#include <fstream>
#include <iostream>

using namespace std;

static const unsigned BASE = 7U;
static const unsigned MODULUS = 20201227U;

static unsigned mulmod(unsigned a, unsigned b, unsigned m)
{
	if (a > b)
	{
		swap(a, b);
	}

	if (b >= m)
	{
		if (m > UINT_MAX / 2U)
		{
			b -= m;
		}
		else
		{
			b %= m;
		}
	}

	unsigned res = 0;
	while (a)
	{
		if (a & 1)
		{
			if (b >= m - res)
			{
				res -= m;
			}
			res += b;
		}
		a >>= 1;

		unsigned tmp = b;
		if (b >= m - b)
		{
			tmp -= m;
		}
		b += tmp;
	}
	return res;
}

static unsigned powmod(unsigned b, unsigned e, unsigned m)
{
	if (m == 1)
	{
		return 0;
	}
	if (b >= m)
	{
		if (m > UINT_MAX / 2U)
		{
			b -= m;
		}
		else
		{
			b %= m;
		}
	}
	unsigned result = 1;
	while (e > 0)
	{
		if (e & 1)
		{
			result = mulmod(result, b, m);
		}
		e >>= 1;
		b = mulmod(b, b, m);
	}
	return result;
}

static unsigned get_encryption_key(unsigned card, unsigned door)
{
	unsigned key = BASE;
	unsigned e = 1;
	while (key != card && key != door)
	{
		key = mulmod(BASE, key, MODULUS);
		e++;
	}
	return powmod(key == door ? card : door, e, MODULUS);
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

	unsigned card, door;
	input >> card >> door;
	input.close();
	if (!input)
	{
		cerr << "Cannot parse the data" << endl;
	}

	cout << "Part1: " << get_encryption_key(card, door) << endl;
	return 0;
}
