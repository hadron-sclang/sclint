include(CTest)

add_test(NAME MethodReturnInLexicalScope COMMAND
    sclint "--style={\"noMethodReturnWithLexicalScope\":true, \"lintTest\":true}"
        "${CMAKE_CURRENT_SOURCE_DIR}/noMethodReturnWithLexicalScope.scd")
