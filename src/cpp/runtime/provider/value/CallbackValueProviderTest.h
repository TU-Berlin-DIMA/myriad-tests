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

#ifndef CALLBACKVALUEPROVIDERTEST_H_
#define CALLBACKVALUEPROVIDERTEST_H_

#include "record/Record.h"
#include "runtime/provider/value/CallbackValueProvider.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

class CallbackValueProviderMockRecord : public Record
{
};

class CallbackValueProviderMockCallback: public Record
{
public:

    CallbackValueProviderMockCallback()
    {
    }

    const I16u callback(const AutoPtr<CallbackValueProviderMockRecord>& ctxRecordPtr, RandomStream& random)
    {
        return random(1, 1000);
    }
};

class CallbackValueProviderTest: public TestFixture
{
public:

    void setUp()
    {
        srand(54352364);
    }

    void testValueProvider()
    {
        RandomStream::Seed seed("341235143");

        AutoPtr<CallbackValueProviderMockRecord> mockCxtRecordPtr;
        RandomStream mockRandom1, mockRandom2;
        CallbackValueProviderMockCallback callbackObject;

        mockRandom1.seed(seed);
        mockRandom2.seed(seed);

        CallbackValueProvider<I16u, CallbackValueProviderMockRecord, CallbackValueProviderMockCallback> valueProvider(callbackObject, &CallbackValueProviderMockCallback::callback, 0);

        for(int i = 0; i < 1000; i++)
        {
            I16u x = static_cast<I16u>(mockRandom1(1, 1000));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, valueProvider(mockCxtRecordPtr, mockRandom2));
        }
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("CallbackValueProviderTest");
        suite->addTest(new TestCaller<CallbackValueProviderTest> ("testValueProvider", &CallbackValueProviderTest::testValueProvider));
        return suite;
    }
};

} // namespace Myriad

#endif /* CALLBACKVALUEPROVIDERTEST_H_ */
