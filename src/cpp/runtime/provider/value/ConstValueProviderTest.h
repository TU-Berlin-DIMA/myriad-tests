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

#include "record/mock/MockRecordA.h"
#include "runtime/provider/value/ConstValueProvider.h"
#include "runtime/setter/FieldSetter.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// unit tests
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class ConstValueProviderTest: public TestFixture
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

    void setUp()
    {
        srand(54352364);
    }

    void testValueProviderFunctor()
    {
    	RecordMetaType recordMeta(4096);
    	RecordFactoryType recordFactory(recordMeta);

        AutoPtr<MockRecordA> mockCxtRecordPtr = recordFactory();
        RandomStream mockRandom;

        for (int i = 0; i < 1000; i++)
        {
            I16u x = random() % 10000;
            I16uConstValueProvider valueProvider(x);

            for (int i = 0; i < 100; i++)
            {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, valueProvider(mockCxtRecordPtr, mockRandom));
            }
        }

        for (int i = 0; i < 1000; i++)
        {
            I64u x = random() % 6431543129;
            I64uConstValueProvider valueProvider(x);

            for (int i = 0; i < 100; i++)
            {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, valueProvider(mockCxtRecordPtr, mockRandom));
            }
        }

        for (int i = 0; i < 1000; i++)
        {
            Decimal x = (random() % 6431543129)/6431543129.0;
            DecimalConstValueProvider valueProvider(x);

            for (int i = 0; i < 100; i++)
            {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, valueProvider(mockCxtRecordPtr, mockRandom));
            }
        }
    }

    void testFieldValueRange()
    {
        for (int j = 0; j < 100; j++)
        {
			I64u sequenceCardinality = random() % 4096;
			RecordMetaType recordMeta(sequenceCardinality);
			RecordFactoryType recordFactory(recordMeta);

			AutoPtr<MockRecordA> mockCxtRecordPtr = recordFactory();

	        for (int i = 0; i < 100; i++)
	        {
	            I16u x = random() % 10000;
	            I16u y = x + 1;
	            I16uConstValueProvider valueProvider(x);

	            for (int i = 0; i < 100; i++)
	            {
	                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", Interval<I64u>(0, sequenceCardinality), valueProvider.valueRange(x, mockCxtRecordPtr));
	                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", Interval<I64u>(0, 0), valueProvider.valueRange(y, mockCxtRecordPtr));
	            }
	        }

	        for (int i = 0; i < 100; i++)
	        {
	            I64u x = random() % 6431543129;
	            I64u y = x + 1;
	            I64uConstValueProvider valueProvider(x);

	            for (int i = 0; i < 100; i++)
	            {
	                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", Interval<I64u>(0, sequenceCardinality), valueProvider.valueRange(x, mockCxtRecordPtr));
	                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", Interval<I64u>(0, 0), valueProvider.valueRange(y, mockCxtRecordPtr));
	            }
	        }
		}
    }

    void testFieldSetterInstance()
    {
    	RecordMetaType recordMeta(4096);
    	RecordFactoryType recordFactory(recordMeta);

        AutoPtr<MockRecordA> mockCxtRecordPtr = recordFactory();
        RandomStream mockRandom;

        for (int j = 0; j < 10; j++)
        {
            for (int i = 0; i < 1000; i++)
            {
                I16u x = random() % 10000;
                I16uConstValueProvider valueProvider(x);
                MockField1Setter fieldSetter(valueProvider);

                for (int i = 0; i < 100; i++)
                {
                	fieldSetter(mockCxtRecordPtr, mockRandom);
                    CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, mockCxtRecordPtr->mockField1());
                }
            }

            for (int i = 0; i < 1000; i++)
            {
                I64u x = random() % 6431543129;
                I64uConstValueProvider valueProvider(x);
                MockField2Setter fieldSetter(valueProvider);

                for (int i = 0; i < 100; i++)
                {
                	fieldSetter(mockCxtRecordPtr, mockRandom);
                    CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, mockCxtRecordPtr->mockField2());
                }
            }

            for (int i = 0; i < 1000; i++)
            {
                Decimal x = (random() % 6431543129)/6431543129.0;
                DecimalConstValueProvider valueProvider(x);
                MockField3Setter fieldSetter(valueProvider);

                for (int i = 0; i < 100; i++)
                {
                	fieldSetter(mockCxtRecordPtr, mockRandom);
                    CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, mockCxtRecordPtr->mockField3());
                }
            }
        }
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("ConstValueProviderTest");
        suite->addTest(new TestCaller<ConstValueProviderTest> ("testValueProviderFunctor", &ConstValueProviderTest::testValueProviderFunctor));
        suite->addTest(new TestCaller<ConstValueProviderTest> ("testFieldValueRange", &ConstValueProviderTest::testFieldValueRange));
        suite->addTest(new TestCaller<ConstValueProviderTest> ("testFieldSetterInstance", &ConstValueProviderTest::testFieldSetterInstance));
        return suite;
    }
};

} // namespace Myriad

#endif /* CONSTVALUEPROVIDERTEST_H_ */
