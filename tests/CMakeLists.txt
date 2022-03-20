find_library(archiver_library ../library)

add_executable(tests tests.cpp)
target_link_libraries(tests archiver_library)