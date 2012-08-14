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

#ifndef RANDOMREFERENCEPROVIDERTEST_H_
#define RANDOMREFERENCEPROVIDERTEST_H_

#include "math/probability/util/CombinedPrFunctionInput.h"
#include "generator/mock/MockRecordBGenerator.h"
#include "record/mock/MockRecordA.h"
#include "record/mock/MockRecordB.h"
#include "runtime/provider/value/CallbackValueProvider.h"
#include "runtime/provider/reference/RandomReferenceProvider.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// unit tests
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class RandomReferenceProviderTest: public TestFixture
{
public:

    // record traits types
    typedef RecordTraits<MockRecordA> MockRecordATraitsType;
    typedef RecordTraits<MockRecordB> MockRecordBTraitsType;
    // record meta types
    typedef MockRecordATraitsType::MetaType MockRecordAMetaType;
    typedef MockRecordBTraitsType::MetaType MockRecordBMetaType;
    // record factory types
    // record meta types
    typedef MockRecordATraitsType::FactoryType MockRecordAFactoryType;
    typedef MockRecordBTraitsType::FactoryType MockRecordBFactoryType;
    // value providers
    typedef CallbackValueProvider<Enum, MockRecordA, RandomReferenceProviderTest> MockRecordBCategoryValueProviderType;
    typedef CallbackValueProvider<Enum, MockRecordA, RandomReferenceProviderTest> MockRecordBTypeValueProviderType;
    // field binders
    typedef EqualityPredicateFieldBinder<MockRecordB, MockRecordBTraitsType::CATEGORY, MockRecordA, MockRecordBCategoryValueProviderType> MockRecordBCategoryFieldBinderType;
    typedef EqualityPredicateFieldBinder<MockRecordB, MockRecordBTraitsType::TYPE, MockRecordA, MockRecordBTypeValueProviderType> MockRecordBTypeFieldBinderType;
    // predicate providers
    typedef EqualityPredicateProvider<MockRecordB, MockRecordA> MockRecordBPredicateProvider;
    // reference providers
    typedef RandomReferenceProvider<MockRecordB, MockRecordA> MockRecordBReferenceProviderType;

    void setUp()
    {
        srand(54352364);
    }

    void testRandomReferenceProvider()
    {
        // record sequence cardinalities
        I64u mockRecordASequenceCardinality = 1000000;
        I32u avgChildrenPerParent = 10000;
        I64u mockRecordBSequenceCardinality = mockRecordASequenceCardinality/avgChildrenPerParent;

        // application config
        AutoPtr<MapConfiguration> appConfig(new MapConfiguration());
        // common required config parameters
        appConfig->setInt("common.partitioning.number-of-chunks", 1);
        appConfig->setInt("common.partitioning.chunks-id", 0);
        // partitioning config for the MockRecordA sequence
        appConfig->setString("generator.mock_record_a.sequence.cardinality", toString<I64u>(mockRecordASequenceCardinality));
        appConfig->setString("generator.mock_record_a.partition.begin", toString<I64u>(0));
        appConfig->setString("generator.mock_record_a.partition.end", toString<I64u>(mockRecordASequenceCardinality));
        // partitioning config for the MockRecordB sequence
        appConfig->setString("generator.mock_record_b.sequence.cardinality", toString<I64u>(mockRecordBSequenceCardinality));
        appConfig->setString("generator.mock_record_b.partition.begin", toString<I64u>(0));
        appConfig->setString("generator.mock_record_b.partition.end", toString<I64u>(mockRecordBSequenceCardinality));
        // notification center
        NotificationCenter& notificationCenter = NotificationCenter::defaultCenter();
        // generator config
        GeneratorPool generatorPool;
        GeneratorConfig generatorConfig(generatorPool);
        generatorConfig.addWriteable(appConfig, 0, true);
        // MockRecordB record generator
        MockRecordBGenerator mockRecordBGenerator("mock_record_b", generatorConfig, notificationCenter);
        mockRecordBGenerator.initialize();
        // MockRecordB sequence inspector
        RandomSetInspector<MockRecordB> mockRecordBSequence(mockRecordBGenerator.inspector());

        MockRecordAMetaType mockRecordAMeta(mockRecordASequenceCardinality);
        MockRecordBMetaType mockRecordBMeta(mockRecordBSequenceCardinality);
        MockRecordAFactoryType mockRecordAFactory(mockRecordAMeta);
        MockRecordBFactoryType mockRecordBFactory(mockRecordBMeta);

        RandomStream randomStream = generatorConfig.masterPRNG();

        // value providers for the MockRecordB predicate provider field binders
        MockRecordBCategoryValueProviderType mockRecordBCategoryValueProvider(*this, &RandomReferenceProviderTest::randomCategory, 1);
        MockRecordBTypeValueProviderType mockRecordBTypeValueProvider(*this, &RandomReferenceProviderTest::randomType, 1);
        // MockRecordB predicate provider field binders
        MockRecordBCategoryFieldBinderType mockRecordBCategoryFieldBinder(mockRecordBCategoryValueProvider);
        MockRecordBTypeFieldBinderType mockRecordBTypeFieldBinder(mockRecordBTypeValueProvider);
        // MockRecordB predicate provider
        MockRecordBPredicateProvider mockRecordBPredicateProvider(mockRecordBFactory, mockRecordBCategoryFieldBinder, mockRecordBTypeFieldBinder);
        // MockRecordB reference provider
        MockRecordBReferenceProviderType randomReferenceProvider(mockRecordBPredicateProvider, mockRecordBSequence);

        vector< Interval<I64u> > mockRecordBGenIDRanges(10);
        int N = mockRecordBSequenceCardinality / 10;
        for (I64u i = 0; i < 10; i++)
		{
        	mockRecordBGenIDRanges[i].set(i * N, (i + 1) * N);
		}

        AutoPtr<MockRecordA> mockCxtRecordPtr;
        AutoPtr<MockRecordB> currReference;
        for (I64u i = 0; i < mockRecordASequenceCardinality; i++)
        {
            randomStream.atChunk(i);
            mockCxtRecordPtr = mockRecordAFactory(i);

			currReference = randomReferenceProvider(mockCxtRecordPtr, randomStream);

            // make sure reference is not NULL
            CPPUNIT_ASSERT_MESSAGE("Reference is NULL", !currReference.isNull());

            // check reference properties
            CPPUNIT_ASSERT_EQUAL_MESSAGE("MockRecordB 'category' value does not match", _lastReturnedCategory, currReference->category());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("MockRecordB 'type' value does not match", _lastReturnedType, currReference->type());

            // check genID range
            size_t j = _lastReturnedCategory * 2 + _lastReturnedType;
            CPPUNIT_ASSERT_MESSAGE("MockRecordB genID is out of expected range", mockRecordBGenIDRanges[j].contains(currReference->genID()));
        }
    }

    Enum randomCategory(const AutoPtr<MockRecordA>& cxtRecordPtr, RandomStream& random)
    {
    	I16u x = random(0, 100);

    	if (x < 10)
    	{
    		_lastReturnedCategory = 0;
    	}
    	else if (x < 40)
    	{
    		_lastReturnedCategory = 1;
    	}
    	else if (x < 60)
    	{
    		_lastReturnedCategory = 2;
    	}
    	else if (x < 90)
    	{
    		_lastReturnedCategory = 3;
    	}
    	else
    	{
    		_lastReturnedCategory = 4;
    	}

    	return _lastReturnedCategory;
    }

    Enum randomType(const AutoPtr<MockRecordA>& cxtRecordPtr, RandomStream& random)
    {
    	I16u x = random(0, 100);

    	if (x < 40)
    	{
    		_lastReturnedType = 0;
    	}
    	else
    	{
    		_lastReturnedType = 1;
    	}

    	return _lastReturnedType;
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("RandomReferenceProviderTest");
        suite->addTest(new TestCaller<RandomReferenceProviderTest> ("testRandomReferenceProvider", &RandomReferenceProviderTest::testRandomReferenceProvider));
        return suite;
    }

protected:

    Enum _lastReturnedCategory;
    Enum _lastReturnedType;
};

} // namespace Myriad

#endif /* RANDOMREFERENCEPROVIDERTEST_H_ */
