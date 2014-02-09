cmake_minimum_required(VERSION 2.8)
project(myriad-tests)

include_directories("src/cpp" "vendor/myriad-toolkit/src/cpp")

file(GLOB_RECURSE TEST_SOURCES src/cpp/*.h src/cpp/*.cpp)
file(GLOB_RECURSE MYRIAD_SOURCES vendor/myriad-toolkit/src/*.h vendor/myriad-toolkit/src/*.cpp)

add_executable(myriad-tests ${TEST_SOURCES} ${MYRIAD_SOURCES})

find_library(POCO_FOUNDATION PocoFoundation)
find_library(POCO_NET PocoNet)
find_library(POCO_UTIL PocoUtil)
find_library(POCO_XML PocoXML)
find_library(CPP_UNIT cppunit)
target_link_libraries(
    myriad-tests
    ${POCO_FOUNDATION} ${POCO_NET} ${POCO_UTIL} ${POCO_XML} ${CPP_UNIT}
)

