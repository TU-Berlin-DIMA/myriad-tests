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

#ifndef EQUALITYPREDICATETEST_H_
#define EQUALITYPREDICATETEST_H_

#include "record/mock/MockRecordA.h"
#include "runtime/predicate/EqualityPredicate.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

class EqualityPredicateTest: public TestFixture
{
public:

    typedef RecordTraits<MockRecordA> RecordTraitsType;
    typedef RecordTraitsType::MetaType RecordMetaType;
    typedef RecordTraitsType::FactoryType RecordFactoryType;
    typedef EqualityPredicate<MockRecordA> EqualityPredicateType;

    void setUp()
    {
        srand(54352364);
    }

    void testEqualityPredicate()
    {
        RecordMetaType recordMeta(4096);
        RecordFactoryType recordFactory(recordMeta);

        AutoPtr<MockRecordA> mockRecordAPtr = recordFactory();

        EqualityPredicateType predicate(recordFactory);

        for (int i = 0; i < 1000; i++)
        {
            predicate.reset();

            // all fiels should be unbound after the reset
            CPPUNIT_ASSERT_MESSAGE("Unexpected bound value of field mockValue1", !predicate.bound(RecordTraitsType::MOCK_FIELD_1));
            CPPUNIT_ASSERT_MESSAGE("Unexpected bound value of field mockValue2", !predicate.bound(RecordTraitsType::MOCK_FIELD_2));
            CPPUNIT_ASSERT_MESSAGE("Unexpected bound value of field mockValue3", !predicate.bound(RecordTraitsType::MOCK_FIELD_3));
            CPPUNIT_ASSERT_MESSAGE("Unexpected bound value of field mockValue4", !predicate.bound(RecordTraitsType::MOCK_FIELD_4));

            // get some random values
            I16u mockField1Value = ((random() % 100) < 60) ? random() % 32768 : nullValue<I16u>();
            I64u mockField2Value = ((random() % 100) < 60) ? random() % 65536 : nullValue<I64u>();
            Decimal mockField3Value = ((random() % 100) < 60) ? (random() % 10000) / 10000.0 : nullValue<Decimal>();
            I16u mockField4Value = ((random() % 100) < 60) ? random() % 32768 : nullValue<I16u>();

            // bound all non-null values
            if (mockField1Value != nullValue<I16u>())
            {
                predicate.fieldValue<RecordTraitsType::MOCK_FIELD_1>(mockField1Value);
            }
            if (mockField2Value != nullValue<I64u>())
            {
                predicate.fieldValue<RecordTraitsType::MOCK_FIELD_2>(mockField2Value);
            }
            if (mockField3Value != nullValue<Decimal>())
            {
                predicate.fieldValue<RecordTraitsType::MOCK_FIELD_3>(mockField3Value);
            }
            if (mockField4Value != nullValue<I16u>())
            {
                predicate.fieldValue<RecordTraitsType::MOCK_FIELD_4>(mockField4Value);
            }

            // check bound state
            CPPUNIT_ASSERT_MESSAGE("Unexpected bound state of field mockValue1", mockField1Value != nullValue<I16u>() ? predicate.bound(RecordTraitsType::MOCK_FIELD_1) : !predicate.bound(RecordTraitsType::MOCK_FIELD_1));
            CPPUNIT_ASSERT_MESSAGE("Unexpected bound state of field mockValue2", mockField2Value != nullValue<I64u>() ? predicate.bound(RecordTraitsType::MOCK_FIELD_2) : !predicate.bound(RecordTraitsType::MOCK_FIELD_2));
            CPPUNIT_ASSERT_MESSAGE("Unexpected bound state of field mockValue3", mockField3Value != nullValue<Decimal>() ? predicate.bound(RecordTraitsType::MOCK_FIELD_3) : !predicate.bound(RecordTraitsType::MOCK_FIELD_3));
            CPPUNIT_ASSERT_MESSAGE("Unexpected bound state of field mockValue4", mockField4Value != nullValue<I16u>() ? predicate.bound(RecordTraitsType::MOCK_FIELD_4) : !predicate.bound(RecordTraitsType::MOCK_FIELD_4));

            // check actual values
            if (mockField1Value != nullValue<I16u>())
            {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Unexpected mockValue1 field value", mockField1Value, predicate.fieldValue<RecordTraitsType::MOCK_FIELD_1>());
            }
            if (mockField2Value != nullValue<I64u>())
            {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Unexpected mockValue1 field value", mockField2Value, predicate.fieldValue<RecordTraitsType::MOCK_FIELD_2>());
            }
            if (mockField3Value != nullValue<Decimal>())
            {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Unexpected mockValue1 field value", mockField3Value, predicate.fieldValue<RecordTraitsType::MOCK_FIELD_3>());
            }
            if (mockField4Value != nullValue<I16u>())
            {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Unexpected mockValue1 field value", mockField4Value, predicate.fieldValue<RecordTraitsType::MOCK_FIELD_4>());
            }
        }
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("EqualityPredicateTest");
        suite->addTest(new TestCaller<EqualityPredicateTest> ("testEqualityPredicate", &EqualityPredicateTest::testEqualityPredicate));
        return suite;
    }
};

} // namespace Myriad

#endif /* EQUALITYPREDICATETEST_H_ */
