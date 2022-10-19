#ifndef SRC_LINT_ISSUE_HPP_
#define SRC_LINT_ISSUE_HPP_

#include <cstdlib>
#include <array>

namespace lint {

enum IssueNumber : int32_t {
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

enum IssueSeverity : int32_t { kFatal = 0, kError = 1, kWarning = 2, kLint = 3, kNote = 4, kNone = 5 };

struct Issue {
    Issue() = delete;
    Issue(IssueNumber number, IssueSeverity severity, int32_t line, int32_t col):
        issueNumber(number), issueSeverity(severity), lineNumber(line), columnNumber(col) { }

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
    IssueSeverity issueSeverity;
    int32_t lineNumber;
    int32_t columnNumber;
};

} // namespace lint

#endif // SRC_LINT_ISSUE_HPP_