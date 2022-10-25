#include "Linter.hpp"

#include "Config.hpp"
#include "detectors/DetectorList.hpp"
#include "ListenerMux.hpp"

#include "fmt/format.h"
#include "SCLexer.h"
#include "SCParser.h"

namespace lint {

template <typename T> struct TypeList<T> {
    static void addDetectors(const Config* config, ListenerMux* mux, Linter* linter, antlr4::CommonTokenStream* tokens,
                             antlr4::TokenStreamRewriter* rewriter) {
        if (config->getOptionNamed(T::kOptionName)) {
            mux->addDetector(std::make_unique<T>(linter, tokens, rewriter));
        }
    }
};

template <typename T, typename... Ts> struct TypeList {
    static void addDetectors(const Config* config, ListenerMux* mux, Linter* linter, antlr4::CommonTokenStream* tokens,
                             antlr4::TokenStreamRewriter* rewriter) {
        TypeList<T>::addDetectors(config, mux, linter, tokens, rewriter);
        TypeList<Ts...>::addDetectors(config, mux, linter, tokens, rewriter);
    }
};


Linter::Linter(const Config* config, std::string_view code):
    m_config(config), m_code(code), m_lowestSeverity(IssueSeverity::kNone) { }

IssueSeverity Linter::lint() {
    antlr4::ANTLRInputStream input(m_code.data(), m_code.size());
    sprklr::SCLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    antlr4::TokenStreamRewriter rewriter(&tokens);
    sprklr::SCParser parser(&tokens);
    if (parser.getNumberOfSyntaxErrors()) {
        m_lowestSeverity = IssueSeverity::kFatal;
        return IssueSeverity::kFatal; // TODO: relay parse errors to user?
    }

    m_mux.clearDetectors();

    DetectorList::addDetectors(m_config, &m_mux, this, &tokens, &rewriter);

    // Walk the parser tree with all detectors installed in the multiplexer.
    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&m_mux, parser.root());

    // Sort the issues by line and column number.
    std::sort(m_issues.begin(), m_issues.end());

    // If testing is enabled, compare the list of expected issues to actual issues, and treat differences as an error.
    if (m_config->getOptionNamed(LintTest::kOptionName))
        filterExpectedIssues();

    m_rewritten = rewriter.getText();
    return m_lowestSeverity;
}

void Linter::addIssue(Issue&& issue) {
    m_lowestSeverity = std::min(m_lowestSeverity, issue.issueSeverity);
    m_issues.push_back(issue);
}

void Linter::addExpectedIssue(Issue&& issue) { m_expectedIssues.push_back(issue); }

void Linter::filterExpectedIssues() {
    std::sort(m_expectedIssues.begin(), m_expectedIssues.end());
    m_lowestSeverity = IssueSeverity::kNone;

    // Copy issues to a new list, copying only those that aren't in the lintTest list as well.
    std::vector<Issue> filteredIssues;
    size_t testLintIndex = 0;
    size_t issueIndex = 0;
    while (testLintIndex < m_expectedIssues.size() || issueIndex < m_issues.size()) {
        if (testLintIndex == m_expectedIssues.size()) {
            // We have run through all the expected issues, copy all remaining encountered issues to the new list.
            m_lowestSeverity = std::min(m_lowestSeverity, m_issues[issueIndex].issueSeverity);
            filteredIssues.emplace_back(m_issues[issueIndex]);
            ++issueIndex;
        } else if (issueIndex == m_issues.size() || m_expectedIssues[testLintIndex] < m_issues[issueIndex]) {
            // This is a missing expected issue, treat as error.
            m_lowestSeverity = std::min(m_lowestSeverity, IssueSeverity::kError);
            filteredIssues.emplace_back(
                IssueSeverity::kError, m_expectedIssues[testLintIndex].lineNumber,
                m_expectedIssues[testLintIndex].columnNumber, LintTest::kOptionName,
                fmt::format("missing expected lint test issue '{}'.", m_expectedIssues[testLintIndex].detectorName));
            ++testLintIndex;
        } else if (m_expectedIssues[testLintIndex] == m_issues[issueIndex]) {
            // Match of expected issue and actual issue, advance both indices past the match without copying.
            ++testLintIndex;
            ++issueIndex;
        } else {
            // Unexpected issue, copy directly and propagate error (if it is an error).
            m_lowestSeverity = std::min(m_lowestSeverity, m_issues[issueIndex].issueSeverity);
            filteredIssues.emplace_back(m_issues[issueIndex]);
            ++issueIndex;
        }
    }

    m_issues.swap(filteredIssues);
}

} // namespace lint
