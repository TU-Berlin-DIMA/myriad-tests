/*
 * ValueGetterTest.h
 *
 *  Created on: May 2, 2012
 *      Author: alexander
 */

#ifndef VALUEGETTERTEST_H_
#define VALUEGETTERTEST_H_

#include "record/RecordX.h"
#include "record/RecordY.h"
#include "reflection/getter/ValueGetter.h"
#include "reflection/getter/FieldGetter.h"
#include "reflection/getter/ReferencedRecordGetter.h"
#include "reflection/getter/ReferencedRecordFieldGetter.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <Poco/AutoPtr.h>

#include <cmath>

using namespace CppUnit;
using namespace Poco;
using namespace std;

namespace Myriad {

class ValueGetterTest: public TestFixture
{
public:

	void setUp()
	{
		srand(64372234);
	}

	void tearDown()
	{
	}

	void testFieldGetter()
	{
		const ValueGetter<RecordX, I64u>& getA = FieldGetter<RecordX, I64u>(&RecordX::a);
		const ValueGetter<RecordX, String>& getB = FieldGetter<RecordX, String>(&RecordX::b);

		for (int i = 0; i < 10000; i++)
		{
			AutoPtr<RecordX> x = new RecordX();
			fillX(x);

			CPPUNIT_ASSERT_EQUAL(x->a(), getA(x));
			CPPUNIT_ASSERT_EQUAL(x->b(), getB(x));
		}
	}

	void testReferencedRecordGetter()
	{
		const ValueGetter<RecordY, AutoPtr<RecordX> >& getX = ReferencedRecordGetter<RecordY, RecordX>(&RecordY::x);

		AutoPtr<RecordY> y = new RecordY();

		for (int i = 0; i < 10000; i++)
		{
			AutoPtr<RecordX> x = new RecordX();
			fillX(x);

			y->x(x);

			CPPUNIT_ASSERT_EQUAL(x, getX(y));
		}
	}

	void testReferencedRecordFieldGetter()
	{
		const ValueGetter<RecordY, I64u>& getXA = ReferencedRecordFieldGetter<RecordY, RecordX, I64u>(&RecordY::x, &RecordX::a);
		const ValueGetter<RecordY, String>& getXB = ReferencedRecordFieldGetter<RecordY, RecordX, String>(&RecordY::x, &RecordX::b);

		AutoPtr<RecordY> y = new RecordY();

		for (int i = 0; i < 10000; i++)
		{
			AutoPtr<RecordX> x = new RecordX();
			fillX(x);

			y->x(x);

			CPPUNIT_ASSERT_EQUAL(x->a(), getXA(y));
			CPPUNIT_ASSERT_EQUAL(x->b(), getXB(y));
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("ValueGetterTest");
		suite->addTest(new TestCaller<ValueGetterTest>("testFieldGetter", &ValueGetterTest::testFieldGetter));
		suite->addTest(new TestCaller<ValueGetterTest>("testReferencedRecordGetter", &ValueGetterTest::testReferencedRecordGetter));
		suite->addTest(new TestCaller<ValueGetterTest>("testReferencedRecordFieldGetter", &ValueGetterTest::testReferencedRecordFieldGetter));
		return suite;
	}

private:

	void fillX(AutoPtr<RecordX>& x)
	{
		// set random value of 'a'
		x->a(rand() % 9 + 2);

		// set random value of 'b'
		I16u b = rand() % 4;
		if (b == 0)
		{
			x->b("club");
		}
		else if (b == 1)
		{
			x->b("heart");
		}
		else if (b == 2)
		{
			x->b("spade");
		}
		else if (b == 3)
		{
			x->b("diamond");
		}
	}
};

} // namespace Myriad

#endif /* VALUEGETTERTEST_H_ */
