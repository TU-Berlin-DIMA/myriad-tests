/*
 * This file is part of the myriad-toolkit package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef CONDITIONALRANDOMIZEDHYDRATORTEST_H_
#define CONDITIONALRANDOMIZEDHYDRATORTEST_H_

#include "hydrator/ConditionalRandomizedHydrator.h"
#include "math/probability/ConditionalQHistogramPrFunction.h"
#include "record/RecordX.h"
#include "record/RecordY.h"
#include "reflection/getter/ReferencedRecordFieldGetter.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace std;
using namespace CppUnit;

namespace Myriad {

class ConditionalRandomizedHydratorTest: public TestFixture
{
public:

	ConditionalRandomizedHydratorTest() :
		_probability(NULL)
	{
	}

	void setUp()
	{
		_random.resetSubstream();

	    size_t size = 512;
	    char path[size];

		_probability = new ConditionalQHistogramPrFunction(string(getcwd(path, size)) + "/build/config/4x4_cond_qhist.qhistogram");
	}

	void tearDown()
	{
		delete _probability;
	}

	void testQHistogramHydrator()
	{
		ReferencedRecordFieldGetter<RecordY, RecordX, I64u>* fieldGetter = new ReferencedRecordFieldGetter<RecordY, RecordX, I64u>(&RecordY::x, &RecordX::a);
		ConditionalRandomizedHydrator<RecordY, I64u, I64u, ConditionalQHistogramPrFunction> hydrator(_random, &RecordY::a, fieldGetter, *_probability);

		unsigned int N = 10000;

		for (unsigned int i = 0; i < N; i++)
		{
			AutoPtr<RecordX> x = new RecordX();
			x->a(_random(0,3));
			AutoPtr<RecordY> y = new RecordY();
			y->x(x);

			hydrator(y);

			CPPUNIT_ASSERT_EQUAL(x->a()+2, y->a());

			_random.nextChunk();
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
		suite->addTest(new TestCaller<ConditionalRandomizedHydratorTest> ("testQHistogramHydrator", &ConditionalRandomizedHydratorTest::testQHistogramHydrator));
		return suite;
	}

private:

	RandomStream _random;
	ConditionalQHistogramPrFunction* _probability;
};

} // namespace Myriad

#endif /* CONDITIONALRANDOMIZEDHYDRATORTEST_H_ */
