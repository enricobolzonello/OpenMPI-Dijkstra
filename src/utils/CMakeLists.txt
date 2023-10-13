# Set the project name
project (utils)

# Add a library with the above sources
add_library(${PROJECT_NAME} utils.c)
add_library(sub::utils ALIAS ${PROJECT_NAME})

include_directories(${star_SOURCE_DIR})
target_link_libraries(utils star)