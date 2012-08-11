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
#include "record/mock/MockRecordA.h"
#include "runtime/provider/reference/ClusteredReferenceProvider.h"

#include <bits/stream_iterator.h>

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

//    typedef RecordTraits<MockRecordA> RecordTraitsType;
//    typedef RecordTraitsType::MetaType RecordMetaType;
//    typedef RecordTraitsType::FactoryType RecordFactoryType;
//    typedef ConstValueProvider<I16u, MockRecordA> I16uConstReferenceProvider;
//    typedef ConstValueProvider<I64u, MockRecordA> I64uConstReferenceProvider;
//    typedef ConstValueProvider<Decimal, MockRecordA> DecimalConstReferenceProvider;
//    typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_1, I16uConstReferenceProvider> MockField1Setter;
//    typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_2, I64uConstReferenceProvider> MockField2Setter;
//    typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_3, DecimalConstReferenceProvider> MockField3Setter;
//    typedef CombinedPrFunction<I16u> PrFunctionType;
//    typedef CombinedPrFunctionInput<I16u> PrFunctionInputType;
//
//    void setUp()
//    {
//        srand(54352364);
//    }
//
//    void testClusteredReferenceProviderWithConstRangeProviderForCardinality()
//    {
//        I64u scale = 4;
//        I64u min = 0;
//        I64u max = power(10, scale);
//
//        RecordMetaType recordMeta(4096);
//        RecordFactoryType recordFactory(recordMeta);
//
//        AutoPtr<MockRecordA> mockCxtRecordPtr;
//        RandomStream mockRandom;
//
//        CombinedPrFunctionInputFactory<I16u> functionInputFactory;
//        AutoPtr<PrFunctionInputType> prFunctionInput = functionInputFactory.getFunction<0>();
//        AutoPtr<PrFunctionType> prFunction(new PrFunctionType(prFunctionInput->serialize()));
//
//        ConstRangeProvider<I64u, MockRecordA> constRangeProvider(min, max);
//
//        ClusteredReferenceProvider<RecordFieldTraits<RecordTraitsType::MOCK_FIELD_1, MockRecordA>::FieldType, MockRecordA, PrFunctionType, ConstRangeProvider<I64u, MockRecordA> > clusteredReferenceProvider(*prFunction.get(), constRangeProvider);
//
//        // test range parameters
//        I16u referenceDomainMin = prFunctionInput->domain().min();
//        I16u referenceDomainMax = prFunctionInput->domain().max();
//        I32u referenceDomainLength = referenceDomainMax - referenceDomainMin;
//
//        // a vector for the computed absolute frequencies
//        vector<I64u> absoluteFrequencies(referenceDomainLength+1, 0);
//        // a vector of boolean flags for all seen reference clusters
//        vector<bool> clusterProcessed(referenceDomainLength+1, false);
//
//        I16u lastReference = nullValue<I16u>();
//        for (I64u i = min; i < max; i++)
//        {
//            mockCxtRecordPtr = recordFactory(i);
//            I16u currReference = clusteredReferenceProvider(mockCxtRecordPtr, mockRandom);
//
//            // check range
//            CPPUNIT_ASSERT_MESSAGE("Reference out of range", (currReference >= referenceDomainMin && currReference < referenceDomainMax) || (currReference == nullValue<I16u>()));
//
//            // update absolute frequency count
//            if (currReference == nullValue<I16u>())
//            {
//                absoluteFrequencies[referenceDomainLength]++;
//            }
//            else
//            {
//                absoluteFrequencies[currReference - referenceDomainMin]++;
//            }
//
//            // mark previous reference cluster as processed
//            if (lastReference != nullValue<I16u>() && lastReference != currReference)
//            {
//                CPPUNIT_ASSERT_MESSAGE(format("Reference cluster for reference %hu is already processed", currReference), !clusterProcessed[lastReference - referenceDomainMin]);
//                clusterProcessed[lastReference - referenceDomainMin] = true;
//            }
//            else if (currReference == nullValue<I16u>() && i == max - 1)
//            {
//                CPPUNIT_ASSERT_MESSAGE(format("Reference cluster for reference %hu is already processed", currReference), !clusterProcessed[lastReference - referenceDomainMin]);
//                clusterProcessed[referenceDomainMax - referenceDomainMin] = true;
//            }
//
//            // update last currReference
//            lastReference = currReference;
//        }
//
//        // check whether all reference clusters are processed
//        for (I16u X = referenceDomainMin; X < referenceDomainMax + 1U; X++)
//        {
//            if (absoluteFrequencies[X - referenceDomainMin] == 0)
//            {
//                continue;
//            }
//
//            CPPUNIT_ASSERT_MESSAGE(format("Cluster for reference %hu is not processed", (X == referenceDomainMax) ? X : nullValue<I16u>()), clusterProcessed[X - referenceDomainMin]);
//        }
//
//        // check frequencies
//        for (I16u X = referenceDomainMin; X < referenceDomainMax + 1U; X++)
//        {
//            double pdfXExp = (X < referenceDomainMax) ? prFunctionInput->pdf(X) : prFunctionInput->pdf(nullValue<I16u>());
//            double pdfXAct = absoluteFrequencies[X - referenceDomainMin]/static_cast<double>(max-min);
//
//            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Relative frequencies do not match", pdfXExp, pdfXAct, 5 * pow(10.0, -static_cast<double>(scale/2)));
//        }
//    }
//
//    void testClusteredReferenceProviderWithContextFieldRangeProviderForCardinality()
//    {
//        // type shortcuts for this test
//        typedef ConstRangeProvider<I64u, MockRecordA> MockField1RangeProviderType;
//        typedef ClusteredReferenceProvider<RecordFieldTraits<RecordTraitsType::MOCK_FIELD_1, MockRecordA>::FieldType, MockRecordA, PrFunctionType, MockField1RangeProviderType> MockField1ReferenceProviderType;
//        typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_1, MockField1ReferenceProviderType> MockField1FieldSetterType;
//
//        typedef ContextFieldRangeProvider<I64u, MockRecordA, MockField1FieldSetterType> MockField4RangeProviderType;
//        typedef ClusteredReferenceProvider<RecordFieldTraits<RecordTraitsType::MOCK_FIELD_4, MockRecordA>::FieldType, MockRecordA, PrFunctionType, MockField4RangeProviderType> MockField4ReferenceProviderType;
//        typedef FieldSetter<MockRecordA, RecordTraitsType::MOCK_FIELD_4, MockField4ReferenceProviderType> MockField4FieldSetterType;
//
//        typedef pair<I16u, I16u> FieldsPairType;
//
//        I64u scale = 4;
//        I64u min = 0;
//        I64u max = power(10, scale);
//
//        RecordMetaType recordMeta(4096);
//        RecordFactoryType recordFactory(recordMeta);
//
//        AutoPtr<MockRecordA> mockCxtRecordPtr;
//        RandomStream mockRandom;
//
//        CombinedPrFunctionInputFactory<I16u> functionInputFactory;
//        AutoPtr<PrFunctionInputType> prFunctionInput = functionInputFactory.getFunction<1>();
//        AutoPtr<PrFunctionType> prFunction(new PrFunctionType(prFunctionInput->serialize()));
//
//        MockField1RangeProviderType mockField1RangeProvider(min, max);
//        MockField1ReferenceProviderType mockField1ReferenceProvider(*prFunction.get(), mockField1RangeProvider);
//        MockField1FieldSetterType mockField1FieldSetter(mockField1ReferenceProvider);
//
//        MockField4RangeProviderType mockField4RangeProvider(mockField1FieldSetter);
//        MockField4ReferenceProviderType mockField4ReferenceProvider(*prFunction.get(), mockField4RangeProvider);
//        MockField4FieldSetterType mockField4FieldSetter(mockField4ReferenceProvider);
//
//        // test range parameters
//        I16u referenceDomainMin = prFunctionInput->domain().min();
//        I16u referenceDomainMax = prFunctionInput->domain().max();
//        I32u referenceDomainLength = referenceDomainMax - referenceDomainMin;
//
//        // a vector for the computed absolute frequencies
//        vector<I64u> absoluteFrequencies((referenceDomainLength+1)*(referenceDomainLength+1), 0);
//        // a vector of boolean flags for all seen reference pair clusters
//        vector<bool> clusterProcessed((referenceDomainLength+1)*(referenceDomainLength+1), false);
//
//        FieldsPairType referenceDomainPair(referenceDomainMin, referenceDomainMax);
//        FieldsPairType nullValuePair(nullValue<I16u>(), nullValue<I16u>());
//        FieldsPairType lastReferencePair(nullValuePair);
//
////        std::cout << "test range is " << referenceDomainMin << ", " << referenceDomainMax << std::endl;
////        std::cout << "test range index length is " << (referenceDomainLength+1)*(referenceDomainLength+1) << std::endl;
////
////        for (I64u i = referenceDomainMin; i <= referenceDomainMax; i++)
////        {
////            for (I64u j = referenceDomainMin; j <= referenceDomainMax; j++)
////            {
////                FieldsPairType pair((i < referenceDomainMax) ? i : nullValue<I16u>(), (j < referenceDomainMax) ? j : nullValue<I16u>());
////                std::cout << "index for reference pair (" << pair.first << ", " << pair.second << ") is " << referencePairIndex(pair, referenceDomainPair) << '\n';
////            }
////            std::cout << std::endl;
////        }
//
//        for (I64u i = min; i < max; i++)
//        {
//            // create a new record
//            mockCxtRecordPtr = recordFactory(i);
//
//            // set mockField1 reference
//            mockField1FieldSetter(mockCxtRecordPtr, mockRandom);
//            // grab mockField1 reference
//            I16u currMockField1Reference = mockCxtRecordPtr->mockField1();
//
//            // set mockField4 reference
//            mockField4FieldSetter(mockCxtRecordPtr, mockRandom);
//            // grab mockField4 reference
//            I16u currMockField4Reference = mockCxtRecordPtr->mockField4();
//
//            // create the curr reference pair
//            FieldsPairType currReferencePair(currMockField1Reference, currMockField4Reference);
//
////            std::cout << format("record[#%06Lu] = (%hu, %hu)", i, currReferencePair.first, currReferencePair.second) << std::endl;
//
//            // check range
//            CPPUNIT_ASSERT_MESSAGE(format("Reference %hu at position %Lu out of range", currReferencePair.second, i), (currMockField4Reference >= referenceDomainMin && currMockField4Reference < referenceDomainMax) || (currMockField4Reference == nullValue<I16u>()));
//
//            // update absolute frequency count
//            absoluteFrequencies[referencePairIndex(currReferencePair, referenceDomainPair)]++;
//
//            // mark previous reference cluster as processed
//            if (lastReferencePair != nullValuePair && lastReferencePair != currReferencePair)
//            {
//                CPPUNIT_ASSERT_MESSAGE(format("Reference cluster for reference pair (%hu, %hu) at index %z for genID = %Lu is already processed", currReferencePair.first, currReferencePair.second, referencePairIndex(lastReferencePair, referenceDomainPair), i), !clusterProcessed[referencePairIndex(lastReferencePair, referenceDomainPair)]);
//                clusterProcessed[referencePairIndex(lastReferencePair, referenceDomainPair)] = true;
//            }
//            else if (currReferencePair == nullValuePair && i == max - 1)
//            {
//                CPPUNIT_ASSERT_MESSAGE(format("Reference cluster for null reference pair is already processed", currReferencePair.first, currReferencePair.second), !clusterProcessed[referencePairIndex(lastReferencePair, referenceDomainPair)]);
//                clusterProcessed[referencePairIndex(currReferencePair, referenceDomainPair)] = true;
//            }
//
//            // update last currReference
//            lastReferencePair = currReferencePair;
//        }
//
//        // check whether all reference clusters are processed
//        for (I16u X = referenceDomainMin; X <= referenceDomainMax; X++)
//        {
//            for (I16u Y = referenceDomainMin; Y <= referenceDomainMax; Y++)
//            {
//                FieldsPairType pair((X < referenceDomainMax) ? X : nullValue<I16u>(), (Y < referenceDomainMax) ? Y : nullValue<I16u>());
//                if (absoluteFrequencies[referencePairIndex(pair, referenceDomainPair)] == 0)
//                {
//                    continue;
//                }
//
//                CPPUNIT_ASSERT_MESSAGE(format("Cluster for reference pair (%hu, %hu) is not processed", pair.first, pair.second), clusterProcessed[referencePairIndex(pair, referenceDomainPair)]);
//            }
//        }
//
//        // check frequencies
//        for (I16u X = referenceDomainMin; X <= referenceDomainMax; X++)
//        {
//            for (I16u Y = referenceDomainMin; Y <= referenceDomainMax; Y++)
//            {
//                FieldsPairType pair((X < referenceDomainMax) ? X : nullValue<I16u>(), (Y < referenceDomainMax) ? Y : nullValue<I16u>());
//
//                double pdfXExp = prFunctionInput->pdf(pair.first) * prFunctionInput->pdf(pair.second);
//                double pdfXAct = absoluteFrequencies[referencePairIndex(pair, referenceDomainPair)]/static_cast<double>(max-min);
//
//                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Relative frequencies do not match", pdfXExp, pdfXAct, 5 * pow(10.0, -static_cast<double>(scale/2)));
//            }
//        }
//    }

    static Test *suite()
    {
        TestSuite* suite = new TestSuite("ClusteredReferenceProviderTest");
//        suite->addTest(new TestCaller<ClusteredReferenceProviderTest> ("testClusteredReferenceProviderWithConstRangeProviderForCardinality", &ClusteredReferenceProviderTest::testClusteredReferenceProviderWithConstRangeProviderForCardinality));
//        suite->addTest(new TestCaller<ClusteredReferenceProviderTest> ("testClusteredReferenceProviderWithContextFieldRangeProviderForCardinality", &ClusteredReferenceProviderTest::testClusteredReferenceProviderWithContextFieldRangeProviderForCardinality));
        return suite;
    }
};

} // namespace Myriad

#endif /* CLUSTEREDREFERENCEPROVIDERTEST_H_ */
