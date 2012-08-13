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

#ifndef EQUALITYPREDICATEPROVIDERTEST_H_
#define EQUALITYPREDICATEPROVIDERTEST_H_

#include "record/mock/MockRecordA.h"
#include "record/mock/MockRecordB.h"
#include "runtime/provider/predicate/EqualityPredicateProvider.h"
#include "runtime/provider/value/ConstValueProvider.h"
#include "runtime/provider/value/ContextFieldValueProvider.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

class EqualityPredicateProviderTest: public TestFixture
{
public:

    typedef RecordTraits<MockRecordA> MockRecordATraitsType;
    typedef MockRecordATraitsType::MetaType MockRecordAMetaType;
    typedef MockRecordATraitsType::FactoryType MockRecordAFactoryType;

    typedef RecordTraits<MockRecordB> MockRecordBTraitsType;
    typedef MockRecordBTraitsType::MetaType MockRecordBMetaType;
    typedef MockRecordBTraitsType::FactoryType MockRecordBFactoryType;

    typedef ContextFieldValueProvider<I16u, MockRecordB, MockRecordBTraitsType::RISK_RATING> PredicateMockField1ValueProviderType;
    typedef ConstValueProvider<Decimal, MockRecordB> PredicateMockField3ValueProviderType;

    typedef EqualityPredicate<MockRecordA> EqualityPredicateType;
    typedef AbstractFieldBinder<MockRecordA, MockRecordB> AbstractFieldBinderType;
    typedef EqualityPredicateFieldBinder<MockRecordA, MockRecordATraitsType::MOCK_FIELD_1, MockRecordB, PredicateMockField1ValueProviderType> PredicateMockField1FieldBinderType;
    typedef EqualityPredicateFieldBinder<MockRecordA, MockRecordATraitsType::MOCK_FIELD_3, MockRecordB, PredicateMockField3ValueProviderType> PredicateMockField3FieldBinderType;
    typedef EqualityPredicateProvider<MockRecordA, MockRecordB> EqualityPredicateProviderType;

    void setUp()
    {
        srand(54352364);
    }

    void testEqualityPredicateProvider()
    {
        MockRecordAMetaType mockRecordAMeta(4096);
        MockRecordAFactoryType mockRecordAFactory(mockRecordAMeta);

        MockRecordBMetaType mockRecordBMeta(4096);
        MockRecordBFactoryType mockRecordBFactory(mockRecordBMeta);

        RandomStream randomStream;

        for(int i = 0; i < 100; i++)
        {
            Decimal mockField3Value = (random() % 10000)/10000.0;

            PredicateMockField1ValueProviderType predicateMockField1ValueProvider;
            PredicateMockField3ValueProviderType predicateMockField3ValueProvider(mockField3Value);

            PredicateMockField1FieldBinderType predicateMockField1Binder(predicateMockField1ValueProvider);
            PredicateMockField3FieldBinderType predicateMockField3Binder(predicateMockField3ValueProvider);

            EqualityPredicateProviderType predicateProvider(mockRecordAFactory, predicateMockField1Binder, predicateMockField3Binder);

            for (int j = 0; j < 100; j++)
            {
                I16u mockField1Value = random() % 10000;

                AutoPtr<MockRecordB> mockRecordBPtr = mockRecordBFactory();
                mockRecordBPtr->riskRating(mockField1Value);

                const EqualityPredicateType& predicate = predicateProvider(mockRecordBPtr, randomStream);

                CPPUNIT_ASSERT_EQUAL_MESSAGE("Bound MockRecordA predicate for mockField1 does not match", mockField1Value, predicate.fieldValue<MockRecordATraitsType::MOCK_FIELD_1>());
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Bound MockRecordA predicate for mockField3 does not match", mockField3Value, predicate.fieldValue<MockRecordATraitsType::MOCK_FIELD_3>());
            }
        }
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("EqualityPredicateProviderTest");
        suite->addTest(new TestCaller<EqualityPredicateProviderTest> ("testEqualityPredicateProvider", &EqualityPredicateProviderTest::testEqualityPredicateProvider));
        return suite;
    }
};

} // namespace Myriad

#endif /* EQUALITYPREDICATEPROVIDERTEST_H_ */
