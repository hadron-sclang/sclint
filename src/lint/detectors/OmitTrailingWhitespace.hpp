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
            const auto token = whitespaceTokens[i];
            const auto type = token->getType();
            if (type == sprklr::SCParser::NEWLINE || type == sprklr::SCParser::CARRIAGE_RETURN) {
                foundNewline = true;
                continue;
            }

            if (foundNewline && (type == sprklr::SCParser::TAB || type == sprklr::SCParser::SPACE)) {
                m_rewriter->Delete(token->getTokenIndex());
                m_linter->addIssue({ IssueSeverity::kLint, token->getLine(), token->getCharPositionInLine(),
                                     kOptionName, "removing whitespace at end of line." });
                continue;
            }

            if (type == sprklr::SCParser::COMMENT_LINE || type == sprklr::SCParser::COMMENT_BLOCK) {
                foundNewline = false;
                // TODO: this awkward code results from not being able to use std::regex in the WASM build. If we
                // end up adding a regex library that works in WASM, consider a refactor to use it here.
                auto commentString = token->getText();
                std::string rewrittenString;
                size_t copyPosition = 0;
                size_t line = 0;
                size_t charPosition = token->getCharPositionInLine();
                // There are zero or more newlines inside a block comment, and exactly one newline in a line comment.
                auto newlineStart = commentString.find_first_of("\r\n");
                bool rewrite = false;
                while (newlineStart != std::string::npos) {
                    // Search left of the newline for any whitespace.
                    int printingStart = static_cast<int>(newlineStart) - 1;
                    for (; printingStart >= 0; --printingStart) {
                        if (commentString[printingStart] != ' ' && commentString[printingStart] != '\t')
                            break;
                    }
                    if (printingStart >= 0 && printingStart < static_cast<int>(newlineStart) - 1) {
                        rewrite = true;
                        rewrittenString.append(commentString.substr(copyPosition, printingStart - copyPosition + 1));
                        m_linter->addIssue({ IssueSeverity::kLint, token->getLine() + line,
                                             charPosition + printingStart, kOptionName,
                                             "removing whitespace at end of line within comment." });
                        copyPosition = newlineStart;
                    }

                    // Increment line and reset character counter for next line.
                    ++line;
                    charPosition = 0;

                    // Skip over the newline assumed to be the next character in the stream.
                    if (commentString[newlineStart] == '\r')
                        newlineStart = commentString.find_first_of("\r\n", newlineStart + 2);
                    else
                        newlineStart = commentString.find_first_of("\n", newlineStart + 1);
                }

                if (rewrite) {
                    rewrittenString.append(commentString.substr(copyPosition));
                    m_rewriter->replace(token->getTokenIndex(), rewrittenString);
                }
            }
        }
    }
};

} // namespace lint

#endif // SRC_LIST_DETECTORS_OMIT_TRAILING_WHITESPACE_HPP_