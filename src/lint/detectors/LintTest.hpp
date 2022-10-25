#ifndef SRC_LINT_DETECTORS_LINT_TEST_HPP_
#define SRC_LINT_DETECTORS_LINT_TEST_HPP_

#include "detectors/Detector.hpp"
#include "Linter.hpp"

#include <algorithm>

namespace lint {

class LintTest : public Detector {
public:
    LintTest() = delete;
    LintTest(Linter* linter, antlr4::CommonTokenStream* tokens, antlr4::TokenStreamRewriter* rewriter):
        Detector(linter, tokens, rewriter), m_tokenScanIndex(0) { }
    virtual ~LintTest() = default;

    static constexpr const char* kOptionName = "lintTest";
    static constexpr bool kDefaultValue = false;

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
                        m_linter->addIssue({ IssueNumber::kMalformedLintTestComment, IssueSeverity::kError,
                                             static_cast<int32_t>(lineNumber),
                                             static_cast<int32_t>(token->getCharPositionInLine()) });
                    } else {
                        m_linter->addExpectedIssue({ static_cast<IssueNumber>(issueNumber), IssueSeverity::kLint,
                                                     static_cast<int32_t>(lineNumber),
                                                     static_cast<int32_t>(columnNumber) });
                    }
                }
            }
            m_tokenScanIndex = token->getTokenIndex() + 1;
        }
    }

    size_t m_tokenScanIndex;
};

} // namespace lint

#endif // SRC_LINT_DETECTORS_LINT_TEST_HPP_