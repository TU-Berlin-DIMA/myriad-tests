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

#ifndef CONSTRANGEPROVIDERTEST_H_
#define CONSTRANGEPROVIDERTEST_H_

#include "record/mock/MockRecordA.h"
#include "runtime/provider/range/ConstRangeProvider.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

class ConstRangeProviderTest: public TestFixture
{
public:

    void setUp()
    {
        srand(54352364);
    }

    void testRangeProviderFunctor()
    {
        AutoPtr<MockRecordA> mockCxtRecordPtr;

        for (int i = 0; i < 1000; i++)
        {
        	Interval<I16u> x(random() % 10000, random() % 10000);
            ConstRangeProvider<I16u, MockRecordA> rangeProvider(x);

            for (int i = 0; i < 100; i++)
            {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Ranges don't match", x, rangeProvider(mockCxtRecordPtr));
            }
        }

        for (int i = 0; i < 1000; i++)
        {
            Interval<I64u> x(random() % 6431543129, random() % 6431543129);
            ConstRangeProvider<I64u, MockRecordA> rangeProvider(x.min(), x.max());

            for (int i = 0; i < 100; i++)
            {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Ranges don't match", x, rangeProvider(mockCxtRecordPtr));
            }
        }

        for (int i = 0; i < 1000; i++)
        {
            Interval<Decimal> x((random() % 6431543129)/6431543129.0, (random() % 6431543129)/6431543129.0);
            ConstRangeProvider<Decimal, MockRecordA> rangeProvider(x);

            for (int i = 0; i < 100; i++)
            {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Ranges don't match", x, rangeProvider(mockCxtRecordPtr));
            }
        }
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("ConstRangeProviderTest");
        suite->addTest(new TestCaller<ConstRangeProviderTest> ("testRangeProvider", &ConstRangeProviderTest::testRangeProviderFunctor));
        return suite;
    }
};

} // namespace Myriad

#endif /* CONSTRANGEPROVIDERTEST_H_ */
