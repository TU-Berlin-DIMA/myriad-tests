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
#include "config/ObjectBuilderTest.h"
#include "config/GeneratorConfigTest.h"
#include "hydrator/ConditionalHydratorTest.h"
#include "hydrator/ClusteredEnumSetHydratorTest.h"
#include "hydrator/EnumSetHydratorTest.h"
#include "hydrator/RangeSetHydratorTest.h"
#include "hydrator/ConditionalRandomizedHydratorTest.h"
#include "math/MathUtilitiesTest.h"
#include "math/IntervalMapTest.h"
#include "math/probability/CombinedPrFunctionTest.h"
#include "math/probability/ProbabilityFunctionsTest.h"
#include "math/probability/QHistogramPrFunctionTest.h"
#include "math/probability/ConditionalQHistogramPrFunctionTest.h"
#include "math/random/CompoundEICGTest.h"
#include "math/random/HashRandomStreamTest.h"
#include "math/random/SurrogateKeyGeneratorTest.h"
#include "reflection/getter/ValueGetterTest.h"

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
	runner.addTest(MathUtilitiesTest::suite());
	runner.addTest(GeneratorConfigTest::suite());
	runner.addTest(ProbabilityFunctionsTest::suite());
     */
//	runner.addTest(ObjectBuilderTest::suite());
//	runner.addTest(CompoundEICGTest::suite());
//	runner.addTest(HashRandomStreamTest::suite());
//	runner.addTest(ConditionalHydratorTest::suite());
//	runner.addTest(ClusteredEnumSetHydratorTest::suite());
//	runner.addTest(EnumSetHydratorTest::suite());
//	runner.addTest(RangeSetHydratorTest::suite());
//	runner.addTest(ConditionalRandomizedHydratorTest::suite());
//	runner.addTest(SurrogateKeyGeneratorTest::suite());
	runner.addTest(CombinedPrFunctionTest<I64u>::suite());
	runner.addTest(CombinedPrFunctionTest<Date>::suite());
//	runner.addTest(QHistogramPrFunctionTest::suite());
//	runner.addTest(ConditionalQHistogramPrFunctionTest::suite());
//	runner.addTest(ValueGetterTest::suite());
	runner.run();

	return 0;
}

