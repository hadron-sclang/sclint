#ifndef SRC_LINT_ISSUE_HPP_
#define SRC_LINT_ISSUE_HPP_

#include <cstdlib>
#include <string>

namespace lint {

enum IssueSeverity : int32_t { kFatal = 0, kError = 1, kWarning = 2, kLint = 3, kNote = 4, kNone = 5 };

struct Issue {
#ifdef EMSCRIPTEN
    // Embind doesn't seem to support a deleted default ctor for "value objects" like this Issue struct, so we
    // define a default ctor for this configuration only, then delete it for non-emscripten builds, which will
    // hopefully continue to prevent its use in shared code.
    Issue(): issueSeverity(kNone), lineNumber(0), columnNumber(0) { }
#else
    Issue() = delete;
#endif

    Issue(IssueSeverity severity, size_t line, size_t col, std::string name, std::string msg = ""):
        issueSeverity(severity), lineNumber(line), columnNumber(col), detectorName(name), message(msg) { }

    bool operator==(const Issue& i) const {
        return lineNumber == i.lineNumber && columnNumber == i.columnNumber && detectorName == i.detectorName;
    }
    bool operator<(const Issue& i) const {
        if (lineNumber < i.lineNumber)
            return true;
        if (lineNumber > i.lineNumber)
            return false;
        return columnNumber < i.columnNumber;
    }

    IssueSeverity issueSeverity;
    size_t lineNumber;
    size_t columnNumber;
    std::string detectorName;
    std::string message; // optional message
};

} // namespace lint

#endif // SRC_LINT_ISSUE_HPP_
