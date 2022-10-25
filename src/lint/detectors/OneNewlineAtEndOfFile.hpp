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
        int newlineCount = 0;
        int carriageReturnCount = 0;
        antlr4::Token* lastToken = nullptr;
        // Last token in the token stream is EOF, so skip it too.
        for (int i = static_cast<int>(m_tokens->size()) - 2; i >= 0; --i) {
            lastToken = m_tokens->get(i);
            if (lastToken->getType() == sprklr::SCParser::CARRIAGE_RETURN)
                ++carriageReturnCount;
            else if (lastToken->getType() == sprklr::SCParser::NEWLINE)
                ++newlineCount;
            else
                break;
        }
        assert(lastToken);
        if (newlineCount != 1) {
            m_linter->addIssue({ IssueNumber::kOneNewlineAtEndOfFile, IssueSeverity::kLint,
                                 static_cast<int32_t>(lastToken->getLine()),
                                 static_cast<int32_t>(lastToken->getCharPositionInLine() + lastToken->getStopIndex()
                                                      - lastToken->getStartIndex() + 1) });
            if (newlineCount == 0) {
#if WIN32
                rewriter.insertAfter(lastToken, "\r\n");
#else
                m_rewriter->insertAfter(lastToken, "\n");
#endif // WIN32
            } else {
                if (carriageReturnCount > 0)
                    m_rewriter->Delete(lastToken->getTokenIndex() + 2,
                                       lastToken->getTokenIndex() + newlineCount + carriageReturnCount - 1);
                else
                    m_rewriter->Delete(lastToken->getTokenIndex() + 1, lastToken->getTokenIndex() + newlineCount - 1);
            }
        }
    }
};

} // namespace lint

#endif // SRC_LIST_DETECTORS_ONE_NEWLINE_AT_END_OF_FILE_HPP_