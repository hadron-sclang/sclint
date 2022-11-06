set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

#############
# lint
add_library(lint STATIC
    # TODO: Add new detectors to this list in alphabetical order
    detectors/AvoidCurryArgument.hpp
    detectors/AvoidMethodReturnInNamedFunctions.hpp
    detectors/Detector.hpp
    detectors/ExampleDetector.hpp
    detectors/LintTest.hpp
    detectors/OneNewlineAtEndOfFile.hpp

    Config.cpp
    Config.hpp
    Issue.hpp
    ListenerMux.cpp
    ListenerMux.hpp
    Linter.cpp
    Linter.hpp
)

target_compile_definitions(lint PUBLIC ANTLR4CPP_STATIC)

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
    # using /MD flag for antlr4_runtime (for Visual C++ compilers only)
    set(ANTLR4_WITH_STATIC_CRT OFF)
endif()

target_compile_options(lint PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
     -Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:
     /W4 /WX /DNOMINMAX>
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
target_link_libraries(lint embind)
find_program(EMCC_BINARY emcc REQUIRED)
find_program(EMCXX_BINARY em++ REQUIRED)
add_custom_command(OUTPUT Embind.cpp.o
    COMMAND ${EMCXX_BINARY} -c ${CMAKE_CURRENT_SOURCE_DIR}/Embind.cpp -o Embind.cpp.o
    DEPENDS Embind.cpp lint
)
add_custom_target(lint.js
                  COMMAND ${EMCC_BINARY} -lembind -o lint.js Embind.cpp.o --whole-archive liblint.a
                        ${CMAKE_BINARY_DIR}/third_party/fmt/libfmt.a
                        ${CMAKE_BINARY_DIR}/third_party/sparkler/grammar/libsprklr.a
                        ${CMAKE_BINARY_DIR}/third_party/sparkler/antlr4_runtime/src/antlr4_runtime/runtime/Cpp/runtime/libantlr4-runtime.a
                        --no-whole-archive
                  DEPENDS lint Embind.cpp.o)
endif()