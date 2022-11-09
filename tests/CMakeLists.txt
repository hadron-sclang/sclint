include(CTest)

### TO ADD TESTS:
# Create a directory with the same name as your option flag, and add tests to that directory.
# This CMake script will add a test for every .scd file in the directory.

# For linter errors and warnings, you can add a comment on the same line as the warning that uses the
# following special format:
# //+//: <option flag name>,<column on this line where error is expected>
# and the linter will treat that as an expected error. See avoidMethodReturnInNamedFunctions/true_positive.scd for an
# example.

# For testing reformatting commands, if your test input is foo.scd, if you include a foo.scd.ref the output of sclint
# will be automatically compared to the foo.scd.ref file and the linter test will fail if there's any difference.
set(SCLINT_TEST_DIRS
    avoidCurryArgument
    avoidMethodReturnInNamedFunctions
    omitTrailingWhitespace
    oneNewlineAtEndOfFile
)

foreach(TEST_DIR ${SCLINT_TEST_DIRS})
    file(GLOB TEST_SCDS "${TEST_DIR}/*.scd")
    foreach(TEST_FILE ${TEST_SCDS})
        # if there's a reference file, check against it
        if(EXISTS "${TEST_FILE}.ref")
            set(CHECK_AGAINST "--checkAgainst" "${TEST_FILE}.ref")
        else()
            set(CHECK_AGAINST "")
        endif()

        if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
            add_test(NAME ${TEST_FILE} COMMAND
                ${PROJECT_SOURCE_DIR}/sclint-js/sclint.js --style "{\"lintTest\": true, \"${TEST_DIR}\": true}"
                    ${CHECK_AGAINST} "${TEST_FILE}"
            )
            set_tests_properties(${TEST_FILE} PROPERTIES
                ENVIRONMENT "NODE_PATH=${CMAKE_BINARY_DIR}/src/lint"
            )
        else()
            add_test(NAME ${TEST_FILE} COMMAND
                sclint "--style={\"lintTest\": true, \"${TEST_DIR}\": true}" ${CHECK_AGAINST} "${TEST_FILE}"
            )
        endif()
    endforeach()
endforeach()
