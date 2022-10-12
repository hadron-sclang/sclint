cmake_minimum_required(VERSION 3.9)

project(sclint VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(default_build_type "Debug")
else()
    set(GIT_BRANCH "not_a_git_checkout")
    set(GIT_COMMIT_HASH "na")
    set(default_build_type "RelWithDebInfo")
endif()

message(STATUS "sclint v${PROJECT_VERSION}, ${GIT_BRANCH}@${GIT_COMMIT_HASH}")

add_subdirectory(third_party)
add_subdirectory(src)
enable_testing()
add_subdirectory(tests)
