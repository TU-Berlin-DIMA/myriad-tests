/*
 * This file is part of the myriad-dgen package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef CONSTVALUEPROVIDERTEST_H_
#define CONSTVALUEPROVIDERTEST_H_

#include "record/Record.h"
#include "runtime/provider/value/ConstValueProvider.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

class MockRecord;

class ConstValueProviderTest: public TestFixture
{
public:

	void setUp()
	{
		srand(54352364);
	}

	void testValueProvider()
	{
		AutoPtr<MockRecord> mockCxtRecordPtr;
		RandomStream mockRandom;

		for (int i = 0; i < 1000; i++)
		{
			I16u x = random() % 10000;
			ConstValueProvider<I16u, MockRecord> valueProvider(x);

			for (int i = 0; i < 100; i++)
			{
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, valueProvider(mockCxtRecordPtr, mockRandom));
			}
		}

		for (int i = 0; i < 1000; i++)
		{
			I64u x = random() % 6431543129;
			ConstValueProvider<I64u, MockRecord> valueProvider(x);

			for (int i = 0; i < 100; i++)
			{
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, valueProvider(mockCxtRecordPtr, mockRandom));
			}
		}

		for (int i = 0; i < 1000; i++)
		{
			Decimal x = (random() % 6431543129)/6431543129.0;
			ConstValueProvider<Decimal, MockRecord> valueProvider(x);

			for (int i = 0; i < 100; i++)
			{
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, valueProvider(mockCxtRecordPtr, mockRandom));
			}
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("ConstValueProviderTest");
		suite->addTest(new TestCaller<ConstValueProviderTest> ("testValueProvider", &ConstValueProviderTest::testValueProvider));
		return suite;
	}

private:

	IntervalMap<ID, I16u>* _intervalMapPtr;
};

class MockRecord : public Record
{
};

} // namespace Myriad

#endif /* CONSTVALUEPROVIDERTEST_H_ */
