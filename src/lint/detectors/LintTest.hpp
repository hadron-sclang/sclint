#ifndef SRC_LINT_DETECTORS_LINT_TEST_HPP_
#define SRC_LINT_DETECTORS_LINT_TEST_HPP_

#include "detectors/Detector.hpp"

#include <algorithm>

namespace lint {

class LintTest : public Detector {
public:
    LintTest() = delete;
    explicit LintTest(const Config* config, std::vector<Issue>* issues, antlr4::CommonTokenStream* tokens):
        Detector(config, issues), m_tokens(tokens), m_tokenScanIndex(0) { }
    virtual ~LintTest() = default;

    void visitTerminal(antlr4::tree::TerminalNode* node) override {
        auto tokenIndex = node->getSymbol()->getTokenIndex();
        if (m_tokenScanIndex < tokenIndex) {
            auto tokens = m_tokens->get(m_tokenScanIndex, tokenIndex);
            scanCommentTokens(tokens);
        }

        auto tokens = m_tokens->getHiddenTokensToRight(m_tokenScanIndex, 2);
        scanCommentTokens(tokens);
    }

private:
    void scanCommentTokens(const std::vector<antlr4::Token*>& tokens) {
        for (auto token : tokens) {
            if (token->getType() == sprklr::SCParser::COMMENT_LINE) {
                auto lineNumber = token->getLine();

                auto commentString = token->toString();
                auto start = commentString.find("//+//:");
                if (start != commentString.npos) {
                    start = start + sizeof("//+//:");
                    // Expecting two numbers separted by a comma. First is error number, second is character number.
                    char* endOfNumber = commentString.data() + start;
                    size_t issueNumber = strtoul(endOfNumber, &endOfNumber, 10);
                    size_t columnNumber = strtoul(endOfNumber + 1, nullptr, 10);
                    if (issueNumber == 0 || columnNumber == 0) {
                        m_issues->emplace_back(IssueNumber::kMalformedLintTestComment, lineNumber,
                                               token->getCharPositionInLine());
                    } else {
                        m_issues->emplace_back(static_cast<IssueNumber>(issueNumber), lineNumber, columnNumber);
                    }
                }
            }
            m_tokenScanIndex = token->getTokenIndex() + 1;
        }
    }

    antlr4::CommonTokenStream* m_tokens;
    size_t m_tokenScanIndex;
};

} // namespace lint

#endif // SRC_LINT_DETECTORS_LINT_TEST_HPP_