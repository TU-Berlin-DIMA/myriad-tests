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
        typedef pair<I16u, I16u> FieldsPairType;

        I64u scale = 4;
        I64u min = 0;
        I64u max = power(10, scale);

        RecordMetaType recordMeta(4096);
        RecordFactoryType recordFactory(recordMeta);

        AutoPtr<MockRecordA> mockCxtRecordPtr;
        RandomStream randomStream;

        CombinedPrFunctionInputFactory<I16u> functionInputFactory;
        AutoPtr<PrFunctionInputType> prFunctionInput = functionInputFactory.getFunction<0>();
        AutoPtr<PrFunctionType> prFunction(new PrFunctionType(prFunctionInput->serialize()));

        ConstRangeProvider<I64u, MockRecordA> constRangeProvider(min, max);

        ClusteredValueProvider<RecordFieldTraits<RecordTraitsType::MOCK_FIELD_1, MockRecordA>::FieldType, MockRecordA, PrFunctionType, ConstRangeProvider<I64u, MockRecordA> > clusteredValueProvider(*prFunction.get(), constRangeProvider);

        // test range parameters
        I16u valueDomainMin = prFunctionInput->domain().min();
        I16u valueDomainMax = prFunctionInput->domain().max();
        I32u valueDomainLength = valueDomainMax - valueDomainMin;

        FieldsPairType valueDomainPair(valueDomainMin, valueDomainMax);

        // a vector for the computed absolute frequencies
        vector<I64u> absoluteFrequencies(valueDomainLength+1, 0);
        // a vector of boolean flags for all seen value clusters
        vector<bool> clusterProcessed(valueDomainLength+1, false);

        I16u lastValue = nullValue<I16u>();
        for (I64u i = min; i < max; i++)
        {
            mockCxtRecordPtr = recordFactory(i);
            I16u currValue = clusteredValueProvider(mockCxtRecordPtr, randomStream);

            // check range
            CPPUNIT_ASSERT_MESSAGE("Value out of range", (currValue >= valueDomainMin && currValue < valueDomainMax) || (currValue == nullValue<I16u>()));

            // update absolute frequency count
            absoluteFrequencies[valueIndex(currValue, valueDomainPair)]++;

            // mark previous value cluster as processed
            if (lastValue != nullValue<I16u>() && lastValue != currValue)
            {
                CPPUNIT_ASSERT_MESSAGE(format("Value cluster for value %hu is already processed", currValue), !clusterProcessed[valueIndex(lastValue, valueDomainPair)]);
                clusterProcessed[valueIndex(lastValue, valueDomainPair)] = true;
            }

            // update last currValue
            lastValue = currValue;
        }

        // mark last value cluster as processed
        CPPUNIT_ASSERT_MESSAGE(format("Value cluster for value %hu is already processed", lastValue), !clusterProcessed[valueIndex(lastValue, valueDomainPair)]);
        clusterProcessed[valueIndex(lastValue, valueDomainPair)] = true;

        // check whether all value clusters are processed
        for (I16u X = valueDomainMin; X < valueDomainMax + 1U; X++)
        {
            if (absoluteFrequencies[valueIndex(X, valueDomainPair)] == 0)
            {
                continue;
            }

            CPPUNIT_ASSERT_MESSAGE(format("Cluster for value %hu is not processed", (X == valueDomainMax) ? X : nullValue<I16u>()), clusterProcessed[valueIndex(X, valueDomainPair)]);
        }

        // check frequencies
        for (I16u X = valueDomainMin; X < valueDomainMax + 1U; X++)
        {
            double pdfXExp = (X < valueDomainMax) ? prFunctionInput->pdf(X) : prFunctionInput->pdf(nullValue<I16u>());
            double pdfXAct = absoluteFrequencies[valueIndex(X, valueDomainPair)]/static_cast<double>(max-min);

            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Relative frequencies do not match", pdfXExp, pdfXAct, 5 * pow(10.0, -static_cast<double>(scale/2)));
        }
    }

    void testClusteredValueProviderWithContextFieldRangeProviderForCardinality()
    {
        // type shortcuts for this test
        typedef ConstRangeProvider<I64u, MockRecordA> MockField1RangeProviderType;
        typedef ClusteredValueProvider<RecordFieldTraits<RecordTraitsType::MOCK_FIELD_1, MockRecordA>::FieldType, MockRecordA, PrFunctionType, MockField1RangeProviderType> MockField1ValueProviderType;
        typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_1, MockField1ValueProviderType> MockField1FieldSetterType;

        typedef ContextFieldRangeProvider<I64u, MockRecordA, MockField1FieldSetterType> MockField4RangeProviderType;
        typedef ClusteredValueProvider<RecordFieldTraits<RecordTraitsType::MOCK_FIELD_4, MockRecordA>::FieldType, MockRecordA, PrFunctionType, MockField4RangeProviderType> MockField4ValueProviderType;
        typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_4, MockField4ValueProviderType> MockField4FieldSetterType;

        typedef pair<I16u, I16u> FieldsPairType;

        I64u scale = 4;
        I64u min = 0;
        I64u max = power(10, scale);

        RecordMetaType recordMeta(4096);
        RecordFactoryType recordFactory(recordMeta);

        AutoPtr<MockRecordA> mockCxtRecordPtr;
        RandomStream randomStream;

        CombinedPrFunctionInputFactory<I16u> functionInputFactory;
        AutoPtr<PrFunctionInputType> prFunctionInput = functionInputFactory.getFunction<1>();
        AutoPtr<PrFunctionType> prFunction(new PrFunctionType(prFunctionInput->serialize()));

        MockField1RangeProviderType mockField1RangeProvider(min, max);
        MockField1ValueProviderType mockField1ValueProvider(*prFunction.get(), mockField1RangeProvider);
        MockField1FieldSetterType mockField1FieldSetter(mockField1ValueProvider);

        MockField4RangeProviderType mockField4RangeProvider(mockField1FieldSetter);
        MockField4ValueProviderType mockField4ValueProvider(*prFunction.get(), mockField4RangeProvider);
        MockField4FieldSetterType mockField4FieldSetter(mockField4ValueProvider);

        // test range parameters
        I16u valueDomainMin = prFunctionInput->domain().min();
        I16u valueDomainMax = prFunctionInput->domain().max();
        I32u valueDomainLength = valueDomainMax - valueDomainMin;

        // a vector for the computed absolute frequencies
        vector<I64u> absoluteFrequencies((valueDomainLength+1)*(valueDomainLength+1), 0);
        // a vector of boolean flags for all seen value pair clusters
        vector<bool> clusterProcessed((valueDomainLength+1)*(valueDomainLength+1), false);

        FieldsPairType valueDomainPair(valueDomainMin, valueDomainMax);
        FieldsPairType nullValuePair(nullValue<I16u>(), nullValue<I16u>());
        FieldsPairType lastValuePair(nullValuePair);

//        std::cout << "test range is " << valueDomainMin << ", " << valueDomainMax << std::endl;
//        std::cout << "test range index length is " << (valueDomainLength+1)*(valueDomainLength+1) << std::endl;
//
//        for (I64u i = valueDomainMin; i <= valueDomainMax; i++)
//        {
//            for (I64u j = valueDomainMin; j <= valueDomainMax; j++)
//            {
//                FieldsPairType pair((i < valueDomainMax) ? i : nullValue<I16u>(), (j < valueDomainMax) ? j : nullValue<I16u>());
//                std::cout << "index for value pair (" << pair.first << ", " << pair.second << ") is " << valuePairIndex(pair, valueDomainPair) << '\n';
//            }
//            std::cout << std::endl;
//        }

        for (I64u i = min; i < max; i++)
        {
            // create a new record
            mockCxtRecordPtr = recordFactory(i);

            // set mockField1 value
            mockField1FieldSetter(mockCxtRecordPtr, randomStream);
            // grab mockField1 value
            I16u currMockField1Value = mockCxtRecordPtr->mockField1();

            // set mockField4 value
            mockField4FieldSetter(mockCxtRecordPtr, randomStream);
            // grab mockField4 value
            I16u currMockField4Value = mockCxtRecordPtr->mockField4();

            // create the curr value pair
            FieldsPairType currValuePair(currMockField1Value, currMockField4Value);

//            std::cout << format("record[#%06Lu] = (%hu, %hu)", i, currValuePair.first, currValuePair.second) << std::endl;

            // check range
            CPPUNIT_ASSERT_MESSAGE(format("Value %hu at position %Lu out of range", currValuePair.second, i), (currMockField4Value >= valueDomainMin && currMockField4Value < valueDomainMax) || (currMockField4Value == nullValue<I16u>()));

            // update absolute frequency count
            absoluteFrequencies[valuePairIndex(currValuePair, valueDomainPair)]++;

            // mark previous value cluster as processed
            if (lastValuePair != nullValuePair && lastValuePair != currValuePair)
            {
                CPPUNIT_ASSERT_MESSAGE(format("Value cluster for value pair (%hu, %hu) at index %z for genID = %Lu is already processed", currValuePair.first, currValuePair.second, valuePairIndex(lastValuePair, valueDomainPair), i), !clusterProcessed[valuePairIndex(lastValuePair, valueDomainPair)]);
                clusterProcessed[valuePairIndex(lastValuePair, valueDomainPair)] = true;
            }
            else if (currValuePair == nullValuePair && i == max - 1)
            {
                CPPUNIT_ASSERT_MESSAGE(format("Value cluster for null value pair is already processed", currValuePair.first, currValuePair.second), !clusterProcessed[valuePairIndex(lastValuePair, valueDomainPair)]);
                clusterProcessed[valuePairIndex(currValuePair, valueDomainPair)] = true;
            }

            // update last currValue
            lastValuePair = currValuePair;
        }

        // check whether all value clusters are processed
        for (I16u X = valueDomainMin; X <= valueDomainMax; X++)
        {
            for (I16u Y = valueDomainMin; Y <= valueDomainMax; Y++)
            {
                FieldsPairType pair((X < valueDomainMax) ? X : nullValue<I16u>(), (Y < valueDomainMax) ? Y : nullValue<I16u>());
                if (absoluteFrequencies[valuePairIndex(pair, valueDomainPair)] == 0)
                {
                    continue;
                }

                CPPUNIT_ASSERT_MESSAGE(format("Cluster for value pair (%hu, %hu) is not processed", pair.first, pair.second), clusterProcessed[valuePairIndex(pair, valueDomainPair)]);
            }
        }

        // check frequencies
        for (I16u X = valueDomainMin; X <= valueDomainMax; X++)
        {
            for (I16u Y = valueDomainMin; Y <= valueDomainMax; Y++)
            {
                FieldsPairType pair((X < valueDomainMax) ? X : nullValue<I16u>(), (Y < valueDomainMax) ? Y : nullValue<I16u>());

                double pdfXExp = prFunctionInput->pdf(pair.first) * prFunctionInput->pdf(pair.second);
                double pdfXAct = absoluteFrequencies[valuePairIndex(pair, valueDomainPair)]/static_cast<double>(max-min);

                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Relative frequencies do not match", pdfXExp, pdfXAct, 5 * pow(10.0, -static_cast<double>(scale/2)));
            }
        }
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("ClusteredValueProviderTest");
        suite->addTest(new TestCaller<ClusteredValueProviderTest> ("testClusteredValueProviderWithConstRangeProviderForCardinality", &ClusteredValueProviderTest::testClusteredValueProviderWithConstRangeProviderForCardinality));
        suite->addTest(new TestCaller<ClusteredValueProviderTest> ("testClusteredValueProviderWithContextFieldRangeProviderForCardinality", &ClusteredValueProviderTest::testClusteredValueProviderWithContextFieldRangeProviderForCardinality));
        return suite;
    }

private:

    template<typename T> static size_t valuePairIndex(const std::pair<T, T> pair, const std::pair<T, T> valueRangePair)
    {
        size_t i, j, s;

        s = static_cast<size_t>(valueRangePair.second - valueRangePair.first + 1);
        i = (nullValue<T>() != pair.first) ? (pair.first - valueRangePair.first) : s-1;
        j = (nullValue<T>() != pair.second) ? (pair.second - valueRangePair.first) : s-1;

        return i * s + j;
    }

    template<typename T> static size_t valueIndex(const T value, const std::pair<T, T> valueRangePair)
    {
        size_t s = static_cast<size_t>(valueRangePair.second - valueRangePair.first + 1);
        return (nullValue<T>() != value) ? (value - valueRangePair.first) : s-1;
    }
};

} // namespace Myriad

#endif /* CLUSTEREDVALUEPROVIDERTEST_H_ */
