set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

add_subdirectory(lint)

# sclint binary not included in the WASM build.
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
#############
# sclint
add_executable(sclint
    sclint.cpp
)

target_link_libraries(sclint
    fmt
    gflags
    lint
)

target_compile_options(sclint PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
     -Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:
     /W4 /WX /DNOMINMAX>
)
endif()