/*
 * Copyright 2010-2011 DIMA Research Group, TU Berlin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: Alexander Alexandrov <alexander.alexandrov@tu-berlin.de>
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

class ConstValueProviderMockRecord;

class ConstValueProviderTest: public TestFixture
{
public:

	void setUp()
	{
		srand(54352364);
	}

	void testValueProvider()
	{
		AutoPtr<ConstValueProviderMockRecord> mockCxtRecordPtr;
		RandomStream mockRandom;

		for (int i = 0; i < 1000; i++)
		{
			I16u x = random() % 10000;
			ConstValueProvider<I16u, ConstValueProviderMockRecord> valueProvider(x);

			for (int i = 0; i < 100; i++)
			{
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, valueProvider(mockCxtRecordPtr, mockRandom));
			}
		}

		for (int i = 0; i < 1000; i++)
		{
			I64u x = random() % 6431543129;
			ConstValueProvider<I64u, ConstValueProviderMockRecord> valueProvider(x);

			for (int i = 0; i < 100; i++)
			{
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, valueProvider(mockCxtRecordPtr, mockRandom));
			}
		}

		for (int i = 0; i < 1000; i++)
		{
			Decimal x = (random() % 6431543129)/6431543129.0;
			ConstValueProvider<Decimal, ConstValueProviderMockRecord> valueProvider(x);

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

class ConstValueProviderMockRecord : public Record
{
};

} // namespace Myriad

#endif /* CONSTVALUEPROVIDERTEST_H_ */
