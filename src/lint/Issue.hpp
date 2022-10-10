#ifndef SRC_LINT_ISSUE_HPP_
#define SRC_LINT_ISSUE_HPP_

#include <string>

namespace lint {

struct Issue {
    size_t lineNumber;
    size_t columnNumber;
    std::string message;

    Issue(size_t line, size_t col, std::string msg): lineNumber(line), columnNumber(col), message(msg) { }
};

} // namespace lint

#endif // SRC_LINT_ISSUE_HPP_