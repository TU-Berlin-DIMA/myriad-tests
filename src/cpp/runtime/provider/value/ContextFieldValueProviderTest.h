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

#ifndef CONTEXTFIELDVALUEPROVIDERTEST_H_
#define CONTEXTFIELDVALUEPROVIDERTEST_H_

#include "record/mock/MockRecordA.h"
#include "runtime/provider/value/ContextFieldValueProvider.h"
#include "runtime/setter/FieldSetter.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// unit tests
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class ContextFieldValueProviderTest: public TestFixture
{
public:

	typedef RecordTraits<MockRecordA> RecordTraitsType;
	typedef RecordTraitsType::MetaType RecordMetaType;
	typedef RecordTraitsType::FactoryType RecordFactoryType;
	typedef ContextFieldValueProvider<I16u, MockRecordA, RecordTraitsType::MOCK_FIELD_1> MockField1ValueProvider;
	typedef ContextFieldValueProvider<I64u, MockRecordA, RecordTraitsType::MOCK_FIELD_2> MockField2ValueProvider;
	typedef ContextFieldValueProvider<Decimal, MockRecordA, RecordTraitsType::MOCK_FIELD_3> MockField3ValueProvider;
	typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_4, MockField1ValueProvider> MockField4Setter;

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
        MockField1ValueProvider mockField1ValueProvider;
        MockField2ValueProvider mockField2ValueProvider;
        MockField3ValueProvider mockField3ValueProvider;

        for (int i = 0; i < 1000; i++)
        {
            I16u x = random() % 10000;

            for (int i = 0; i < 100; i++)
            {
            	mockCxtRecordPtr->mockField1(x);
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, mockField1ValueProvider(mockCxtRecordPtr, mockRandom));
            }
        }

        for (int i = 0; i < 1000; i++)
        {
            I64u x = random() % 6431543129;


            for (int i = 0; i < 100; i++)
            {
            	mockCxtRecordPtr->mockField2(x);
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, mockField2ValueProvider(mockCxtRecordPtr, mockRandom));
            }
        }

        for (int i = 0; i < 1000; i++)
        {
            Decimal x = (random() % 6431543129)/6431543129.0;
            for (int i = 0; i < 100; i++)
            {
            	mockCxtRecordPtr->mockField3(x);
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, mockField3ValueProvider(mockCxtRecordPtr, mockRandom));
            }
        }
    }

    void testFieldValueRange()
    {
		RecordMetaType recordMeta(4096);
		RecordFactoryType recordFactory(recordMeta);

		AutoPtr<MockRecordA> mockCxtRecordPtr = recordFactory();
		RandomStream mockRandom;
		MockField1ValueProvider mockField1ValueProvider;
		MockField2ValueProvider mockField2ValueProvider;

		for (int i = 0; i < 100; i++)
		{
			I16u x = random() % 10000;
			I16u y = x + 1;

			CPPUNIT_ASSERT_THROW_MESSAGE("fieldValueRange() is not supported", mockField1ValueProvider.fieldValueRange(x, mockCxtRecordPtr, mockRandom), RuntimeException);
			CPPUNIT_ASSERT_THROW_MESSAGE("fieldValueRange() is not supported", mockField1ValueProvider.fieldValueRange(y, mockCxtRecordPtr, mockRandom), RuntimeException);
		}

		for (int i = 0; i < 100; i++)
		{
			I64u x = random() % 6431543129;
			I64u y = x + 1;

			CPPUNIT_ASSERT_THROW_MESSAGE("fieldValueRange() is not supported", mockField2ValueProvider.fieldValueRange(x, mockCxtRecordPtr, mockRandom), RuntimeException);
			CPPUNIT_ASSERT_THROW_MESSAGE("fieldValueRange() is not supported", mockField2ValueProvider.fieldValueRange(y, mockCxtRecordPtr, mockRandom), RuntimeException);
		}
    }

    void testFieldSetterInstance()
    {
    	RecordMetaType recordMeta(4096);
    	RecordFactoryType recordFactory(recordMeta);

        AutoPtr<MockRecordA> mockCxtRecordPtr = recordFactory();
        RandomStream mockRandom;
        MockField1ValueProvider mockField1Provider;
        MockField4Setter mockField4Setter(mockField1Provider);

		for (int i = 0; i < 1000; i++)
		{
			I16u x = random() % 10000;

			for (int i = 0; i < 100; i++)
			{
				mockCxtRecordPtr->mockField1(x);
				mockField4Setter(mockCxtRecordPtr, mockRandom);
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", x, mockCxtRecordPtr->mockField4());
			}
		}
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("ContextFieldValueProviderTest");
        suite->addTest(new TestCaller<ContextFieldValueProviderTest> ("testValueProviderFunctor", &ContextFieldValueProviderTest::testValueProviderFunctor));
        suite->addTest(new TestCaller<ContextFieldValueProviderTest> ("testFieldValueRange", &ContextFieldValueProviderTest::testFieldValueRange));
        suite->addTest(new TestCaller<ContextFieldValueProviderTest> ("testFieldSetterInstance", &ContextFieldValueProviderTest::testFieldSetterInstance));
        return suite;
    }
};

} // namespace Myriad

#endif /* CONTEXTFIELDVALUEPROVIDERTEST_H_ */
