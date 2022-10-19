#include "Linter.hpp"

#include "Config.hpp"
#include "detectors/LintTest.hpp"
#include "detectors/MethodReturnLexicalScope.hpp"

#include "SCLexer.h"
#include "SCParser.h"

namespace lint {

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

    if (m_config->noMethodReturnWithLexicalScope)
        m_mux.addDetector(std::make_unique<MethodReturnLexicalScope>(this, &rewriter));

    if (m_config->lintTest)
        m_mux.addDetector(std::make_unique<LintTest>(this, &rewriter, &tokens));

    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&m_mux, parser.root());

    // Easier just to scan tokens for the last one being a newline.
    if (m_config->oneNewlineAtEndOfFile) {
        int newlineCount = 0;
        int carriageReturnCount = 0;
        antlr4::Token* lastToken = nullptr;
        // Last token in the token stream is EOF, so skip it too.
        for (int i = static_cast<int>(tokens.size()) - 2; i >= 0; --i) {
            lastToken = tokens.get(i);
            if (lastToken->getType() == sprklr::SCParser::CARRIAGE_RETURN)
                ++carriageReturnCount;
            else if (lastToken->getType() == sprklr::SCParser::NEWLINE)
                ++newlineCount;
            else
                break;
        }
        assert(lastToken);
        if (newlineCount != 1) {
            m_issues.emplace_back(IssueNumber::kOneNewlineAtEndOfFile, IssueSeverity::kLint,
                                  static_cast<int32_t>(lastToken->getLine()),
                                  static_cast<int32_t>(lastToken->getCharPositionInLine() + lastToken->getStopIndex()
                                                       - lastToken->getStartIndex() + 1));
            if (newlineCount == 0) {
#if WIN32
                rewriter.insertAfter(lastToken, "\r\n");
#else
                rewriter.insertAfter(lastToken, "\n");
#endif // WIN32
            } else {
                if (carriageReturnCount > 0)
                    rewriter.Delete(lastToken->getTokenIndex() + 2,
                                    lastToken->getTokenIndex() + newlineCount + carriageReturnCount - 1);
                else
                    rewriter.Delete(lastToken->getTokenIndex() + 1, lastToken->getTokenIndex() + newlineCount - 1);
            }
        }
    }

    // Sort the issues by line and column number.
    std::sort(m_issues.begin(), m_issues.end());

    if (m_config->lintTest) {
        std::sort(m_expectedIssues.begin(), m_expectedIssues.end());
        m_lowestSeverity = IssueSeverity::kNone;
        // Copy issues to a new list, copying only those that aren't in the lintTest list as well.
        std::vector<Issue> filteredIssues;
        size_t testLintIndex = 0;
        size_t issueIndex = 0;
        while (testLintIndex < m_expectedIssues.size() || issueIndex < m_issues.size()) {
            if (testLintIndex == m_expectedIssues.size()) {
                m_lowestSeverity = std::min(m_lowestSeverity, m_issues[issueIndex].issueSeverity);
                filteredIssues.emplace_back(m_issues[issueIndex]);
                ++issueIndex;
            } else if (issueIndex == m_issues.size()) {
                m_lowestSeverity = std::min(m_lowestSeverity, m_expectedIssues[testLintIndex].issueSeverity);
                filteredIssues.emplace_back(m_expectedIssues[testLintIndex]);
                ++testLintIndex;
            } else if (m_expectedIssues[testLintIndex] < m_issues[issueIndex]) {
                m_lowestSeverity = std::min(m_lowestSeverity, IssueSeverity::kError);
                filteredIssues.emplace_back(kExpectedLintTestIssue, IssueSeverity::kError,
                                            m_expectedIssues[testLintIndex].lineNumber,
                                            m_expectedIssues[testLintIndex].columnNumber);
                ++testLintIndex;
            } else if (m_expectedIssues[testLintIndex] == m_issues[issueIndex]) {
                // Advance both indices past the match without copying.
                ++testLintIndex;
                ++issueIndex;
            } else {
                m_lowestSeverity = std::min(m_lowestSeverity, m_issues[issueIndex].issueSeverity);
                filteredIssues.emplace_back(m_issues[issueIndex]);
                ++issueIndex;
            }
        }

        m_issues.swap(filteredIssues);
    }

    m_rewritten = rewriter.getText();
    return m_lowestSeverity;
}

void Linter::addIssue(Issue&& issue) {
    m_lowestSeverity = std::min(m_lowestSeverity, issue.issueSeverity);
    m_issues.push_back(issue);
}

void Linter::addExpectedIssue(Issue&& issue) { m_expectedIssues.push_back(issue); }

} // namespace lint
