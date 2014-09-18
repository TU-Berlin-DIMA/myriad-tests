cmake_minimum_required(VERSION 2.8)
project(myriad-tests)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# the HEADERS might be added to the executable target in order to be included in VisualStudio and XCode
set (MYRIAD_TESTS_HEADERS src/cpp/config/GeneratorConfig.h
                          src/cpp/config/GeneratorConfigTest.h
                          src/cpp/config/ObjectBuilderTest.h
                          src/cpp/core/types/MyriadDateTest.h
                          src/cpp/generator/GeneratorSubsystem.h
                          src/cpp/generator/mock/MockRecordBGenerator.h
                          src/cpp/generator/mock/MockRecordBSetterChain.h
                          src/cpp/math/algebra/MultiplicativeGroupTest.h
                          src/cpp/math/MathUtilitiesTest.h
                          src/cpp/math/probability/CombinedPrFunctionTest.h
                          src/cpp/math/probability/ProbabilityFunctionsTest.h
                          src/cpp/math/probability/util/CombinedPrFunctionInput.h
                          src/cpp/math/random/CompoundEICGTest.h
                          src/cpp/math/random/HashRandomStreamTest.h
                          src/cpp/math/random/PermutatorTest.h
                          src/cpp/record/Card.h
                          src/cpp/record/mock/MockRecordA.h
                          src/cpp/record/mock/MockRecordAMeta.h
                          src/cpp/record/mock/MockRecordB.h
                          src/cpp/record/mock/MockRecordBMeta.h
                          src/cpp/record/ProductClass.h
                          src/cpp/record/RecordRangePredicateTest.h
                          src/cpp/record/RecordX.h
                          src/cpp/record/RecordY.h
                          src/cpp/runtime/predicate/EqualityPredicateTest.h
                          src/cpp/runtime/provider/predicate/EqualityPredicateProviderTest.h
                          src/cpp/runtime/provider/range/ConstRangeProviderTest.h
                          src/cpp/runtime/provider/range/ContextFieldRangeProviderTest.h
                          src/cpp/runtime/provider/reference/ClusteredReferenceProviderTest.h
                          src/cpp/runtime/provider/reference/RandomReferenceProviderTest.h
                          src/cpp/runtime/provider/value/CallbackValueProviderTest.h
                          src/cpp/runtime/provider/value/ClusteredValueProviderTest.h
                          src/cpp/runtime/provider/value/ConstValueProviderTest.h
                          src/cpp/runtime/provider/value/ContextFieldValueProviderTest.h
                          src/cpp/runtime/provider/value/RandomValueProviderTest.h)

set (MYRIAD_TESTS_SOURCES src/cpp/core/main.cpp
                          src/cpp/generator/GeneratorSubsystem.cpp)

set(MYRIAD_TOOLKIT_SOURCES vendor/myriad-toolkit/src/cpp/communication/CommunicationSubsystem.cpp
                   vendor/myriad-toolkit/src/cpp/config/AbstractGeneratorConfig.cpp
                   vendor/myriad-toolkit/src/cpp/core/exceptions.cpp
                   vendor/myriad-toolkit/src/cpp/core/Frontend.cpp
                   vendor/myriad-toolkit/src/cpp/core/types/MyriadEnumSet.cpp
                   vendor/myriad-toolkit/src/cpp/generator/AbstractGeneratorSubsystem.cpp
                   vendor/myriad-toolkit/src/cpp/math/algebra/MultiplicativeGroup.cpp
                   vendor/myriad-toolkit/src/cpp/math/probability/Probability.cpp
                   vendor/myriad-toolkit/src/cpp/math/random/RNG.cpp
)

find_package(Poco COMPONENTS Foundation Net Util XML REQUIRED)
find_package(CppUnit REQUIRED)

include_directories("src/cpp" "vendor/myriad-toolkit/src/cpp")
include_directories(${Poco_INCLUDE_DIRS} ${CPPUNIT_INCLUDE_DIR})

add_executable(myriad-tests ${MYRIAD_TESTS_SOURCES} ${MYRIAD_TOOLKIT_SOURCES})

target_link_libraries(
    myriad-tests
    ${Poco_LIBRARIES} ${CPPUNIT_LIBRARY}
)

