set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

#############
# lint
add_library(lint STATIC
    Config.cpp
    Config.hpp
    detectors/Detector.cpp
    detectors/Detector.hpp
    detectors/LintTest.hpp
    detectors/MethodReturnLexicalScope.hpp
    detectors/WarnOnCurryArgument.hpp

    Issue.hpp
    Linter.cpp
    Linter.hpp
)

target_link_libraries(lint
    fmt
    sprklr
)

target_include_directories(lint PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    ${RAPIDJSON_INCLUDE_DIR}
)

if(WIN32)
    set_property(TARGET lint PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
    )
    # required if linking to static library
    target_compile_definitions(lint PUBLIC ANTLR4CPP_STATIC)
    # using /MD flag for antlr4_runtime (for Visual C++ compilers only)
    set(ANTLR4_WITH_STATIC_CRT OFF)
endif()

target_compile_options(lint PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
     -Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:
     /W4 /WX /DNOMINMAX>
)
