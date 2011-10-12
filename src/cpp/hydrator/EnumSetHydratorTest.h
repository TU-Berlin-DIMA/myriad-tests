/*
 * This file is part of the myriad-toolkit package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef ENUMSETHYDRATORTEST_H_
#define ENUMSETHYDRATORTEST_H_

#include "hydrator/EnumSetHydrator.h"
#include "record/Card.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <vector>

using namespace std;
using namespace CppUnit;

namespace Myriad {

class EnumSetHydratorTest: public TestFixture
{
public:

	EnumSetHydratorTest()
	{
	}

	void testCustomProbabilityHydrator()
	{
		vector<Card::SuiteType> range;
		range.push_back(Card::CLUB);
		range.push_back(Card::DIAMOND);
		range.push_back(Card::HEART);
		range.push_back(Card::SPADE);

		CustomDiscreteProbability f("enumsethydrator.test.random");
		f.define(0, 0.10);
		f.define(1, 0.20);
		f.define(2, 0.30);
		f.define(3, 0.40);

		RandomStream random;
		EnumSetHydrator<Card, Card::SuiteType> hydrator(random, &Card::suite, f);

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

			const Card::SuiteType& type = recordPtr->suite();

			if (type == Card::CLUB)
			{
				distribution[Card::CLUB]++;
			}
			else if (type == Card::DIAMOND)
			{
				distribution[Card::DIAMOND]++;
			}
			else if (type == Card::HEART)
			{
				distribution[Card::HEART]++;
			}
			else if (type == Card::SPADE)
			{
				distribution[Card::SPADE]++;
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
		vector<Card::SuiteType> range;
		range.push_back(Card::CLUB);
		range.push_back(Card::DIAMOND);
		range.push_back(Card::HEART);
		range.push_back(Card::SPADE);

		RandomStream random;
		EnumSetHydrator<Card, Card::SuiteType> hydrator(random, &Card::suite, 4);

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

			const Card::SuiteType& type = recordPtr->suite();

			if (type == Card::CLUB)
			{
				distribution[Card::CLUB]++;
			}
			else if (type == Card::DIAMOND)
			{
				distribution[Card::DIAMOND]++;
			}
			else if (type == Card::HEART)
			{
				distribution[Card::HEART]++;
			}
			else if (type == Card::SPADE)
			{
				distribution[Card::SPADE]++;
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
		TestSuite* suite = new TestSuite("EnumSetHydratorTest");
		suite->addTest(new TestCaller<EnumSetHydratorTest> ("testCustomProbabilityHydrator", &EnumSetHydratorTest::testCustomProbabilityHydrator));
		suite->addTest(new TestCaller<EnumSetHydratorTest> ("testUniformProbabilityHydrator", &EnumSetHydratorTest::testUniformProbabilityHydrator));
		return suite;
	}

private:
};

} // namespace Myriad

#endif /* ENUMSETHYDRATORTEST_H_ */
