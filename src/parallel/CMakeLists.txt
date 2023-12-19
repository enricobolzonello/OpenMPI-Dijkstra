project(parallel_dijkstra)

find_package(MPI REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)

add_executable(${PROJECT_NAME}_mat parallel_Dijkstra_matrix.c)
#add_executable(${PROJECT_NAME}_star parallel_Dijkstra_star.c)

target_link_libraries(${PROJECT_NAME}_mat MPI::MPI_C)
#target_link_libraries(${PROJECT_NAME}_star MPI::MPI_C)

include_directories(${utils_SOURCE_DIR})
#target_link_libraries(${PROJECT_NAME}_star sub::utils_star)
target_link_libraries(${PROJECT_NAME}_mat sub::utils_mat)

#include_directories(${star_SOURCE_DIR})
#target_link_libraries(${PROJECT_NAME}_star star)