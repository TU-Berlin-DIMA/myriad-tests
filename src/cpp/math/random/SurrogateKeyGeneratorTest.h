/*
 * This file is part of the myriad-dgen package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef SURROGATEKEYGENERATORTEST_H_
#define SURROGATEKEYGENERATORTEST_H_

#include "math/random/SurrogateKeyGenerator.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <Poco/Format.h>

using namespace CppUnit;
using namespace Poco;

namespace Myriad {

class SurrogateKeyGeneratorTest: public TestFixture
{
public:
	void setUp()
	{
		N = nextPrime(100000000);
		skGen.configure(N);
	}

	void tearDown()
	{
		N = 0;
	}

	void testRandomAccess()
	{
		CPPUNIT_ASSERT((I64u) skGen[0] == 1);
		CPPUNIT_ASSERT((I64u) skGen[1] == 43);
		CPPUNIT_ASSERT((I64u) skGen[242113] == 53628160);
		CPPUNIT_ASSERT((I64u) skGen[4321230] == 27812749);
		CPPUNIT_ASSERT((I64u) skGen[459231] == 47762795);
		CPPUNIT_ASSERT((I64u) skGen[79301211] == 19341446);
		CPPUNIT_ASSERT((I64u) skGen[24051985] == 31448861);
	}


	void testLargeIntervals()
	{
		skGen.configure(100003);

		SurrogateKeyGenerator::iterator it = skGen[0];

		I64u i = 0;
		I64u x = 1;
		while (i < 100000)
		{
			CPPUNIT_ASSERT(static_cast<I64u>(skGen[i]) == x);
			CPPUNIT_ASSERT(static_cast<I64u>(it) == x);

			x = (x*46) % 100003;

			++i;
			++it;
		}
	}

	void testIterator()
	{
		I64u N = 2343763; //171877;
		I64u a = 44; //45;
		SurrogateKeyGenerator skGen(N);

		// test postfix incremet it++
		I64u i = 0;
		SurrogateKeyGenerator::iterator it = skGen[i];

		do
		{
			I64u x = modexp(a, i, N);

			CPPUNIT_ASSERT(skGen(i) == x);
			CPPUNIT_ASSERT(static_cast<I64u>(skGen[i]) == x);
			CPPUNIT_ASSERT(it++ == x);

			i++;
		}
		while(static_cast<I64u>(it) != 1);

		// check order of i
		CPPUNIT_ASSERT(i == N-1);

		I64u begin = 1234567, end = 1244567, j = begin;
		for (SurrogateKeyGenerator::iterator it = skGen[begin]; it != skGen[end]; ++it)
		{
			I64u x = modexp(a, j, N);

			CPPUNIT_ASSERT(skGen(j) == x);
			CPPUNIT_ASSERT(static_cast<I64u>(skGen[j]) == x);
			CPPUNIT_ASSERT(it == x);

			j++;
		}
	}


	void testLargeIntegers()
	{
		I64u N = 8832000017ULL;
		skGen.configure(N);

		SurrogateKeyGenerator::iterator it = skGen[1];

		I64u i = 1;
		while (static_cast<I64u>(it) != 1)
		{
			++i;
			++it;

			if (i % 100000000 == 0)
			{
				std::cout << "current position is " << i << std::endl;

				if (i >= N)
				{
					CPPUNIT_FAIL("skGen iterator overflow");
				}
			}
		}

		std::cout << "i = " << i << endl;
		CPPUNIT_ASSERT(i == N-1);
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("SurrogateKeyGeneratorTest");
		suite->addTest(new TestCaller<SurrogateKeyGeneratorTest> ("testRandomAccess", &SurrogateKeyGeneratorTest::testRandomAccess));
		suite->addTest(new TestCaller<SurrogateKeyGeneratorTest> ("testLargeIntervals", &SurrogateKeyGeneratorTest::testLargeIntervals));
		suite->addTest(new TestCaller<SurrogateKeyGeneratorTest> ("testIterator", &SurrogateKeyGeneratorTest::testIterator));
//		suite->addTest(new TestCaller<SurrogateKeyGeneratorTest> ("testLargeIntegers", &SurrogateKeyGeneratorTest::testLargeIntegers));
		return suite;
	}

private:

	I64u N;
	SurrogateKeyGenerator skGen;
};

} // namespace Myriad

#endif /* SURROGATEKEYGENERATORTEST_H_ */
