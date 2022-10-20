#ifndef SRC_LINT_DETECTORS_WARN_ON_CURRY_ARGUMENT_HPP_
#define SRC_LINT_DETECTORS_WARN_ON_CURRY_ARGUMENT_HPP_

#include "detectors/Detector.hpp"
#include "Linter.hpp"

#include <string>

namespace lint {

class WarnOnCurryArgument : public Detector {
public:
    WarnOnCurryArgument() = delete;
    WarnOnCurryArgument(Linter* linter, antlr4::TokenStreamRewriter* rewriter): Detector(linter, rewriter) { }
    virtual ~WarnOnCurryArgument() = default;

    void enterExprCurryArg(sprklr::SCParser::ExprCurryArgContext* ctx) override {
        m_linter->addIssue({ IssueNumber::kWarnOnCurryArgument, IssueSeverity::kWarning,
                             static_cast<int32_t>(ctx->UNDERSCORE()->getSymbol()->getLine()),
                             static_cast<int32_t>(ctx->UNDERSCORE()->getSymbol()->getCharPositionInLine()) });
    }
};

} // namespace lint

#endif // SRC_LINT_DETECTORS_WARN_ON_CURRY_ARGUMENT_HPP_
