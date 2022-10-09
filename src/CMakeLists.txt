set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

#############
# sclint
add_executable(sclint
    MethodReturnWithLexicalScopeListener.hpp
    sclint.cpp
)

target_link_libraries(sclint
    gflags
    fmt
    sprklr
)

target_compile_options(sclint PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
     -Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:
     /W4 /WX /DNOMINMAX>
)