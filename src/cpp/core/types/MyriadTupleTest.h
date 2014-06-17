/*
 * MyriadTupleTest.h
 *
 *  Created on: Jun 5, 2014
 *      Author: mho
 */

#ifndef MYRIADTUPLETEST_H_
#define MYRIADTUPLETEST_H_

#include "core/types/MyriadAbstractTuple.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace std;
using namespace CppUnit;

namespace Myriad
{

class MyriadTupleTest: public TestFixture
{
public:
	MyriadTupleTest(){}

	void testConstructorTuple()
	{
		I16 v1 = 11, v2 = 21;
		MyriadTuple<I16, I16> t1(v1, v2);
		CPPUNIT_ASSERT_EQUAL(v1, t1.getFirst());
		CPPUNIT_ASSERT_EQUAL(v2, t1.getSecond());

		I32 v3 = 0;
		MyriadTuple<I16, I32> t2(v1, v3);
		CPPUNIT_ASSERT_EQUAL(v1, t2.getFirst());
		CPPUNIT_ASSERT_EQUAL(v3, t2.getSecond());

		Decimal v4 = 0.4;
		MyriadTuple<I32, Decimal> t3(v3, v4);
		CPPUNIT_ASSERT_EQUAL(v3, t3.getFirst());
		CPPUNIT_ASSERT_EQUAL(v4, t3.getSecond());

		ID v5 = 1024;
		String v6 = "smoke";
		MyriadTuple<ID, String> t4(v5, v6);
		CPPUNIT_ASSERT_EQUAL(v5, t4.getFirst());
		CPPUNIT_ASSERT_EQUAL(v6, t4.getSecond());
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

	// test operator== for MyriadTuple
	void testIsEqual(){
		MyriadTuple<I64,I64> t(1, 1);
		MyriadTuple<I64, I64> t1(1, 1);
		CPPUNIT_ASSERT(t == t1);

		MyriadTuple<String, I32> t2("42", 2048);
		MyriadTuple<String, I32> t3("42", 2048);
		CPPUNIT_ASSERT(t2 == t3);
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("MyriadTupleTest");
		suite->addTest(new TestCaller<MyriadTupleTest> ("testConstructorTuple", &MyriadTupleTest::testConstructorTuple));
		suite->addTest(new TestCaller<MyriadTupleTest> ("testConstructorTriple", &MyriadTupleTest::testConstructorTriple));
		suite->addTest(new TestCaller<MyriadTupleTest> ("testIsEqual", &MyriadTupleTest::testIsEqual));

		return suite;
	}

private:


};

} // namespace Myriad

#endif /* MYRIADTUPLETEST_H_ */
