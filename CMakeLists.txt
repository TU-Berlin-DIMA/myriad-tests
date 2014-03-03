cmake_minimum_required(VERSION 2.8)
project(myriad-tests)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include_directories("src/cpp" "vendor/myriad-toolkit/src/cpp")

file(GLOB_RECURSE TEST_SOURCES src/cpp/*.h src/cpp/*.cpp)
file(GLOB_RECURSE MYRIAD_SOURCES vendor/myriad-toolkit/src/*.h vendor/myriad-toolkit/src/*.cpp)

find_package(Poco COMPONENTS Foundation Net Util XML REQUIRED)
find_package(CppUnit REQUIRED)

include_directories(${Poco_INCLUDE_DIRS} ${CPPUNIT_INCLUDE_DIR})

add_executable(myriad-tests ${TEST_SOURCES} ${MYRIAD_SOURCES})

target_link_libraries(
    myriad-tests
    ${Poco_LIBRARIES} ${CPPUNIT_LIBRARY}
)

