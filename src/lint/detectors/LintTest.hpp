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
            m_tokenScanIndex = token->getTokenIndex() + 1;

            // Expecting "//+//: detectorName, columnNumber"
            if (token->getType() != sprklr::SCParser::COMMENT_LINE)
                continue;

            auto lineNumber = token->getLine();

            auto commentString = token->toString();
            auto start = commentString.find("//+//:");
            if (start == commentString.npos)
                continue;

            start = start + sizeof("//+//:");

            // Consume any whitespace before name.
            while (commentString[start] == ' ' && start < commentString.size())
                ++start;
            auto comma = commentString.find(',', start);
            if (comma == commentString.npos) {
                m_linter->addIssue({ IssueSeverity::kError, lineNumber, token->getCharPositionInLine(), kOptionName,
                                     "malformed lint test comment, missing comma." });
                continue;
            }

            auto detectorName = commentString.substr(start, comma - start);
            if (comma + 1 >= commentString.size()) {
                m_linter->addIssue({ IssueSeverity::kError, lineNumber, token->getCharPositionInLine(), kOptionName,
                                     "malformed lint test comment, missing column number after comma." });
                continue;
            }

            size_t columnNumber = strtoul(commentString.data() + comma + 1, nullptr, 10);
            m_linter->addExpectedIssue({ IssueSeverity::kLint, lineNumber, columnNumber, detectorName });
        }
    }

    size_t m_tokenScanIndex;
};

} // namespace lint

#endif // SRC_LINT_DETECTORS_LINT_TEST_HPP_