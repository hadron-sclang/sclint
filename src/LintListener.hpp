#ifndef SCLINT_SRC_LINT_LISTENER_HPP_
#define SCLINT_SRC_LINT_LISTENER_HPP_

#include "SCParserBaseListener.h"

#include <iostream>

namespace sclint {

class LintListener : public sprklr::SCParserBaseListener {
public:
    LintListener():
        sprklr::SCParserBaseListener(),
        m_inBlock(false) {}
    virtual ~LintListener() = default;

    void enterBlock(sprklr::SCParser::BlockContext* /* ctx */) override { m_inBlock = true; }
    void exitBlock(sprklr::SCParser::BlockContext* /* ctx */) override { m_inBlock = false; }

    void enterBody(sprklr::SCParser::BodyContext* ctx) override {
        if (!m_inBlock)
            return;

        if (ctx->returnExpr()) {
            std::cerr << ctx->returnExpr()->start->getLine() << ":"
                      << ctx->returnExpr()->start->getCharPositionInLine() << std::endl;
        }
    }

private:
    bool m_inBlock;
};

} // namespace sclint

#endif // SCLING_SRC_LINT_LISTENER_HPP_
