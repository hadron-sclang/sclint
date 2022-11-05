#ifndef SRC_LINT_LINTER_HPP_
#define SRC_LINT_LINTER_HPP_

#include "Issue.hpp"

#include <string>
#include <vector>

namespace lint {

class Config;

class Linter {
public:
    Linter();
    ~Linter() = default;

    // Returns the most severe linter issue encountered.
    IssueSeverity lint(const Config& config, const std::string& code);

    const std::vector<Issue>& issues() const { return m_issues; }
    const std::string& rewrittenString() const { return m_rewritten; }

    // Detector API
    void addIssue(Issue&& issue);
    void addExpectedIssue(Issue&& issue);

private:
    void filterExpectedIssues();

    std::vector<Issue> m_issues;
    std::vector<Issue> m_expectedIssues;
    IssueSeverity m_lowestSeverity;
    std::string m_rewritten;
};

} // namespace lint

#endif // SRC_LINT_LINTER_HPP_