/*
 * This file is part of the myriad-dgen package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PERMUTATORTEST_H_
#define PERMUTATORTEST_H_

#include "core/UnitTest.h"
#include "math/random/Permutator.h"

#include <algorithm>
#include <list>

using std::for_each;
using std::list;

namespace Myriad {

class PermutationConfig
{
public:
	// configuration name
	string configName;
	// generator of the underlying Group
	unsigned long g;
	// the modulo value P (should be prime)
	unsigned long P;
	unsigned long* map;
};

class PermutatorTest: public UnitTest
{
public:
	typedef list<PermutationConfig*> ConfigList;

	void run()
	{
		// setup some configurations in a map
		ConfigList list;

		// config #1: products
		{
			PermutationConfig* config = new PermutationConfig();
			config->configName = "products";
			config->g = 500009;
			config->P = 500009;
			list.push_back(config);
		}

		// config #2: resellers
		{
			PermutationConfig* config = new PermutationConfig();
			config->configName = "resellers";
			config->g = 2436;
			config->P = 25013;
			list.push_back(config);
		}

		// config #2: productOffers
		{
			PermutationConfig* config = new PermutationConfig();
			config->configName = "productOffers";
			config->g = 627617;
			config->P = 2500009;
			list.push_back(config);
		}

		for_each(list.begin(), list.end(), &testConfig);
	}

	static void testConfig(PermutationConfig* config)
	{
		std::cout << format("testing setup `%s`", config->configName) << std::endl;
		std::cout << format("*  G  := <%lu>", config->g    ) << std::endl;
		std::cout << format("* |G| := <%lu>", config->P - 1) << std::endl;
		std::cout << format("*  P  := <%lu>", config->P    ) << std::endl;
		std::cout << std::endl;

		unsigned long g = config->g;
		unsigned long N = config->P;
		unsigned long map[N];

		Permutator p(g, N);

		std::cout << "computing map..." << std::endl;

		for (unsigned long i = 0; i < N - 1; i++)
		{
			unsigned long x = p.exp(i);

			map[i] = x;
		}

		etrophy(map, N, 2);
		etrophy(map, N, 4);
		etrophy(map, N, 8);
	}

	// check the sign etrophy of all 3-sequences
	static void etrophy(unsigned long* mapPtr, unsigned long mapSize, unsigned long l)
	{
		unsigned long* M = mapPtr;
		unsigned long sizeM = mapSize;

		unsigned long level = l;
		unsigned long sizeT = pow(2, level);
		unsigned long T[sizeT];

		std::cout << "compute etrophy map for level = " << level << std::endl;
		std::cout << std::endl;

		for (unsigned long i = 0; i < sizeT; i++)
		{
			T[i] = 0;
		}

		for (unsigned long j = 0; j < sizeM - level; j++)
		{
			unsigned short z = 0;

			for (unsigned long i = 0; i < level; i++)
			{
				if (M[j + i] < M[j + i + 1])
				{
					z = z | (1 << i);
				}
			}

			T[z]++;
		}

		char y = 'h';
		char n = 'l';

		for (unsigned long i = 0; i < sizeT; i++)
		{
			for (unsigned long j = 0; j < level; j++)
			{
				if ((1 << j) & i)
				{
					std::cout << y;
				}
				else
				{
					std::cout << n;
				}
			}

			std::cout << format(" = %-10lu (%-10E)", T[i], (T[i] / static_cast<double> (sizeM))) << std::endl;
		}

		std::cout << std::endl;
	} // end of etrophy function
};

// parameters:
// 500000 => P := 500009, g := 372705

} // namespace Myriad

#endif /* PERMUTATORTEST_H_ */
