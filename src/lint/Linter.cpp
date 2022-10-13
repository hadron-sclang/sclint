#include "Linter.hpp"

#include "Config.hpp"
#include "detectors/LintTest.hpp"
#include "detectors/MethodReturnLexicalScope.hpp"

#include "SCLexer.h"
#include "SCParser.h"

namespace lint {

Linter::Linter(const Config* config, std::string_view code): m_config(config), m_code(code) { }

bool Linter::lint() {
    antlr4::ANTLRInputStream input(m_code.data(), m_code.size());
    sprklr::SCLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    sprklr::SCParser parser(&tokens);
    if (parser.getNumberOfSyntaxErrors())
        return false;

    m_mux.clearDetectors();

    if (m_config->noMethodReturnWithLexicalScope)
        m_mux.addDetector(std::make_unique<MethodReturnLexicalScope>(m_config, &m_issues));

    if (m_config->lintTest)
        m_mux.addDetector(std::make_unique<LintTest>(m_config, &m_expectedIssues, &tokens));

    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&m_mux, parser.root());

    // Easier just to scan tokens for the last one being a newline.
    if (m_config->oneNewlineAtEndOfFile) {
        int newlineCount = 0;
        antlr4::Token* lastToken = nullptr;
        // Last token in the token stream is EOF, so skip it too.
        for (int i = static_cast<int>(tokens.size()) - 2; i >= 0; --i) {
            lastToken = tokens.get(i);
            if (lastToken->getType() == sprklr::SCParser::NEWLINE)
                ++newlineCount;
            else
                break;
        }
        assert(lastToken);
        if (newlineCount != 1) {
            m_issues.emplace_back(IssueNumber::kOneNewlineAtEndOfFile, lastToken->getLine(),
                                  lastToken->getCharPositionInLine() + lastToken->getStopIndex()
                                      - lastToken->getStartIndex() + 1);
        }
    }

    // Sort the issues by line and column number.
    std::sort(m_issues.begin(), m_issues.end());

    if (m_config->lintTest) {
        std::sort(m_expectedIssues.begin(), m_expectedIssues.end());
        // Copy issues to a new list, copying only those that aren't in the lintTest list as well.
        std::vector<Issue> filteredIssues;
        size_t testLintIndex = 0;
        size_t issueIndex = 0;
        while (testLintIndex < m_expectedIssues.size() || issueIndex < m_issues.size()) {
            if (testLintIndex == m_expectedIssues.size()) {
                filteredIssues.emplace_back(m_issues[issueIndex]);
                ++issueIndex;
            } else if (issueIndex == m_issues.size()) {
                filteredIssues.emplace_back(m_expectedIssues[testLintIndex]);
                ++testLintIndex;
            } else if (m_expectedIssues[testLintIndex] < m_issues[issueIndex]) {
                filteredIssues.emplace_back(kExpectedLintTestIssue, m_expectedIssues[testLintIndex].lineNumber,
                                            m_expectedIssues[testLintIndex].columnNumber);
                ++testLintIndex;
            } else if (m_expectedIssues[testLintIndex] == m_issues[issueIndex]) {
                // Advance both indices past the match without copying.
                ++testLintIndex;
                ++issueIndex;
            } else {
                filteredIssues.emplace_back(m_issues[issueIndex]);
                ++issueIndex;
            }
        }

        m_issues.swap(filteredIssues);
    }

    return true;
}

} // namespace lint
