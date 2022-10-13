include(CTest)

# OneNewlineAtEndOfFile
add_test(NAME OneNewlineAtEndOfFile_no_newline COMMAND
    sclint "--style=file" --printConfig "${CMAKE_CURRENT_SOURCE_DIR}/OneNewlineAtEndOfFile/no_newline.scd"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/OneNewlineAtEndOfFile"
)

# NoMethodReturnInLexicalScope
add_test(NAME MethodReturnInLexicalScope_true_positive COMMAND
    sclint "--style=file" "${CMAKE_CURRENT_SOURCE_DIR}/NoMethodReturnWithLexicalScope/true_positive.scd"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/NoMethodReturnWithLexicalScope"
)
add_test(NAME MethodReturnInLexicalScope_false_positive COMMAND
    sclint "--style=file" "${CMAKE_CURRENT_SOURCE_DIR}/NoMethodReturnWithLexicalScope/false_positive.scd"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/NoMethodReturnWithLexicalScope"
)
