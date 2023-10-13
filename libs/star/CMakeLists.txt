# Set the project name
project (star)

# Add a library with the above sources
add_library(${PROJECT_NAME} src/star.c)
add_library(sub::star ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC include
)