cmake_minimum_required(VERSION 2.6)
project(myriad-tests)

include_directories("src/cpp" "vendor/myriad-toolkit/src/cpp")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
add_executable(myriad-tests src/cpp/core/main.cpp)

find_library(POCO_FOUNDATION PocoFoundation)
find_library(POCO_NET PocoNet)
find_library(POCO_UTIL PocoUtil)
find_library(POCO_XML PocoXML)
find_library(CPP_UNIT cppunit)
target_link_libraries(
    myriad-tests
    ${POCO_FOUNDATION} ${POCO_NET} ${POCO_UTIL} ${POCO_XML} ${CPP_UNIT}
    )

