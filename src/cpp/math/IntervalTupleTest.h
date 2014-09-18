#ifndef INTERVALTUPLETEST_H_
#define INTERVALTUPLETEST_H_

#include "core/types/MyriadAbstractTuple.h"
#include "math/IntervalTuple.h"


#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace std;
using namespace CppUnit;

namespace Myriad
{

class IntervalTupleTest: public TestFixture
{
public:
	IntervalTupleTest(){}

	// test for constructor, min, max
	void testConstructorIntervalTuple()
	{
		const MyriadTuple<I64, I64> minIn(11, 1023);
		const MyriadTuple<I64, I64> maxIn(12, 1024);
		IntervalTuple<MyriadTuple<I64, I64> > interval(minIn, maxIn);


//		MyriadTuple<I64, I64> minOut(interval.min());
//		MyriadTuple<I64, I64> maxOut(interval.max());

		// test for MyriadTuple also
//		CPPUNIT_ASSERT(minIn == minOut);
//		CPPUNIT_ASSERT_EQUAL(maxIn, maxOut);

//		CPPUNIT_ASSERT_EQUAL(v2, t1.getSecond());
//
//		I32 v3 = 0;
//		MyriadTuple<I16, I32> t2(v1, v3);
//		CPPUNIT_ASSERT_EQUAL(v1, t2.getFirst());
//		CPPUNIT_ASSERT_EQUAL(v3, t2.getSecond());
//
//		Decimal v4 = 0.4;
//		MyriadTuple<I32, Decimal> t3(v3, v4);
//		CPPUNIT_ASSERT_EQUAL(v3, t3.getFirst());
//		CPPUNIT_ASSERT_EQUAL(v4, t3.getSecond());
//
//		ID v5 = 1024;
//		String v6 = "smoke";
//		MyriadTuple<ID, String> t4(v5, v6);
//		CPPUNIT_ASSERT_EQUAL(v5, t4.getFirst());
//		CPPUNIT_ASSERT_EQUAL(v6, t4.getSecond());
	}

	void testConstructorTriple()
	{
		I16 v1 = 11, v2 = 11, v3 = 21;
		MyriadTriple<I16, I16, I16> t1(v1, v2, v3);
		CPPUNIT_ASSERT_EQUAL(v1, t1.getFirst());
		CPPUNIT_ASSERT_EQUAL(v2, t1.getSecond());
		CPPUNIT_ASSERT_EQUAL(v3, t1.getThird());

		I64 v4 = 2048;
		MyriadTriple<I16, I64, I64> t2(v1, v4, v4);
		CPPUNIT_ASSERT_EQUAL(v1, t2.getFirst());
		CPPUNIT_ASSERT_EQUAL(v4, t2.getSecond());
		CPPUNIT_ASSERT_EQUAL(v4, t2.getThird());

		String v5 = "holy";
		MyriadTriple<I64, String, String> t3(v4, v5, v5);
		CPPUNIT_ASSERT_EQUAL(v4, t3.getFirst());
		CPPUNIT_ASSERT_EQUAL(v5, t3.getSecond());
		CPPUNIT_ASSERT_EQUAL(v5, t3.getThird());
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("IntervalTupleTest");
		suite->addTest(new TestCaller<IntervalTupleTest> ("testConstructorTuple", &IntervalTupleTest::testConstructorIntervalTuple));
		//suite->addTest(new TestCaller<IntervalTupleTest> ("testConstructorTriple", &IntervalTupleTest::testConstructorTriple));
		return suite;
	}

private:


};

} // namespace Myriad

#endif /* INTERVALTUPLETEST_H_ */
