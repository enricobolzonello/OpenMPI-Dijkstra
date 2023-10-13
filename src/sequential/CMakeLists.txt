project(sequential_dijkstra_main)
add_executable(${PROJECT_NAME} main.c)

add_library(seqlib sequential_Dijkstra.c sequential_Dijkstra.h)
target_link_libraries(${PROJECT_NAME} seqlib)

include_directories(${utils_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} sub::utils)

include_directories(${star_SOURCE_DIR})
target_link_libraries(seqlib sub::utils)