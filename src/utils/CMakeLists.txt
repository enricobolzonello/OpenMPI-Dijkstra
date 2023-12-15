# Set the project name
project (utils)

# Add a library with the above sources
add_library(${PROJECT_NAME}_star starutils.c)
add_library(sub::utils_star ALIAS ${PROJECT_NAME}_star)

include_directories(${star_SOURCE_DIR})
target_link_libraries(utils_star sub::star)

add_library(${PROJECT_NAME}_mat matrixutils.c)
add_library(sub::utils_mat ALIAS ${PROJECT_NAME}_mat)