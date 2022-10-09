#### fmt
add_subdirectory(fmt)

#### gflags
set(GFLAGS_BUILD_STATIC_LIBS ON CACHE BOOL "build gflags as a static library")
set(GFLAGS_BUILD_gflags_LIB ON CACHE BOOL "build multithreaded gflags library")
add_subdirectory(gflags)

#### rapidjson
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "skip building rapidjson docs")
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "skip building rapidjson examples")
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "skip building rapidjson googletest tests")
set(RAPIDJSON_BUILD_CXX11 OFF CACHE BOOL "prefer cxx17 for rapidjson")
set(RAPIDJSON_BUILD_CXX17 ON CACHE BOOL "prefer cxx17 for rapidjson")
set(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT OFF CACHE BOOL "clang compiler doesn't like -march")
add_subdirectory(rapidjson)
set(RAPIDJSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include CACHE STRING "rapidjson header include path")

#### sprklr
add_subdirectory(sparkler)
