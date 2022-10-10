#ifndef SRC_LINT_DETECTORS_DETECTOR_HPP_
#define SRC_LINT_DETECTORS_DETECTOR_HPP_

#include "Issue.hpp"

#include "SCParserListener.h"
#include "SCParserBaseListener.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace lint {

struct Config;

class Detector : public sprklr::SCParserBaseListener {
public:
    Detector() = default;
    virtual ~Detector() = default;

    void setIssues(std::vector<Issue>* issues) { m_issues = issues; }
    void setConfig(const Config* config) { m_config = config; }

protected:
    std::vector<Issue>* m_issues = nullptr;
    const Config* m_config;
};

class DetectorMux : public sprklr::SCParserBaseListener {
public:
    DetectorMux() = delete;
    DetectorMux(const Config* config);
    ~DetectorMux() = default;

    void clearDetectors();
    void addDetector(std::unique_ptr<Detector> detector);

    const std::vector<Issue>& issues() const { return m_issues; }

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

    void visitTerminal(antlr4::tree::TerminalNode* node) override;

private:
    const Config* m_config;
    std::vector<std::unique_ptr<sprklr::SCParserListener>> m_detectors;
    std::vector<Issue> m_issues;
};

} // namespace lint

#endif // SRC_LINT_DETECTORS_DETECTOR_HPP_