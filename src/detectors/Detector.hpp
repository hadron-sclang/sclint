#ifndef SCLINT_SRC_DETECTORS_DETECTOR_HPP_
#define SCLINT_SRC_DETECTORS_DETECTOR_HPP_

#include "SCParserListener.h"
#include "SCParserBaseListener.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace sclint {

struct Issue {
    size_t lineNumber;
    size_t columnNumber;
    std::string message;

    Issue(size_t line, size_t col, std::string msg): lineNumber(line), columnNumber(col), message(msg) { }
};

class Detector : public sprklr::SCParserBaseListener {
public:
    Detector() = default;
    virtual ~Detector() = default;

    void setIssues(std::vector<Issue>* issues) { m_issues = issues; }

protected:
    std::vector<Issue>* m_issues = nullptr;
};

class DetectorMux : public sprklr::SCParserBaseListener {
public:
    DetectorMux() = default;
    ~DetectorMux() = default;

    void addDetector(std::unique_ptr<Detector> detector) {
        detector->setIssues(&m_issues);
        m_detectors.push_back(std::move(detector));
    }

    const std::vector<Issue>& issues() { return m_issues; }

    // SCParserListener overrides
    void enterRoot(sprklr::SCParser::RootContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterRoot(ctx); });
    }
    void exitRoot(sprklr::SCParser::RootContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitRoot(ctx); });
    }

    void enterTopLevelStatement(sprklr::SCParser::TopLevelStatementContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [ctx](auto& detector) { detector->enterTopLevelStatement(ctx); });
    }
    void exitTopLevelStatement(sprklr::SCParser::TopLevelStatementContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [ctx](auto& detector) { detector->exitTopLevelStatement(ctx); });
    }

    void enterClassDef(sprklr::SCParser::ClassDefContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterClassDef(ctx); });
    }
    void exitClassDef(sprklr::SCParser::ClassDefContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitClassDef(ctx); });
    }

    void enterVarDef(sprklr::SCParser::VarDefContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterVarDef(ctx); });
    }
    void exitVarDef(sprklr::SCParser::VarDefContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitVarDef(ctx); });
    }

    void enterExprBinop(sprklr::SCParser::ExprBinopContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterExprBinop(ctx); });
    }
    void exitExprBinop(sprklr::SCParser::ExprBinopContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitExprBinop(ctx); });
    }

    void enterExprAssignDotName(sprklr::SCParser::ExprAssignDotNameContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [ctx](auto& detector) { detector->enterExprAssignDotName(ctx); });
    }
    void exitExprAssignDotName(sprklr::SCParser::ExprAssignDotNameContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [ctx](auto& detector) { detector->exitExprAssignDotName(ctx); });
    }

    void enterExprAssignGlobal(sprklr::SCParser::ExprAssignGlobalContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [ctx](auto& detector) { detector->enterExprAssignGlobal(ctx); });
    }
    void exitExprAssignGlobal(sprklr::SCParser::ExprAssignGlobalContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [ctx](auto& detector) { detector->exitExprAssignGlobal(ctx); });
    }

    void enterExprAssign(sprklr::SCParser::ExprAssignContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [ctx](auto& detector) { detector->enterExprAssign(ctx); });
    }
    void exitExprAssign(sprklr::SCParser::ExprAssignContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitExprAssign(ctx); });
    }

    void enterBlock(sprklr::SCParser::BlockContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterBlock(ctx); });
    }
    void exitBlock(sprklr::SCParser::BlockContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitBlock(ctx); });
    }

    void enterBlockArgList(sprklr::SCParser::BlockArgListContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [ctx](auto& detector) { detector->enterBlockArgList(ctx); });
    }
    void exitBlockArgList(sprklr::SCParser::BlockArgListContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [ctx](auto& detector) { detector->exitBlockArgList(ctx); });
    }

    void enterBody(sprklr::SCParser::BodyContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterBody(ctx); });
    }
    void exitBody(sprklr::SCParser::BodyContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitBody(ctx); });
    }

    void enterKeyArgList(sprklr::SCParser::KeyArgListContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [ctx](auto& detector) { detector->enterKeyArgList(ctx); });
    }
    void exitKeyArgList(sprklr::SCParser::KeyArgListContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitKeyArgList(ctx); });
    }

    void visitTerminal(antlr4::tree::TerminalNode* node) override {
        std::for_each(m_detectors.begin(), m_detectors.end(),
                      [node](auto& detector) { detector->visitTerminal(node); });
    }

private:
    std::vector<std::unique_ptr<sprklr::SCParserListener>> m_detectors;
    std::vector<Issue> m_issues;
};

} // namespace sclint

#endif // SCLINT_SRC_DETECTORS_DETECTOR_HPP_