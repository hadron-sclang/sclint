#ifndef SRC_LINT_LINTER_HPP_
#define SRC_LINT_LINTER_HPP_

#include "Issue.hpp"
#include "ListenerMux.hpp"

#include <memory>
#include <string_view>
#include <string>
#include <vector>

namespace lint {

struct Config;
class ListenerMux;

class Linter {
public:
    Linter() = delete;
    Linter(const Config* config, std::string_view code);
    ~Linter() = default;

    // Returns the most severe linter issue encountered.
    IssueSeverity lint();

    void addIssue(Issue&& issue);
    void addExpectedIssue(Issue&& issue);

    const Config* config() { return m_config; }
    const std::vector<Issue>& issues() const { return m_issues; }
    const std::string_view rewrittenString() const { return std::string_view(m_rewritten.data(), m_rewritten.size()); }

private:
    const Config* m_config;
    std::string_view m_code;
    std::vector<Issue> m_issues;
    std::vector<Issue> m_expectedIssues;
    ListenerMux m_mux;
    IssueSeverity m_lowestSeverity;
    std::string m_rewritten;
};

} // namespace lint

#endif // SRC_LINT_LINTER_HPP_