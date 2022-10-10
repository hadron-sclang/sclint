#ifndef SRC_LINT_ISSUE_HPP_
#define SRC_LINT_ISSUE_HPP_

#include <array>

namespace lint {

enum IssueNumber : size_t { kMethodReturnInNamedFunction = 0, kCarriageReturnInFile = 1, kNumberOfIssueNumbers = 2 };

struct Issue {
    IssueNumber issueNumber;
    size_t lineNumber;
    size_t columnNumber;

    Issue(IssueNumber number, size_t line, size_t col): issueNumber(number), lineNumber(line), columnNumber(col) { }
};


static constexpr std::array<const char*, kNumberOfIssueNumbers> kIssueTextTable = {
    "method return in named function.",
    "carriage return character '\\r' in file, some tools will report different line numbers"
};


} // namespace lint

#endif // SRC_LINT_ISSUE_HPP_