project(sequential_dijkstra)
#add_executable(${PROJECT_NAME}_star sequential_Dijkstra.c sequential_Dijkstra.h)
add_executable(${PROJECT_NAME}_mat sequential_Dijkstra_matrix.c)

include_directories(${utils_SOURCE_DIR})
#target_link_libraries(${PROJECT_NAME}_star sub::utils_star)
target_link_libraries(${PROJECT_NAME}_mat sub::utils_mat)