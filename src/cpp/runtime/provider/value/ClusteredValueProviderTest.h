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

#ifndef CLUSTEREDVALUEPROVIDERTEST_H_
#define CLUSTEREDVALUEPROVIDERTEST_H_

#include "record/mock/MockRecordA.h"
#include "runtime/provider/value/ClusteredValueProvider.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// unit tests
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class ClusteredValueProviderTest: public TestFixture
{
public:

    void setUp()
    {
        srand(54352364);
    }

    void testClusteredValueProviderWithConstRangeProviderForCardinality()
    {
        AutoPtr<MockRecordA> mockCxtRecordPtr;
        RandomStream mockRandom;

        // TODO: implement
    }

    void testClusteredValueProviderWithUnsupportedRangeProviderForCardinality()
    {
        AutoPtr<MockRecordA> mockCxtRecordPtr;
        RandomStream mockRandom;

        // TODO: implement
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("ClusteredValueProviderTest");
        suite->addTest(new TestCaller<ClusteredValueProviderTest> ("testClusteredValueProviderWithConstRangeProviderForCardinality", &ClusteredValueProviderTest::testClusteredValueProviderWithConstRangeProviderForCardinality));
        suite->addTest(new TestCaller<ClusteredValueProviderTest> ("testClusteredValueProviderWithUnsupportedRangeProviderForCardinality", &ClusteredValueProviderTest::testClusteredValueProviderWithUnsupportedRangeProviderForCardinality));
        return suite;
    }
};

} // namespace Myriad

#endif /* CLUSTEREDVALUEPROVIDERTEST_H_ */
