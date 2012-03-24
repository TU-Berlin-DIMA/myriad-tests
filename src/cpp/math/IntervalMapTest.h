/*
 * This file is part of the myriad-dgen package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef INTERVALMAPTEST_H_
#define INTERVALMAPTEST_H_

#include "math/util.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

class IntervalMapTest: public TestFixture
{
public:

	void setUp()
	{
		_intervalMapPtr = new IntervalMap<ID, I16u>("anonymous");
	}

	void tearDown()
	{
		delete _intervalMapPtr;
	}

	void testIntervalCompareOperator()
	{
		Interval<ID> a(10, 20);
		Interval<ID> b(15, 25);
		Interval<ID> c(20, 30);
		Interval<ID> d(40, 50);
		Interval<ID> e(10, 55);

		CPPUNIT_ASSERT(a < b);
		CPPUNIT_ASSERT(b < c);
		CPPUNIT_ASSERT(c < d);
		CPPUNIT_ASSERT(a < e);
		CPPUNIT_ASSERT(e < b);
	}

	void testDisjunctIntervalMap()
	{
		IntervalMap<ID, I16u>& map = *_intervalMapPtr;

		// define disjunct intervals
		map.define(Interval<ID>(0    , 500 ), 0);
		map.define(Interval<ID>(500  , 1000), 1);
		map.define(Interval<ID>(1000 , 2000), 2);
		map.define(Interval<ID>(2000 , 3400), 3);
		map.define(Interval<ID>(3400 , 5000), 4);

		// test disjunct intervals
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(0), map(0));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(0), map(125));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(0), map(499));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(1), map(500));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(1), map(750));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(1), map(999));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(2), map(1000));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(2), map(1500));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(2), map(1999));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(3), map(2000));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(3), map(2700));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(3), map(3399));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(4), map(3400));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(4), map(4200));
		CPPUNIT_ASSERT_EQUAL(static_cast<I16u>(4), map(4999));
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("IntervalMapTest");
		suite->addTest(new TestCaller<IntervalMapTest> ("testIntervalCompareOperator", &IntervalMapTest::testIntervalCompareOperator));
		suite->addTest(new TestCaller<IntervalMapTest> ("testDisjunctIntervalMap", &IntervalMapTest::testDisjunctIntervalMap));
		return suite;
	}

private:

	IntervalMap<ID, I16u>* _intervalMapPtr;
};

} // namespace Myriad

#endif /* INTERVALMAPTEST_H_ */
