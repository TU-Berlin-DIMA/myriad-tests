/*
 * This file is part of the myriad-toolkit package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef RANGESETHYDRATORTEST_H_
#define RANGESETHYDRATORTEST_H_

#include "hydrator/RangeSetHydrator.h"
#include "record/Card.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <vector>

using namespace std;
using namespace CppUnit;

namespace Myriad {

class RangeSetHydratorTest: public TestFixture
{
public:

	RangeSetHydratorTest()
	{
	}

	void testCustomProbabilityHydrator()
	{
		vector<String> range;
		range.push_back("Jack");
		range.push_back("Queen");
		range.push_back("King");
		range.push_back("Ace");

		CustomDiscreteProbability f("rangesethydrator.test.random");
		f.define(0, 0.10);
		f.define(1, 0.20);
		f.define(2, 0.30);
		f.define(3, 0.40);

		RandomStream random;
		RangeSetHydrator<Card, String> hydrator(random, &Card::type, range, f);

		unsigned int N = 100000;
		unsigned int M = range.size();
		unsigned int distribution[M];

		for (unsigned int i = 0; i < M; i++)
		{
			distribution[i] = 0;
		}

		for (unsigned int i = 0; i < N; i++)
		{
			AutoPtr<Card> recordPtr = new Card();

			hydrator(recordPtr);

			const String& type = recordPtr->type();

			if (type == "Jack")
			{
				distribution[0]++;
			}
			else if (type == "Queen")
			{
				distribution[1]++;
			}
			else if (type == "King")
			{
				distribution[2]++;
			}
			else if (type == "Ace")
			{
				distribution[3]++;
			}

			random.nextChunk();
		}

		for (unsigned int i = 0; i < range.size(); i++)
		{
			double x = f.at(i);
			double y = distribution[i]/static_cast<double>(N);

			CPPUNIT_ASSERT_DOUBLES_EQUAL(x, y, 0.05);
		}
	}

	void testUniformProbabilityHydrator()
	{
		vector<String> range;
		range.push_back("Jack");
		range.push_back("Queen");
		range.push_back("King");
		range.push_back("Ace");

		RandomStream random;
		RangeSetHydrator<Card, String> hydrator(random, &Card::type, range);

		unsigned int N = 100000;
		unsigned int M = range.size();
		unsigned int distribution[M];

		for (unsigned int i = 0; i < M; i++)
		{
			distribution[i] = 0;
		}

		for (unsigned int i = 0; i < N; i++)
		{
			AutoPtr<Card> recordPtr = new Card();

			hydrator(recordPtr);

			const String& type = recordPtr->type();

			if (type == "Jack")
			{
				distribution[0]++;
			}
			else if (type == "Queen")
			{
				distribution[1]++;
			}
			else if (type == "King")
			{
				distribution[2]++;
			}
			else if (type == "Ace")
			{
				distribution[3]++;
			}

			random.nextChunk();
		}

		for (unsigned int i = 0; i < range.size(); i++)
		{
			double x = 1/static_cast<double>(M);
			double y = distribution[i]/static_cast<double>(N);

			CPPUNIT_ASSERT_DOUBLES_EQUAL(x, y, 0.05);
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("RangeSetHydratorTest");
		suite->addTest(new TestCaller<RangeSetHydratorTest> ("testCustomProbabilityHydrator", &RangeSetHydratorTest::testCustomProbabilityHydrator));
		suite->addTest(new TestCaller<RangeSetHydratorTest> ("testUniformProbabilityHydrator", &RangeSetHydratorTest::testUniformProbabilityHydrator));
		return suite;
	}

private:
};

} // namespace Myriad

#endif /* RANGESETHYDRATORTEST_H_ */
