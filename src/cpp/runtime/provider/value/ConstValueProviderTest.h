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
#include "runtime/setter/FieldSetter.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// mock record traits
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class ConstValueProviderMockRecord;

template<>
struct RecordTraits<ConstValueProviderMockRecord>
{
	typedef RecordMeta<ConstValueProviderMockRecord> MetaType;
	typedef RecordFactory<ConstValueProviderMockRecord> FactoryType;

	enum Field { UNKNOWN, GEN_ID, MOCK_FIELD_1, MOCK_FIELD_2, MOCK_FIELD_3 };
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// mock record
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class ConstValueProviderMockRecord : public Record
{
public:

	typedef RecordTraits<ConstValueProviderMockRecord>::MetaType RecordMetaType;

	ConstValueProviderMockRecord(const RecordMetaType& meta) :
		_meta(meta)
	{
	}

	const RecordMetaType& meta() const
	{
		return _meta;
	}

	inline void mockField1(const I16u& v)
	{
		_mock_field_1 = v;
	}

	inline const I16u& mockField1() const
	{
		return _mock_field_1;
	}

	inline void mockField2(const I64u& v)
	{
		_mock_field_2 = v;
	}

	inline const I64u& mockField2() const
	{
		return _mock_field_2;
	}

	inline void mockField3(const Decimal& v)
	{
		_mock_field_3 = v;
	}

	inline const Decimal& mockField3() const
	{
		return _mock_field_3;
	}

private:

	// meta reference
	const RecordMetaType& _meta;

	// fields
	I16u _mock_field_1;
	I64u _mock_field_2;
	Decimal _mock_field_3;
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// mock record field inspection structures
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

// mock_field_1
template<>
struct RecordFieldTraits<RecordTraits<ConstValueProviderMockRecord>::MOCK_FIELD_1, ConstValueProviderMockRecord>
{
    typedef I16u FieldType;
    // record field getter / setter types
    typedef typename MethodTraits<ConstValueProviderMockRecord, FieldType>::Setter FieldSetterType;
    typedef typename MethodTraits<ConstValueProviderMockRecord, FieldType>::Getter FieldGetterType;

    static inline FieldSetterType setter()
    {
        return static_cast<FieldSetterType>(&ConstValueProviderMockRecord::mockField1);
    }

    static inline FieldGetterType getter()
    {
        return static_cast<FieldGetterType>(&ConstValueProviderMockRecord::mockField1);
    }
};

// mock_field_2
template<>
struct RecordFieldTraits<RecordTraits<ConstValueProviderMockRecord>::MOCK_FIELD_2, ConstValueProviderMockRecord>
{
    typedef I64u FieldType;
    // record field getter / setter types
    typedef typename MethodTraits<ConstValueProviderMockRecord, FieldType>::Setter FieldSetterType;
    typedef typename MethodTraits<ConstValueProviderMockRecord, FieldType>::Getter FieldGetterType;

    static inline FieldSetterType setter()
    {
        return static_cast<FieldSetterType>(&ConstValueProviderMockRecord::mockField2);
    }

    static inline FieldGetterType getter()
    {
        return static_cast<FieldGetterType>(&ConstValueProviderMockRecord::mockField2);
    }
};

// mock_field_3
template<>
struct RecordFieldTraits<RecordTraits<ConstValueProviderMockRecord>::MOCK_FIELD_3, ConstValueProviderMockRecord>
{
    typedef Decimal FieldType;
    // record field getter / setter types
    typedef typename MethodTraits<ConstValueProviderMockRecord, FieldType>::Setter FieldSetterType;
    typedef typename MethodTraits<ConstValueProviderMockRecord, FieldType>::Getter FieldGetterType;

    static inline FieldSetterType setter()
    {
        return static_cast<FieldSetterType>(&ConstValueProviderMockRecord::mockField3);
    }

    static inline FieldGetterType getter()
    {
        return static_cast<FieldGetterType>(&ConstValueProviderMockRecord::mockField3);
    }
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// unit tests
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class ConstValueProviderTest: public TestFixture
{
public:

	typedef RecordTraits<ConstValueProviderMockRecord> RecordTraitsType;
	typedef RecordTraitsType::MetaType RecordMetaType;
	typedef RecordTraitsType::FactoryType RecordFactoryType;
	typedef ConstValueProvider<I16u, ConstValueProviderMockRecord> I16uConstValueProvider;
	typedef ConstValueProvider<I64u, ConstValueProviderMockRecord> I64uConstValueProvider;
	typedef ConstValueProvider<Decimal, ConstValueProviderMockRecord> DecimalConstValueProvider;

    void setUp()
    {
        srand(54352364);
    }

    void testValueProviderFunctor()
    {
    	RecordMetaType recordMeta(4096);
    	RecordFactoryType recordFactory(recordMeta);

        AutoPtr<ConstValueProviderMockRecord> mockCxtRecordPtr = recordFactory();
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

			AutoPtr<ConstValueProviderMockRecord> mockCxtRecordPtr = recordFactory();
			RandomStream mockRandom;

	        for (int i = 0; i < 100; i++)
	        {
	            I16u x = random() % 10000;
	            I16u y = x + 1;
	            I16uConstValueProvider valueProvider(x);

	            for (int i = 0; i < 100; i++)
	            {
	                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", Interval<I64u>(0, sequenceCardinality), valueProvider.fieldValueRange(x, mockCxtRecordPtr));
	                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", Interval<I64u>(0, 0), valueProvider.fieldValueRange(y, mockCxtRecordPtr));
	            }
	        }

	        for (int i = 0; i < 100; i++)
	        {
	            I64u x = random() % 6431543129;
	            I64u y = x + 1;
	            I64uConstValueProvider valueProvider(x);

	            for (int i = 0; i < 100; i++)
	            {
	                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", Interval<I64u>(0, sequenceCardinality), valueProvider.fieldValueRange(x, mockCxtRecordPtr));
	                CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", Interval<I64u>(0, 0), valueProvider.fieldValueRange(y, mockCxtRecordPtr));
	            }
	        }
		}
    }

    void testFieldSetterInstance()
    {
    	RecordMetaType recordMeta(4096);
    	RecordFactoryType recordFactory(recordMeta);

        AutoPtr<ConstValueProviderMockRecord> mockCxtRecordPtr = recordFactory();
        RandomStream mockRandom;

        for (int j = 0; j < 10; j++)
        {
            for (int i = 0; i < 1000; i++)
            {
                I16u x = random() % 10000;
                I16uConstValueProvider valueProvider(x);
                FieldSetter<ConstValueProviderMockRecord, RecordTraitsType::MOCK_FIELD_1, I16uConstValueProvider> fieldSetter(valueProvider);

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
                FieldSetter<ConstValueProviderMockRecord, RecordTraitsType::MOCK_FIELD_2, I64uConstValueProvider> fieldSetter(valueProvider);

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
                FieldSetter<ConstValueProviderMockRecord, RecordTraitsType::MOCK_FIELD_3, DecimalConstValueProvider> fieldSetter(valueProvider);

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
