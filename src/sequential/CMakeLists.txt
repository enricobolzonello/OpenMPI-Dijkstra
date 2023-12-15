project(sequential_dijkstra_main)
add_executable(${PROJECT_NAME} sequential_Dijkstra.c sequential_Dijkstra.h)

include_directories(${utils_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} sub::utils_star)