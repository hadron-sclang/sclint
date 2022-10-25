#ifndef SRC_LINT_DETECTORS_AVOID_CURRY_ARGUMENT_HPP_
#define SRC_LINT_DETECTORS_AVOID_CURRY_ARGUMENT_HPP_

#include "detectors/Detector.hpp"
#include "Linter.hpp"

#include <string>

namespace lint {

class AvoidCurryArgument : public Detector {
public:
    AvoidCurryArgument() = delete;
    AvoidCurryArgument(Linter* linter, antlr4::CommonTokenStream* tokens, antlr4::TokenStreamRewriter* rewriter):
        Detector(linter, tokens, rewriter) { }
    virtual ~AvoidCurryArgument() = default;

    static constexpr const char* kOptionName = "avoidCurryArgument";
    static constexpr bool kDefaultValue = false;

    void enterExprCurryArg(sprklr::SCParser::ExprCurryArgContext* ctx) override {
        m_linter->addIssue({ IssueSeverity::kWarning, ctx->UNDERSCORE()->getSymbol()->getLine(),
                             ctx->UNDERSCORE()->getSymbol()->getCharPositionInLine(), kOptionName,
                             "avoid use of the curry argument (_)." });
    }
};

} // namespace lint

#endif // SRC_LINT_DETECTORS_AVOID_CURRY_ARGUMENT_HPP_
