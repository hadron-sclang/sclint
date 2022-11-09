#ifndef SRC_LIST_DETECTORS_OMIT_TRAILING_WHITESPACE_HPP_
#define SRC_LIST_DETECTORS_OMIT_TRAILING_WHITESPACE_HPP_

#include "detectors/Detector.hpp"
#include "Linter.hpp"

#include <regex>

namespace lint {

class OmitTrailingWhitespace : public Detector {
public:
    OmitTrailingWhitespace() = delete;
    OmitTrailingWhitespace(Linter* linter, antlr4::CommonTokenStream* tokens, antlr4::TokenStreamRewriter* rewriter):
        Detector(linter, tokens, rewriter) { }
    virtual ~OmitTrailingWhitespace() = default;

    static constexpr const char* kOptionName = "omitTrailingWhitespace";
    static constexpr bool kDefaultValue = true;

    void visitTerminal(antlr4::tree::TerminalNode* node) override {
        // Look for whitespace tokens to the right of this one.
        auto whitespaceTokens = m_tokens->getHiddenTokensToRight(node->getSymbol()->getTokenIndex());
        // Moving from right to left, first identify a newline, then remove any whitespace left of that newline.
        bool foundNewline = false;
        for (int i = static_cast<int>(whitespaceTokens.size()) - 1; i >= 0; --i) {
            const antlr4::Token* token = whitespaceTokens[i];
            assert(token);
            const auto type = token->getType();
            if (type == sprklr::SCParser::NEWLINE || type == sprklr::SCParser::CARRIAGE_RETURN) {
                foundNewline = true;
            } else if (type == sprklr::SCParser::COMMENT_LINE || type == sprklr::SCParser::COMMENT_BLOCK) {
                // Reset the newline flag as we found some printing characters on this line.
                foundNewline = false;
                rewriteComment(token);
            } else if (foundNewline && (type == sprklr::SCParser::TAB || type == sprklr::SCParser::SPACE)) {
                m_rewriter->Delete(token->getTokenIndex());
                m_linter->addIssue({ IssueSeverity::kLint, token->getLine(), token->getCharPositionInLine(),
                                     kOptionName, "removing whitespace at end of line." });
            }
        }
    }

private:
    void rewriteComment(const antlr4::Token* token) {
        // TODO: this awkward code results from not being able to use std::regex in the WASM build. If we
        // end up adding a regex library that works in WASM, consider a refactor to use it here.
        const auto commentString = token->getText();
        std::string rewrittenString;
        size_t copyPosition = 0;
        size_t line = 0;
        size_t lastNewlineStart = 0;
        bool rewrite = false;

        // There are zero or more newlines inside a block comment, and exactly one newline in a line comment.
        auto newlineStart = commentString.find_first_of("\r\n");
        // We assume that a valid block or line comment could never start with a newline.
        assert(newlineStart == std::string::npos || newlineStart > 0);

        while (newlineStart != std::string::npos) {
            // Search left of the newline for any whitespace.
            int printingStart = static_cast<int>(newlineStart) - 1;
            for (; printingStart >= static_cast<int>(copyPosition); --printingStart) {
                if (commentString[printingStart] != ' ' && commentString[printingStart] != '\t')
                    break;
            }

            if (printingStart >= static_cast<int>(copyPosition)
                && (printingStart < (static_cast<int>(newlineStart) - 1))) {
                rewrite = true;
                size_t charPosition =
                    line == 0 ? token->getCharPositionInLine() + printingStart : printingStart - lastNewlineStart;
                rewrittenString.append(commentString.substr(copyPosition, printingStart - copyPosition + 1));
                m_linter->addIssue({ IssueSeverity::kLint, token->getLine() + line, charPosition, kOptionName,
                                     "removing whitespace at end of line within comment." });
                copyPosition = newlineStart;
            }

            // Increment line counter
            ++line;

            if (newlineStart + 1 >= commentString.size())
                break;

            lastNewlineStart = newlineStart;

            if (commentString[newlineStart] == '\r')
                newlineStart = commentString.find_first_of('\r', newlineStart + 1);
            else
                newlineStart = commentString.find_first_of('\n', newlineStart + 1);
        }

        if (rewrite) {
            rewrittenString.append(commentString.substr(copyPosition));
            m_rewriter->replace(token->getTokenIndex(), rewrittenString);
        }
    }
};

} // namespace lint

#endif // SRC_LIST_DETECTORS_OMIT_TRAILING_WHITESPACE_HPP_