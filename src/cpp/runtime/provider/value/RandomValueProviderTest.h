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

#ifndef RANDOMVALUEPROVIDERTEST_H_
#define RANDOMVALUEPROVIDERTEST_H_

#include "record/mock/MockRecordA.h"
#include "runtime/provider/value/RandomValueProvider.h"
#include "runtime/setter/FieldSetter.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// unit tests
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class RandomValueProviderTest: public TestFixture
{
public:

    typedef RecordTraits<MockRecordA> RecordTraitsType;
    typedef RecordTraitsType::MetaType RecordMetaType;
    typedef RecordTraitsType::FactoryType RecordFactoryType;

    typedef CombinedPrFunction<I16u> PrFunctionType;
    typedef CombinedPrFunctionInput<I16u> PrFunctionInputType;

    typedef RandomValueProvider<I16u, MockRecordA, PrFunctionType> MockField1ValueProvider;
    typedef FieldSetter<MockRecordA, RecordTraits<MockRecordA>::MOCK_FIELD_1, MockField1ValueProvider> MockField1Setter;

    void setUp()
    {
        srand(54352364);
    }

    void testValueProviderFunctor()
    {
        I64u scale = 4;
        I64u min = 0;
        I64u max = power(10, scale);

        RecordMetaType recordMeta(4096);
        RecordFactoryType recordFactory(recordMeta);

        AutoPtr<MockRecordA> mockCxtRecordPtr = recordFactory();
        RandomStream mockRandom;

        CombinedPrFunctionInputFactory<I16u> functionInputFactory;
        AutoPtr<PrFunctionInputType> prFunctionInput(functionInputFactory.getFunction<0>());
        AutoPtr<PrFunctionType> prFunction(new PrFunctionType(prFunctionInput->serialize()));

        MockField1ValueProvider mockField1ValueProvider(*prFunction.get());

        Interval<I16u> valueDomain = prFunctionInput->domain();

        // a vector for the computed absolute frequencies
        vector<I64u> absoluteFrequencies((valueDomain.length()+1), 0);

        for (I64u i = min; i < max; i++)
        {
            // grab mockField1 value
            I16u value = mockField1ValueProvider(mockCxtRecordPtr, mockRandom);

            // check range
            CPPUNIT_ASSERT_MESSAGE(format("Value %hu at position %Lu out of range", value, i), (value >= valueDomain.min() && value < valueDomain.max()) || (value == nullValue<I16u>()));

            // update absolute frequency count
            if (value == nullValue<I16u>())
            {
                absoluteFrequencies[valueDomain.length()]++;
            }
            else
            {
                absoluteFrequencies[value - valueDomain.min()]++;
            }
        }

        // check frequencies
        for (I16u X = valueDomain.min(); X < valueDomain.max() + 1U; X++)
        {
            double pdfXExp = (X < valueDomain.max()) ? prFunctionInput->pdf(X) : prFunctionInput->pdf(nullValue<I16u>());
            double pdfXAct = absoluteFrequencies[X - valueDomain.min()]/static_cast<double>(max-min);

            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Relative frequencies do not match", pdfXExp, pdfXAct, 5 * pow(10.0, -static_cast<double>(scale/2)));
        }
    }

    void testFieldValueRange()
    {
        RecordMetaType recordMeta(4096);
        RecordFactoryType recordFactory(recordMeta);

        AutoPtr<MockRecordA> mockCxtRecordPtr = recordFactory();
        RandomStream mockRandom;

        CombinedPrFunctionInputFactory<I16u> functionInputFactory;
        AutoPtr<PrFunctionInputType> prFunctionInput(functionInputFactory.getFunction<0>());
        AutoPtr<PrFunctionType> prFunction(new PrFunctionType(prFunctionInput->serialize()));

        MockField1ValueProvider mockField1ValueProvider(*prFunction.get());

        for (int i = 0; i < 100; i++)
        {
            I16u x = random() % 10000;
            I16u y = x + 1;

            CPPUNIT_ASSERT_THROW_MESSAGE("valueRange() is not supported", mockField1ValueProvider.valueRange(x, mockCxtRecordPtr, mockRandom), RuntimeException);
            CPPUNIT_ASSERT_THROW_MESSAGE("valueRange() is not supported", mockField1ValueProvider.valueRange(y, mockCxtRecordPtr, mockRandom), RuntimeException);
        }
    }

    void testFieldSetterInstance()
    {
        I64u scale = 4;
        I64u min = 0;
        I64u max = power(10, scale);

        RecordMetaType recordMeta(4096);
        RecordFactoryType recordFactory(recordMeta);

        AutoPtr<MockRecordA> mockCxtRecordPtr = recordFactory();
        RandomStream mockRandom, mockRandomCopy;

        CombinedPrFunctionInputFactory<I16u> functionInputFactory;
        AutoPtr<PrFunctionInputType> prFunctionInput(functionInputFactory.getFunction<0>());
        AutoPtr<PrFunctionType> prFunction(new PrFunctionType(prFunctionInput->serialize()));

        MockField1ValueProvider mockField1ValueProvider(*prFunction.get());
        MockField1Setter mockField1Setter(mockField1ValueProvider);

        for (I64u i = min; i < max; i++)
        {
            mockCxtRecordPtr = recordFactory(i);

            I16u x = prFunction->sample(mockRandomCopy());
            mockField1Setter(mockCxtRecordPtr, mockRandom);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, mockCxtRecordPtr->mockField1());
        }
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("RandomValueProviderTest");
        suite->addTest(new TestCaller<RandomValueProviderTest> ("testValueProviderFunctor", &RandomValueProviderTest::testValueProviderFunctor));
        suite->addTest(new TestCaller<RandomValueProviderTest> ("testFieldValueRange", &RandomValueProviderTest::testFieldValueRange));
        suite->addTest(new TestCaller<RandomValueProviderTest> ("testFieldSetterInstance", &RandomValueProviderTest::testFieldSetterInstance));
        return suite;
    }
};

} // namespace Myriad

#endif /* RANDOMVALUEPROVIDERTEST_H_ */
