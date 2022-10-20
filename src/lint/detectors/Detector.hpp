#ifndef SRC_LINT_DETECTORS_DETECTOR_HPP_
#define SRC_LINT_DETECTORS_DETECTOR_HPP_

#include "Issue.hpp"

#include "SCParserListener.h"
#include "SCParserBaseListener.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace antlr4 {
class TokenStreamRewriter;
}

namespace lint {

struct Config;
class Linter;

class Detector : public sprklr::SCParserBaseListener {
public:
    Detector() = delete;
    Detector(Linter* linter, antlr4::TokenStreamRewriter* rewriter): m_linter(linter), m_rewriter(rewriter) { }
    virtual ~Detector() = default;

protected:
    Linter* m_linter;
    antlr4::TokenStreamRewriter* m_rewriter;
};

class DetectorMux : public sprklr::SCParserBaseListener {
public:
    DetectorMux() = default;
    ~DetectorMux() = default;

    void clearDetectors();
    void addDetector(std::unique_ptr<Detector> detector);

    // SCParserListener overrides
    void enterRoot(sprklr::SCParser::RootContext* ctx) override;
    void exitRoot(sprklr::SCParser::RootContext* ctx) override;

    void enterTopLevelStatement(sprklr::SCParser::TopLevelStatementContext* ctx) override;
    void exitTopLevelStatement(sprklr::SCParser::TopLevelStatementContext* ctx) override;

    void enterClassDef(sprklr::SCParser::ClassDefContext* ctx) override;
    void exitClassDef(sprklr::SCParser::ClassDefContext* ctx) override;

    void enterVarDef(sprklr::SCParser::VarDefContext* ctx) override;
    void exitVarDef(sprklr::SCParser::VarDefContext* ctx) override;

    void enterExprBinop(sprklr::SCParser::ExprBinopContext* ctx) override;
    void exitExprBinop(sprklr::SCParser::ExprBinopContext* ctx) override;

    void enterExprAssignDotName(sprklr::SCParser::ExprAssignDotNameContext* ctx) override;
    void exitExprAssignDotName(sprklr::SCParser::ExprAssignDotNameContext* ctx) override;

    void enterExprAssignGlobal(sprklr::SCParser::ExprAssignGlobalContext* ctx) override;
    void exitExprAssignGlobal(sprklr::SCParser::ExprAssignGlobalContext* ctx) override;

    void enterExprAssign(sprklr::SCParser::ExprAssignContext* ctx) override;
    void exitExprAssign(sprklr::SCParser::ExprAssignContext* ctx) override;

    void enterBlock(sprklr::SCParser::BlockContext* ctx) override;
    void exitBlock(sprklr::SCParser::BlockContext* ctx) override;

    void enterBlockArgList(sprklr::SCParser::BlockArgListContext* ctx) override;
    void exitBlockArgList(sprklr::SCParser::BlockArgListContext* ctx) override;

    void enterBody(sprklr::SCParser::BodyContext* ctx) override;
    void exitBody(sprklr::SCParser::BodyContext* ctx) override;

    void enterKeyArgList(sprklr::SCParser::KeyArgListContext* ctx) override;
    void exitKeyArgList(sprklr::SCParser::KeyArgListContext* ctx) override;

    void enterExprCurryArg(sprklr::SCParser::ExprCurryArgContext* ctx) override;
    void exitExprCurryArg(sprklr::SCParser::ExprCurryArgContext* ctx) override;

    void visitTerminal(antlr4::tree::TerminalNode* node) override;

private:
    std::vector<std::unique_ptr<sprklr::SCParserListener>> m_detectors;
};

} // namespace lint

#endif // SRC_LINT_DETECTORS_DETECTOR_HPP_
