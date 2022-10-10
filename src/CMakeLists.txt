set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

#############
# sclint
add_executable(sclint
    detectors/MethodReturnLexicalScope.hpp
    detectors/Whitespace.hpp
    sclint.cpp
)

if(WIN32)
    set_property(TARGET schemac PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
    )
    # required if linking to static library
    target_compile_definitions(schemac PUBLIC ANTLR4CPP_STATIC)
    # using /MD flag for antlr4_runtime (for Visual C++ compilers only)
    set(ANTLR4_WITH_STATIC_CRT OFF)
endif()

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
