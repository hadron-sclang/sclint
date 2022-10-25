include(CTest)

# AvoidCurryArgument
add_test(NAME AvoidCurryArgument_true_positive COMMAND
    sclint "--style=file" "true_positive.scd"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/AvoidCurryArgument"
)

# AvoidMethodReturnInNamedFunctions
add_test(NAME AvoidMethodReturnInNamedFunctions_true_positive COMMAND
    sclint "--style=file" "${CMAKE_CURRENT_SOURCE_DIR}/AvoidMethodReturnInNamedFunctions/true_positive.scd"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/AvoidMethodReturnInNamedFunctions"
)
add_test(NAME AvoidMethodReturnInNamedFunctions_false_positive COMMAND
    sclint "--style=file" "${CMAKE_CURRENT_SOURCE_DIR}/AvoidMethodReturnInNamedFunctions/false_positive.scd"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/AvoidMethodReturnInNamedFunctions"
)

# OneNewlineAtEndOfFile
add_test(NAME OneNewlineAtEndOfFile_no_newline COMMAND
    sclint "--style=file" "--checkAgainst=no_newline.scd.ref" "no_newline.scd"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/OneNewlineAtEndOfFile"
)
add_test(NAME OneNewlineAtEndOfFile_one_newline COMMAND
    sclint "--style=file" "--checkAgainst=one_newline.scd" "one_newline.scd"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/OneNewlineAtEndOfFile"
)
add_test(NAME OneNewlineAtEndOfFile_two_newlines COMMAND
    sclint "--style=file" "--checkAgainst=two_newlines.scd.ref" "two_newlines.scd"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/OneNewlineAtEndOfFile"
)
add_test(NAME OneNewlineAtEndOfFile_n_newlines COMMAND
    sclint "--style=file" "--checkAgainst=n_newlines.scd.ref" "n_newlines.scd"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/OneNewlineAtEndOfFile"
)
