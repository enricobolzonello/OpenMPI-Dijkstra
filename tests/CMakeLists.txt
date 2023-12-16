include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(CHECK_INSTALL_DIR "/usr/local/opt/check")

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

# Since Check uses Threads to parallelize the tests, it's mandatory
# add pthread as a dependency, alongside the Check libraries.
#add_executable(star_tests star_tests.c)
#target_link_libraries(star_tests sub::star ${CHECK_LIBRARIES})

#add_executable(seqdijkstra_tests seqdijkstra_tests.c)
#target_link_libraries(seqdijkstra_tests seqlib ${CHECK_LIBRARIES})

#include_directories(${star_SOURCE_DIR})
#target_link_libraries(seqdijkstra_tests sub::star)

# Create testing target and redirect its output to `Testing` folder
add_test(NAME star_tests COMMAND star_tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(star_tests PROPERTIES TIMEOUT 30)

#add_test(NAME seqdijkstra_tests COMMAND seqdijkstra_tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
#set_tests_properties(seqdijkstra_tests PROPERTIES TIMEOUT 30) 