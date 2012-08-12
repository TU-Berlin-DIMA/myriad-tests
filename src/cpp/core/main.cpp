/*
 * Copyright 2011 DIMA research group, TU Berlin
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
 * @author: Alexander Alexandrov <alexander@alex.androv.eu>
 */

#include "core/constants.h"
#include "core/Frontend.h"
#include "core/types/MyriadDateTest.h"
#include "config/ObjectBuilderTest.h"
#include "config/GeneratorConfigTest.h"
#include "math/MathUtilitiesTest.h"
#include "math/IntervalMapTest.h"
#include "math/probability/CombinedPrFunctionTest.h"
#include "math/probability/ProbabilityFunctionsTest.h"
#include "math/random/CompoundEICGTest.h"
#include "math/random/HashRandomStreamTest.h"
#include "math/random/SurrogateKeyGeneratorTest.h"
#include "record/RecordRangePredicateTest.h"
#include "reflection/getter/ValueGetterTest.h"
#include "runtime/provider/range/ConstRangeProviderTest.h"
#include "runtime/provider/range/ContextFieldRangeProviderTest.h"
#include "runtime/provider/reference/ClusteredReferenceProviderTest.h"
#include "runtime/provider/value/CallbackValueProviderTest.h"
#include "runtime/provider/value/ClusteredValueProviderTest.h"
#include "runtime/provider/value/ConstValueProviderTest.h"
#include "runtime/provider/value/ContextFieldValueProviderTest.h"
#include "runtime/provider/value/RandomValueProviderTest.h"

#include <cppunit/ui/text/TestRunner.h>

using namespace CppUnit;
using namespace Myriad;

namespace Myriad {

/**
 * Application name.
 */
const String Constant::APP_NAME = "Myriad UnitTest Suite";

/**
 * Application version.
 */
const String Constant::APP_VERSION = "0.1.0";

} // Myriad namespace

int main(int argc, char **argv)
{
    TextUi::TestRunner runner;

    /*
     * TODO: these tests need to be updated / removed
    runner.addTest(IntervalMapTest::suite());
    runner.addTest(GeneratorConfigTest::suite());
     */

    // core/types
    runner.addTest(MyriadDateTest::suite());
    // math/random
    runner.addTest(CompoundEICGTest::suite());
    runner.addTest(HashRandomStreamTest::suite());
    // math/probability
    runner.addTest(MathUtilitiesTest::suite());
    runner.addTest(CombinedPrFunctionTest<I64u>::suite());
    runner.addTest(CombinedPrFunctionTest<Date>::suite());
//    runner.addTest(ConditionalCombinedPrFunctionTest::suite()); //TODO: implement this test, reuse the old ConditionalQHistogramPrFunctionTest
    runner.addTest(ProbabilityFunctionsTest::suite()); //TODO: disable function output and add result verification code to this test suite
    // util
//    runner.addTest(ValueGetterTest::suite()); // TODO: refactor value getter logic and update the unit test
//    runner.addTest(ObjectBuilderTest::suite()); // TODO: move in ObjectBuilder
    // record
    runner.addTest(RecordRangePredicateTest::suite());
    // runtime/provider/range
    runner.addTest(ConstRangeProviderTest::suite());
    runner.addTest(ContextFieldRangeProviderTest::suite());
    // runtime/provider/reference
    runner.addTest(ClusteredReferenceProviderTest::suite());
    // runtime/provider/value
    runner.addTest(CallbackValueProviderTest::suite());
    runner.addTest(ClusteredValueProviderTest::suite());
    runner.addTest(ConstValueProviderTest::suite());
    runner.addTest(ContextFieldValueProviderTest::suite());
    runner.addTest(RandomValueProviderTest::suite());
    // runtime/setter (TODO: write unit tests for the field and reference setter types)
//    runner.addTest(FieldSetterTest::suite());
//    runner.addTest(ReferenceSetterTest::suite());

    runner.run();

    return 0;
}

