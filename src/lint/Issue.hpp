#ifndef SRC_LINT_ISSUE_HPP_
#define SRC_LINT_ISSUE_HPP_

#include <array>

namespace lint {

enum IssueNumber : size_t {
    kMethodReturnInLexicalScope = 1,
    kCarriageReturnInFile = 2,
    kMalformedLintTestComment = 3,
    kMissingExpectedLintTestIssue = 4,
    kNumberOfIssueNumbers = 5
};

static constexpr std::array<const char*, kNumberOfIssueNumbers> kIssueTextTable = {
    "no error.", // reserve zero to indicate bad error code.
    "method return in lexical scope.", // kMethodReturnInLexicalScope
    "carriage return character '\\r' in file, some tools will report different line numbers", // kCarriageReturnInFile
    "malformed lint test comment", // kMalformedLintTestComment
    "missing expected lint test issue" // kMissingExpectedLintTestIssue
};


struct Issue {
    Issue(IssueNumber number, size_t line, size_t col): issueNumber(number), lineNumber(line), columnNumber(col) { }

    bool operator==(const Issue& i) {
        return issueNumber == i.issueNumber && lineNumber == i.lineNumber && columnNumber == i.columnNumber;
    }

    IssueNumber issueNumber;
    size_t lineNumber;
    size_t columnNumber;
};

} // namespace lint

#endif // SRC_LINT_ISSUE_HPP_