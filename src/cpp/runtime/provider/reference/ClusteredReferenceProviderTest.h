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

#ifndef CLUSTEREDREFERENCEPROVIDERTEST_H_
#define CLUSTEREDREFERENCEPROVIDERTEST_H_

#include "math/probability/util/CombinedPrFunctionInput.h"
#include "generator/mock/MockRecordBGenerator.h"
#include "record/mock/MockRecordA.h"
#include "record/mock/MockRecordB.h"
#include "runtime/provider/reference/ClusteredReferenceProvider.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// unit tests
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class ClusteredReferenceProviderTest: public TestFixture
{
public:

    typedef RecordTraits<MockRecordA> MockRecordATraitsType;
    typedef RecordTraits<MockRecordB> MockRecordBTraitsType;

    typedef ConstValueProvider<I32u, MockRecordA> MaxChildrenValueProvider;
    typedef ContextFieldValueProvider<I32u, MockRecordB, MockRecordBTraitsType::CATALOG_SIZE, 0, 0> ChildrenCountValueProviderType;
    typedef ClusteredReferenceProvider<MockRecordB, MockRecordA, ChildrenCountValueProviderType, MockRecordATraitsType::POSITION> MockRecordBReferenceProviderType;

    typedef MockRecordATraitsType::MetaType MockRecordAMetaType;
    typedef MockRecordATraitsType::FactoryType MockRecordAFactoryType;

    void setUp()
    {
        srand(54352364);
    }

    void testClusteredReferenceProvider()
    {
        I64u scale = 4;
        I64u min = 0;
        I64u max = power(10, scale);

        I32u maxChildrenPerParent = 50;

        // application config
        AutoPtr<MapConfiguration> appConfig(new MapConfiguration());
        appConfig->setInt("common.partitioning.number-of-chunks", 1);
        appConfig->setInt("common.partitioning.chunks-id", 0);
        appConfig->setString("generator.mock_record_b.sequence.cardinality", toString<I64u>(max/maxChildrenPerParent));
        appConfig->setString("generator.mock_record_b.partition.begin", toString<I64u>(0));
        appConfig->setString("generator.mock_record_b.partition.end", toString<I64u>(max/maxChildrenPerParent));
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
        RandomSequenceInspector<MockRecordB> mockRecordBSequence(mockRecordBGenerator.inspector());

        MockRecordAMetaType recordMeta(4096);
        MockRecordAFactoryType recordFactory(recordMeta);

        RandomStream randomStream = generatorConfig.masterPRNG();

        MaxChildrenValueProvider maxChildrenValueProvider(maxChildrenPerParent);
        ChildrenCountValueProviderType childrenCountValueProvider;
        MockRecordBReferenceProviderType clusteredReferenceProvider(maxChildrenValueProvider, childrenCountValueProvider, mockRecordBSequence);

        I32u referenceDomainMin = 0;
        I32u referenceDomainMax = generatorConfig.cardinality("mock_record_b");
        I32u referenceDomainLength = referenceDomainMax - referenceDomainMin;

        // a vector for the computed absolute frequencies
        vector<I64u> absoluteFrequencies(referenceDomainLength, 0);
        // a vector of boolean flags for all seen reference clusters
        vector<bool> clusterProcessed(referenceDomainLength, false);

        AutoPtr<MockRecordA> lastMockCxtRecordPtr, mockCxtRecordPtr;
        AutoPtr<MockRecordB> lastReference, currReference;
        for (I64u i = min; i < max; i++)
        {
            randomStream.atChunk(i);
            mockCxtRecordPtr = recordFactory(i);

            try
            {
                currReference = clusteredReferenceProvider(mockCxtRecordPtr, randomStream);
            }
            catch(const InvalidRecordException& e)
            {
                lastMockCxtRecordPtr.assign(NULL);
                i = e.nextValidGenID()-1;
                continue;
            }

            // make sure reference is not NULL
            CPPUNIT_ASSERT_MESSAGE("Reference is NULL", !currReference.isNull());
            // check range
            CPPUNIT_ASSERT_MESSAGE("Reference out of range", currReference->genID() >= referenceDomainMin && currReference->genID() < referenceDomainMax);

            // update absolute frequency count
            absoluteFrequencies[currReference->genID()]++;

            // mark previous reference cluster as processed
            if (!lastReference.isNull() && lastReference != currReference)
            {
                CPPUNIT_ASSERT_MESSAGE(format("Reference cluster for reference #%Lu is already processed", lastReference->genID()), !clusterProcessed[lastReference->genID()]);
                clusterProcessed[lastReference->genID()] = true;
            }

            if (lastReference != currReference)
            {
            	lastMockCxtRecordPtr.assign(NULL);
            }

            CPPUNIT_ASSERT_EQUAL_MESSAGE(format("Wrong position value of nested record #%Lu", mockCxtRecordPtr->genID()), lastMockCxtRecordPtr.isNull() ? 0 : lastMockCxtRecordPtr->position()+1, mockCxtRecordPtr->position());

            // update last currReference
            lastReference = currReference;
            lastMockCxtRecordPtr = mockCxtRecordPtr;
        }

        // make sure the last cluster is marked as processed as weel
        CPPUNIT_ASSERT_MESSAGE(format("Reference cluster for reference #%Lu is already processed", lastReference->genID()), !clusterProcessed[lastReference->genID()]);
        clusterProcessed[lastReference->genID()] = true;

        // check whether all reference clusters are processed
        for (I64u X = referenceDomainMin; X < referenceDomainMax; X++)
        {
            if (absoluteFrequencies[X] == 0)
            {
                continue;
            }

            CPPUNIT_ASSERT_MESSAGE(format("Cluster for reference #%Lu is not processed", X), clusterProcessed[X]);
        }

        // check absolute frequencies
        for (I32u X = referenceDomainMin; X < referenceDomainMax; X++)
        {
            I32u absoluteFrequencyExp = mockRecordBSequence.at(X)->catalogSize();
            I32u absoluteFrequencyAct = absoluteFrequencies[X - referenceDomainMin];

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Relative frequencies do not match", absoluteFrequencyExp, absoluteFrequencyAct);
        }
    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("ClusteredReferenceProviderTest");
        suite->addTest(new TestCaller<ClusteredReferenceProviderTest> ("testClusteredReferenceProvider", &ClusteredReferenceProviderTest::testClusteredReferenceProvider));
        return suite;
    }
};

} // namespace Myriad

#endif /* CLUSTEREDREFERENCEPROVIDERTEST_H_ */
