#ifndef SRC_LIST_DETECTORS_ONE_NEWLINE_AT_END_OF_FILE_HPP_
#define SRC_LIST_DETECTORS_ONE_NEWLINE_AT_END_OF_FILE_HPP_

#include "detectors/Detector.hpp"
#include "Linter.hpp"

namespace lint {

class OneNewlineAtEndOfFile : public Detector {
public:
    OneNewlineAtEndOfFile() = delete;
    OneNewlineAtEndOfFile(Linter* linter, antlr4::CommonTokenStream* tokens, antlr4::TokenStreamRewriter* rewriter):
        Detector(linter, tokens, rewriter) { }
    virtual ~OneNewlineAtEndOfFile() = default;

    static constexpr const char* kOptionName = "oneNewlineAtEndOfFile";
    static constexpr bool kDefaultValue = true;

    void exitRoot(sprklr::SCParser::RootContext*) override {
        // Find the last meaningful (printing) token in the input, if any. The last token is always EOF so we skip it.
        antlr4::Token* lastToken = nullptr;
        for (int i = static_cast<int>(m_tokens->size()) - 2; i >= 0; --i) {
            lastToken = m_tokens->get(i);
            if (lastToken->getChannel() != 1)
                break;
        }

        // Case for empty file.
        if (!lastToken)
            return;

        // Special case for a whitespace-only input.
        if (lastToken == m_tokens->get(0)) {
            m_linter->addIssue({ IssueSeverity::kLint, 1, 1, kOptionName, "truncating whitespace-only file." });
            m_rewriter->Delete(0, m_tokens->size() - 1);
            return;
        }

        // Case with no whitespace between last printing token and EOF.
        if (lastToken->getTokenIndex() + 2 >= m_tokens->size()) {
            insertNewlineAfter(lastToken);
            m_linter->addIssue({ IssueSeverity::kLint, lastToken->getLine(), lastToken->getCharPositionInLine(),
                                 kOptionName, "inserting single newline at end of file." });
            return;
        }

        // Case with more than one nonprinting tokens between last token and EOF.
        if (lastToken->getTokenIndex() + 2 + kNewlineSize < m_tokens->size()) {
            m_rewriter->Delete(lastToken->getTokenIndex() + 1, m_tokens->size() - 2);
            insertNewlineAfter(lastToken);
            m_linter->addIssue({ IssueSeverity::kLint, lastToken->getLine(), lastToken->getCharPositionInLine(),
                                 kOptionName, "removing excess whitespace at end of file." });
            return;
        }


        // Only remaining case should be 2 nonprinting tokens between last token and EOF.
        assert(lastToken->getTokenIndex() + 2 + kNewlineSize == m_tokens->size());
#if WIN32
        auto firstToken = m_tokens->get(lastToken->getTokenIndex() + 1);
        auto secondToken = m_tokens->get(lastToken->getTokenIndex() + 2);
        if ((firstToken->getType() != sprklr::SCParser::CARRIAGE_RETURN)
            || (secondToken->getType() != sprklr::SCParser::NEWLINE)) {
            m_rewriter->replace(firstToken->getTokenIndex(), "\r");
            m_rewriter->replace(secondToken->getTokenIndex(), "\n");
            m_linter->addIssue(
                { IssueSeverity::kLint, firstToken->getLine(), firstToken->getCharPositionInLine(), kOptionName,
                  "replaced single terminating whitespace characters with a carriage return and newline." });
        }
#else
        auto token = m_tokens->get(lastToken->getTokenIndex() + 1);
        if (token->getType() != sprklr::SCParser::NEWLINE) {
            m_rewriter->replace(token->getTokenIndex(), "\n");
            m_linter->addIssue({ IssueSeverity::kLint, token->getLine(), token->getCharPositionInLine(), kOptionName,
                                 "replaced single terminating whitespace character with a newline." });
        }
#endif
    }

private:
#if WIN32
    inline void insertNewlineAfter(antlr4::Token* token) { m_rewriter->insertAfter(token, "\r\n"); }
    static constexpr size_t kNewlineSize = 2;
#else
    inline void insertNewlineAfter(antlr4::Token* token) { m_rewriter->insertAfter(token, "\n"); }
    static constexpr size_t kNewlineSize = 1;
#endif
};

} // namespace lint

#endif // SRC_LIST_DETECTORS_ONE_NEWLINE_AT_END_OF_FILE_HPP_