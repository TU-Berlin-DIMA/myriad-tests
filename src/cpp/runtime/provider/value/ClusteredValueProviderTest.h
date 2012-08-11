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

#include "math/probability/util/CombinedPrFunctionInput.h"
#include "record/mock/MockRecordA.h"
#include "runtime/provider/value/ClusteredValueProvider.h"
#include "runtime/provider/value/ContextFieldValueProvider.h"

#include <bits/stream_iterator.h>

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

    typedef RecordTraits<MockRecordA> RecordTraitsType;
    typedef RecordTraitsType::MetaType RecordMetaType;
    typedef RecordTraitsType::FactoryType RecordFactoryType;
    typedef ConstValueProvider<I16u, MockRecordA> I16uConstValueProvider;
    typedef ConstValueProvider<I64u, MockRecordA> I64uConstValueProvider;
    typedef ConstValueProvider<Decimal, MockRecordA> DecimalConstValueProvider;
    typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_1, I16uConstValueProvider> MockField1Setter;
    typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_2, I64uConstValueProvider> MockField2Setter;
    typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_3, DecimalConstValueProvider> MockField3Setter;
    typedef CombinedPrFunction<I16u> PrFunctionType;
    typedef CombinedPrFunctionInput<I16u> PrFunctionInputType;

    void setUp()
    {
        srand(54352364);
    }

    void testClusteredValueProviderWithConstRangeProviderForCardinality()
    {
        RecordMetaType recordMeta(4096);
        RecordFactoryType recordFactory(recordMeta);

        AutoPtr<MockRecordA> mockCxtRecordPtr;
        RandomStream mockRandom;

        PrFunctionInputType* prFunctionInput = CombinedPrFunctionInput<I16u>::factory();
        PrFunctionType* prFunction(new PrFunctionType(prFunctionInput->serialize()));

        I64u scale = 4;

        I64u min = 0;
        I64u max = power(10, scale);
        ConstRangeProvider<I64u, MockRecordA> constRangeProvider(min, max);

        ClusteredValueProvider<RecordFieldTraits<RecordTraitsType::MOCK_FIELD_1, MockRecordA>::FieldType, MockRecordA, PrFunctionType, ConstRangeProvider<I64u, MockRecordA> > clusteredValueProvider(*prFunction, constRangeProvider);

        // test range parameters
        I16u testRangeMin = prFunctionInput->testRangeMin();
        I16u testRangeMax = prFunctionInput->testRangeMax();
        I32u testRangeLength = testRangeMax - testRangeMin;

        // a vector for the computed absolute frequencies
        vector<I64u> absoluteFrequencies(testRangeLength+1, 0);
        vector<bool> clusterProcessed(testRangeLength+1, false);

        I16u lastValue = nullValue<I16u>();
        for (I64u i = min; i < max; i++)
        {
            mockCxtRecordPtr = recordFactory(i);
            I16u currValue = clusteredValueProvider(mockCxtRecordPtr, mockRandom);

            // check range
            CPPUNIT_ASSERT_MESSAGE("Value out of range", (currValue >= testRangeMin && currValue < testRangeMax) || (currValue == nullValue<I16u>()));

            // update absolute frequency count
            if (currValue == nullValue<I16u>())
            {
                absoluteFrequencies[testRangeLength]++;
            }
            else
            {
                absoluteFrequencies[currValue - testRangeMin]++;
            }

            // mark previous value cluster as processed
            if (lastValue != nullValue<I16u>() && lastValue != currValue)
            {
                CPPUNIT_ASSERT_MESSAGE(format("Value cluster for value %hu is already processed", currValue), !clusterProcessed[lastValue - testRangeMin]);
                clusterProcessed[lastValue - testRangeMin] = true;
            }
            else if (currValue == nullValue<I16u>() && i == max - 1)
            {
                CPPUNIT_ASSERT_MESSAGE(format("Value cluster for value %hu is already processed", currValue), !clusterProcessed[lastValue - testRangeMin]);
                clusterProcessed[testRangeMax] = true;
            }

            // update last currValue
            lastValue = currValue;
        }

        // check whether all value clusters are processed
        for (I16u X = testRangeMin; X < testRangeMax + 1U; X++)
        {
            if (absoluteFrequencies[X - testRangeMin] == 0)
            {
                continue;
            }

            CPPUNIT_ASSERT_MESSAGE(format("Cluster for value %hu is not processed", (X == testRangeMax) ? X : nullValue<I16u>()), clusterProcessed[X - testRangeMin]);
        }

        // check frequencies
        for (I16u X = testRangeMin; X < testRangeMax + 1U; X++)
        {
            double pdfXExp = (X < testRangeMax) ? prFunctionInput->pdf(X) : prFunctionInput->pdf(nullValue<I16u>());
            double pdfXAct = absoluteFrequencies[X - testRangeMin]/static_cast<double>(max-min);

            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Relative frequencies do not match", pdfXExp, pdfXAct, 5 * pow(10.0, -static_cast<double>(scale/2)));
        }
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
