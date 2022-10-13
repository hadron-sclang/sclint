#include "detectors/Detector.hpp"

namespace lint {

void DetectorMux::clearDetectors() { m_detectors.clear(); }

void DetectorMux::addDetector(std::unique_ptr<Detector> detector) { m_detectors.push_back(std::move(detector)); }

void DetectorMux::enterRoot(sprklr::SCParser::RootContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterRoot(ctx); });
}

void DetectorMux::exitRoot(sprklr::SCParser::RootContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitRoot(ctx); });
}

void DetectorMux::enterTopLevelStatement(sprklr::SCParser::TopLevelStatementContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(),
                  [ctx](auto& detector) { detector->enterTopLevelStatement(ctx); });
}

void DetectorMux::exitTopLevelStatement(sprklr::SCParser::TopLevelStatementContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(),
                  [ctx](auto& detector) { detector->exitTopLevelStatement(ctx); });
}

void DetectorMux::enterClassDef(sprklr::SCParser::ClassDefContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterClassDef(ctx); });
}

void DetectorMux::exitClassDef(sprklr::SCParser::ClassDefContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitClassDef(ctx); });
}

void DetectorMux::enterVarDef(sprklr::SCParser::VarDefContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterVarDef(ctx); });
}

void DetectorMux::exitVarDef(sprklr::SCParser::VarDefContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitVarDef(ctx); });
}

void DetectorMux::enterExprBinop(sprklr::SCParser::ExprBinopContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterExprBinop(ctx); });
}

void DetectorMux::exitExprBinop(sprklr::SCParser::ExprBinopContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitExprBinop(ctx); });
}

void DetectorMux::enterExprAssignDotName(sprklr::SCParser::ExprAssignDotNameContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(),
                  [ctx](auto& detector) { detector->enterExprAssignDotName(ctx); });
}

void DetectorMux::exitExprAssignDotName(sprklr::SCParser::ExprAssignDotNameContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(),
                  [ctx](auto& detector) { detector->exitExprAssignDotName(ctx); });
}

void DetectorMux::enterExprAssignGlobal(sprklr::SCParser::ExprAssignGlobalContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(),
                  [ctx](auto& detector) { detector->enterExprAssignGlobal(ctx); });
}

void DetectorMux::exitExprAssignGlobal(sprklr::SCParser::ExprAssignGlobalContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(),
                  [ctx](auto& detector) { detector->exitExprAssignGlobal(ctx); });
}

void DetectorMux::enterExprAssign(sprklr::SCParser::ExprAssignContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterExprAssign(ctx); });
}

void DetectorMux::exitExprAssign(sprklr::SCParser::ExprAssignContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitExprAssign(ctx); });
}

void DetectorMux::enterBlock(sprklr::SCParser::BlockContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterBlock(ctx); });
}

void DetectorMux::exitBlock(sprklr::SCParser::BlockContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitBlock(ctx); });
}

void DetectorMux::enterBlockArgList(sprklr::SCParser::BlockArgListContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterBlockArgList(ctx); });
}

void DetectorMux::exitBlockArgList(sprklr::SCParser::BlockArgListContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitBlockArgList(ctx); });
}

void DetectorMux::enterBody(sprklr::SCParser::BodyContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterBody(ctx); });
}

void DetectorMux::exitBody(sprklr::SCParser::BodyContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitBody(ctx); });
}

void DetectorMux::enterKeyArgList(sprklr::SCParser::KeyArgListContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->enterKeyArgList(ctx); });
}

void DetectorMux::exitKeyArgList(sprklr::SCParser::KeyArgListContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) { detector->exitKeyArgList(ctx); });
}

void DetectorMux::visitTerminal(antlr4::tree::TerminalNode* node) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [node](auto& detector) { detector->visitTerminal(node); });
}

} // namespace lint