#ifndef SRC_LINT_DETECTORS_WHITESPACE_HPP_
#define SRC_LINT_DETECTORS_WHITESPACE_HPP_

#include "detectors/Detector.hpp"

#include "fmt/format.h"

#include <string>

namespace lint {

class Whitespace : public Detector {
public:
    Whitespace() = delete;
    explicit Whitespace(antlr4::CommonTokenStream* tokens): Detector(), m_tokens(tokens), m_tokenScanIndex(0) { }
    virtual ~Whitespace() = default;

    void visitTerminal(antlr4::tree::TerminalNode* node) override {
        auto tokenIndex = node->getSymbol()->getTokenIndex();

        // Grab any tokens we may have skipped from before this terminal.
        if (m_tokenScanIndex < tokenIndex) {
            auto tokens = m_tokens->get(m_tokenScanIndex, tokenIndex);
            scanWhitespaceTokens(tokens);
        }

        // Look for any to the right of this.
        auto tokens = m_tokens->getHiddenTokensToRight(m_tokenScanIndex, 1);
        scanWhitespaceTokens(tokens);
    }

private:
    void scanWhitespaceTokens(const std::vector<antlr4::Token*>& whitespaceTokens) {
        for (auto token : whitespaceTokens) {
            if (token->getType() == sprklr::SCParser::CARRIAGE_RETURN) {
                m_issues->emplace_back(IssueNumber::kCarriageReturnInFile, token->getLine(),
                                       token->getCharPositionInLine());
            }
            m_tokenScanIndex = token->getTokenIndex() + 1;
        }
    }

    antlr4::CommonTokenStream* m_tokens;
    size_t m_tokenScanIndex;
};

} // namespace lint

#endif // SRC_LINT_DETECTORS_WHITESPACE_HPP_
