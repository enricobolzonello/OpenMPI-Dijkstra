project(parallel_dijkstra)

find_package(MPI REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)

add_executable(${PROJECT_NAME} parallel_Dijkstra.c)

target_link_libraries(${PROJECT_NAME} MPI::MPI_C)

include_directories(${utils_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} sub::utils)

include_directories(${star_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} sub::star)