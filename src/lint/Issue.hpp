#ifndef SRC_LINT_ISSUE_HPP_
#define SRC_LINT_ISSUE_HPP_

#include <array>

namespace lint {

enum IssueNumber : size_t {
    kMethodReturnInLexicalScope = 1,
    kOneNewlineAtEndOfFile = 2,
    kMalformedLintTestComment = 3,
    kMissingExpectedLintTestIssue = 4,
    kExpectedLintTestIssue = 5,
    kNumberOfIssueNumbers = 6
};

static constexpr std::array<const char*, kNumberOfIssueNumbers> kIssueTextTable = {
    "no error.", // reserve zero to indicate bad error code.
    "method return in lexical scope.", // kMethodReturnInLexicalScope
    "use exactly one newline at the end of a file", // kOneNewlineAtEndOfFile
    "malformed lint test comment", // kMalformedLintTestComment
    "expected lint test issue", // kExpectedLintTestIssue
    "missing expected lint test issue" // kMissingExpectedLintTestIssue
};

struct Issue {
    Issue(IssueNumber number, size_t line, size_t col): issueNumber(number), lineNumber(line), columnNumber(col) { }

    bool operator==(const Issue& i) const {
        return issueNumber == i.issueNumber && lineNumber == i.lineNumber && columnNumber == i.columnNumber;
    }
    bool operator<(const Issue& i) const {
        if (lineNumber < i.lineNumber)
            return true;
        if (lineNumber > i.lineNumber)
            return false;
        return columnNumber < i.columnNumber;
    }

    IssueNumber issueNumber;
    size_t lineNumber;
    size_t columnNumber;
};

} // namespace lint

#endif // SRC_LINT_ISSUE_HPP_