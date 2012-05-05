/*
 * ValueGetterTest.h
 *
 *  Created on: May 2, 2012
 *      Author: alexander
 */

#ifndef VALUEGETTERTEST_H_
#define VALUEGETTERTEST_H_

#include "record/Record.h"
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

class X: public Record
{
public:

	void a(const I64u& v)
	{
		_a = v;
	}

	const I64u& a() const
	{
		return _a;
	}

	void b(const String& v)
	{
		_b = v;
	}

	const String& b() const
	{
		return _b;
	}

private:

	I64u _a;
	String _b;
};

class Y: public Record
{
public:

	void x(const AutoPtr<X>& v)
	{
		_x = v;
	}

	const AutoPtr<X>& x() const
	{
		return _x;
	}

private:

	AutoPtr<X> _x;
};

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
		const ValueGetter<X, I64u>& getA = FieldGetter<X, I64u>(&X::a);
		const ValueGetter<X, String>& getB = FieldGetter<X, String>(&X::b);

		for (int i = 0; i < 10000; i++)
		{
			AutoPtr<X> x = new X();
			fillX(x);

			CPPUNIT_ASSERT_EQUAL(x->a(), getA(x));
			CPPUNIT_ASSERT_EQUAL(x->b(), getB(x));
		}
	}

	void testReferencedRecordGetter()
	{
		const ValueGetter<Y, AutoPtr<X> >& getX = ReferencedRecordGetter<Y, X>(&Y::x);

		AutoPtr<Y> y = new Y();

		for (int i = 0; i < 10000; i++)
		{
			AutoPtr<X> x = new X();
			fillX(x);

			y->x(x);

			CPPUNIT_ASSERT_EQUAL(x, getX(y));
		}
	}

	void testReferencedRecordFieldGetter()
	{
		const ValueGetter<Y, I64u>& getXA = ReferencedRecordFieldGetter<Y, X, I64u>(&Y::x, &X::a);
		const ValueGetter<Y, String>& getXB = ReferencedRecordFieldGetter<Y, X, String>(&Y::x, &X::b);

		AutoPtr<Y> y = new Y();

		for (int i = 0; i < 10000; i++)
		{
			AutoPtr<X> x = new X();
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

	void fillX(AutoPtr<X>& x)
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
