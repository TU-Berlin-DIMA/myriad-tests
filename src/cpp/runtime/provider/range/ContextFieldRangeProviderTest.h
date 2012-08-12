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

#ifndef CONTEXTFIELDRANGEPROVIDERTEST_H_
#define CONTEXTFIELDRANGEPROVIDERTEST_H_

#include "record/mock/MockRecordA.h"
#include "runtime/provider/value/ConstValueProvider.h"
#include "runtime/provider/range/ContextFieldRangeProvider.h"
#include "runtime/setter/FieldSetter.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// unit tests
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class ContextFieldRangeProviderTest: public TestFixture
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

    void testRangeProviderFunctor()
    {
        for (int j = 0; j < 10; j++)
        {
			I64u sequenceCardinality = random() % 4096;
			RecordMetaType recordMeta(sequenceCardinality);
			RecordFactoryType recordFactory(recordMeta);

            AutoPtr<MockRecordA> mockCxtRecordPtr = recordFactory();
            RandomStream mockRandom;

            for (int i = 0; i < 1000; i++)
            {
                I16u x = random() % 10000;
                I16uConstValueProvider clusteredValueProvider(x);
                FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_1, I16uConstValueProvider> fieldSetter(clusteredValueProvider);

                ContextFieldRangeProvider<I64u, MockRecordA, MockField1Setter> contextFieldRangeProvider(fieldSetter);

                for (int i = 0; i < 100; i++)
                {
                	fieldSetter(mockCxtRecordPtr, mockRandom);
                    CPPUNIT_ASSERT_EQUAL_MESSAGE("Values don't match", Interval<I64u>(0, sequenceCardinality), contextFieldRangeProvider(mockCxtRecordPtr));
                }
            }
        }
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("ContextFieldRangeProviderTest");
        suite->addTest(new TestCaller<ContextFieldRangeProviderTest> ("testRangeProvider", &ContextFieldRangeProviderTest::testRangeProviderFunctor));
        return suite;
    }
};

} // namespace Myriad

#endif /* CONTEXTFIELDRANGEPROVIDERTEST_H_ */
