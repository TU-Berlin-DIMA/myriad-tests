/*
 * This file is part of the myriad-toolkit package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef CONDITIONALHYDRATORTEST_H_
#define CONDITIONALHYDRATORTEST_H_

#include "hydrator/ConditionalHydrator.h"
#include "hydrator/ConstValueHydrator.h"
#include "record/Card.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <vector>

using namespace std;
using namespace CppUnit;

namespace Myriad {

class ConditionalHydratorTest: public TestFixture
{
public:

	typedef ConstValueHydrator<Card, String> H1;
	typedef ConditionalHydrator<Card, Card::SuiteType, H1, H1> H2;

	void testHydrator()
	{
		const string queen = "Queen";
		const string king = "King";

		RandomStream random;

		H1 queenHydrator(&Card::type, queen);
		H1 kingHydrator(&Card::type, king);
		H2 hydrator(&Card::suite, Card::DIAMOND, queenHydrator, kingHydrator);

		for (unsigned int i = 0; i < 10000; i++)
		{
			AutoPtr<Card> recordPtr = new Card();
			recordPtr->suite(static_cast<Card::SuiteType>(random(0, 3)));

			hydrator(recordPtr);

			const string x = recordPtr->type();

			if (recordPtr->suite() == Card::DIAMOND)
			{
				CPPUNIT_ASSERT_EQUAL(queen, x);
			}
			else
			{
				CPPUNIT_ASSERT_EQUAL(king, x);
			}

			random.nextChunk();
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("ConditionalHydratorTest");
		suite->addTest(new TestCaller<ConditionalHydratorTest> ("testHydrator", &ConditionalHydratorTest::testHydrator));
		return suite;
	}
};

} // namespace Myriad

#endif /* CONDITIONALHYDRATORTEST_H_ */
