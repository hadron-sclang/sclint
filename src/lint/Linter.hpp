#ifndef SRC_LINT_LINTER_HPP_
#define SRC_LINT_LINTER_HPP_

#include "Issue.hpp"
#include "ListenerMux.hpp"

#ifdef EMSCRIPTEN
#    include <emscripten/bind.h>
#endif

#include <memory>
#include <string_view>
#include <string>
#include <vector>

namespace lint {

class Config;
class ListenerMux;

class Linter {
public:
    Linter();
    ~Linter() = default;

    // Returns the most severe linter issue encountered.
    IssueSeverity lint(const Config& config, std::string_view code);

    const std::vector<Issue>& issues() const { return m_issues; }
    const std::string_view rewrittenString() const { return std::string_view(m_rewritten.data(), m_rewritten.size()); }

    // Detector API
    void addIssue(Issue&& issue);
    void addExpectedIssue(Issue&& issue);

private:
    void filterExpectedIssues();

    std::vector<Issue> m_issues;
    std::vector<Issue> m_expectedIssues;
    ListenerMux m_mux;
    IssueSeverity m_lowestSeverity;
    std::string m_rewritten;
};

#ifdef EMSCRIPTEN
EMSCRIPTEN_BINDINGS(sclint_linter_binding) {
    emscripten::class_<Linter>("Linter")
        .constructor()
        .function("lint", &Linter::lint)
        .function("issues", &Linter::issues)
        .function("rewrittenString", &Linter::rewrittenString);
}
#endif

} // namespace lint

#endif // SRC_LINT_LINTER_HPP_