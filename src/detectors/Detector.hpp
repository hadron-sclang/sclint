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

    Issue(size_t line, size_t col, std::string msg): lineNumber(line), columnNumber(col), message(msg) {}
};

class Detector : public sprklr::SCParserBaseListener {
public:
    Detector() = default;
    virtual ~Detector() = default;

    void setIssues(std::vector<Issue>* issues) { m_issues = issues; }

protected:
    std::vector<Issue>* m_issues = nullptr;
};

class DetectorMux : public sprklr::SCParserListener {
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
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterRoot(ctx);
        });
    }
    void exitRoot(sprklr::SCParser::RootContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitRoot(ctx);
        });
    }

    void enterTopLevelStatement(sprklr::SCParser::TopLevelStatementContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterTopLevelStatement(ctx);
        });
    }
    void exitTopLevelStatement(sprklr::SCParser::TopLevelStatementContext* ctx) override {
       std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitTopLevelStatement(ctx);
        });
    }

    void enterClassDef(sprklr::SCParser::ClassDefContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterClassDef(ctx);
        });
    }
    void exitClassDef(sprklr::SCParser::ClassDefContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitClassDef(ctx);
        });
    }

    void enterSuperclass(sprklr::SCParser::SuperclassContext*) override { }
    void exitSuperclass(sprklr::SCParser::SuperclassContext*) override { }

    void enterClassVarDecl(sprklr::SCParser::ClassVarDeclContext*) override { }
    void exitClassVarDecl(sprklr::SCParser::ClassVarDeclContext*) override { }

    void enterRwSlotDefList(sprklr::SCParser::RwSlotDefListContext*) override { }
    void exitRwSlotDefList(sprklr::SCParser::RwSlotDefListContext*) override { }

    void enterRwSlotDef(sprklr::SCParser::RwSlotDefContext*) override { }
    void exitRwSlotDef(sprklr::SCParser::RwSlotDefContext*) override { }

    void enterRwSpec(sprklr::SCParser::RwSpecContext*) override { }
    void exitRwSpec(sprklr::SCParser::RwSpecContext*) override { }

    void enterName(sprklr::SCParser::NameContext*) override { }
    void exitName(sprklr::SCParser::NameContext*) override { }

    void enterLiteral(sprklr::SCParser::LiteralContext*) override { }
    void exitLiteral(sprklr::SCParser::LiteralContext*) override { }

    void enterCoreLiteral(sprklr::SCParser::CoreLiteralContext*) override { }
    void exitCoreLiteral(sprklr::SCParser::CoreLiteralContext*) override { }

    void enterInteger(sprklr::SCParser::IntegerContext*) override { }
    void exitInteger(sprklr::SCParser::IntegerContext*) override { }

    void enterIntegerNumber(sprklr::SCParser::IntegerNumberContext*) override { }
    void exitIntegerNumber(sprklr::SCParser::IntegerNumberContext*) override { }

    void enterFloatingPoint(sprklr::SCParser::FloatingPointContext*) override { }
    void exitFloatingPoint(sprklr::SCParser::FloatingPointContext*) override { }

    void enterFloatLiteral(sprklr::SCParser::FloatLiteralContext*) override { }
    void exitFloatLiteral(sprklr::SCParser::FloatLiteralContext*) override { }

    void enterFloatNumber(sprklr::SCParser::FloatNumberContext*) override { }
    void exitFloatNumber(sprklr::SCParser::FloatNumberContext*) override { }

    void enterAccidental(sprklr::SCParser::AccidentalContext*) override { }
    void exitAccidental(sprklr::SCParser::AccidentalContext*) override { }

    void enterStrings(sprklr::SCParser::StringsContext*) override { }
    void exitStrings(sprklr::SCParser::StringsContext*) override { }

    void enterSymbol(sprklr::SCParser::SymbolContext*) override { }
    void exitSymbol(sprklr::SCParser::SymbolContext*) override { }

    void enterBooleanConstant(sprklr::SCParser::BooleanConstantContext*) override { }
    void exitBooleanConstant(sprklr::SCParser::BooleanConstantContext*) override { }

    void enterList(sprklr::SCParser::ListContext*) override { }
    void exitList(sprklr::SCParser::ListContext*) override { }

    void enterInnerListLiteral(sprklr::SCParser::InnerListLiteralContext*) override { }
    void exitInnerListLiteral(sprklr::SCParser::InnerListLiteralContext*) override { }

    void enterListLiteral(sprklr::SCParser::ListLiteralContext*) override { }
    void exitListLiteral(sprklr::SCParser::ListLiteralContext*) override { }

    void enterListLiterals(sprklr::SCParser::ListLiteralsContext*) override { }
    void exitListLiterals(sprklr::SCParser::ListLiteralsContext*) override { }

    void enterInnerDictLiteral(sprklr::SCParser::InnerDictLiteralContext*) override { }
    void exitInnerDictLiteral(sprklr::SCParser::InnerDictLiteralContext*) override { }

    void enterDictLiterals(sprklr::SCParser::DictLiteralsContext*) override { }
    void exitDictLiterals(sprklr::SCParser::DictLiteralsContext*) override { }

    void enterDictLiteral(sprklr::SCParser::DictLiteralContext*) override { }
    void exitDictLiteral(sprklr::SCParser::DictLiteralContext*) override { }

    void enterConstDefList(sprklr::SCParser::ConstDefListContext*) override { }
    void exitConstDefList(sprklr::SCParser::ConstDefListContext*) override { }

    void enterRSlotDef(sprklr::SCParser::RSlotDefContext*) override { }
    void exitRSlotDef(sprklr::SCParser::RSlotDefContext*) override { }

    void enterMethodDef(sprklr::SCParser::MethodDefContext*) override { }
    void exitMethodDef(sprklr::SCParser::MethodDefContext*) override { }

    void enterMethodDefName(sprklr::SCParser::MethodDefNameContext*) override { }
    void exitMethodDefName(sprklr::SCParser::MethodDefNameContext*) override { }

    void enterArgDecls(sprklr::SCParser::ArgDeclsContext*) override { }
    void exitArgDecls(sprklr::SCParser::ArgDeclsContext*) override { }

    void enterVarDefList(sprklr::SCParser::VarDefListContext*) override { }
    void exitVarDefList(sprklr::SCParser::VarDefListContext*) override { }

    void enterVarDef(sprklr::SCParser::VarDefContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterVarDef(ctx);
        });
    }
    void exitVarDef(sprklr::SCParser::VarDefContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitVarDef(ctx);
        });
    }

    void enterExprMsgDotNameArgs(sprklr::SCParser::ExprMsgDotNameArgsContext*) override { }
    void exitExprMsgDotNameArgs(sprklr::SCParser::ExprMsgDotNameArgsContext*) override { }

    void enterExprBinop(sprklr::SCParser::ExprBinopContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterExprBinop(ctx);
        });
    }
    void exitExprBinop(sprklr::SCParser::ExprBinopContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitExprBinop(ctx);
        });
    }

    void enterExprMsgNamePerform(sprklr::SCParser::ExprMsgNamePerformContext*) override { }
    void exitExprMsgNamePerform(sprklr::SCParser::ExprMsgNamePerformContext*) override { }

    void enterExprIndexSeriesAssign(sprklr::SCParser::ExprIndexSeriesAssignContext*) override { }
    void exitExprIndexSeriesAssign(sprklr::SCParser::ExprIndexSeriesAssignContext*) override { }

    void enterExprAssignDotName(sprklr::SCParser::ExprAssignDotNameContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterExprAssignDotName(ctx);
        });
    }
    void exitExprAssignDotName(sprklr::SCParser::ExprAssignDotNameContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitExprAssignDotName(ctx);
        });
    }

    void enterExprMsgClassBlock(sprklr::SCParser::ExprMsgClassBlockContext*) override { }
    void exitExprMsgClassBlock(sprklr::SCParser::ExprMsgClassBlockContext*) override { }

    void enterExprArray(sprklr::SCParser::ExprArrayContext*) override { }
    void exitExprArray(sprklr::SCParser::ExprArrayContext*) override { }

    void enterExprMsgKeyBlock(sprklr::SCParser::ExprMsgKeyBlockContext*) override { }
    void exitExprMsgKeyBlock(sprklr::SCParser::ExprMsgKeyBlockContext*) override { }

    void enterExprMsgClass(sprklr::SCParser::ExprMsgClassContext*) override { }
    void exitExprMsgClass(sprklr::SCParser::ExprMsgClassContext*) override { }

    void enterExprMsgNameBlock(sprklr::SCParser::ExprMsgNameBlockContext*) override { }
    void exitExprMsgNameBlock(sprklr::SCParser::ExprMsgNameBlockContext*) override { }

    void enterExprMsgKeyArgs(sprklr::SCParser::ExprMsgKeyArgsContext*) override { }
    void exitExprMsgKeyArgs(sprklr::SCParser::ExprMsgKeyArgsContext*) override { }

    void enterExprMsgKeyPerform(sprklr::SCParser::ExprMsgKeyPerformContext*) override { }
    void exitExprMsgKeyPerform(sprklr::SCParser::ExprMsgKeyPerformContext*) override { }

    void enterExprCurryArg(sprklr::SCParser::ExprCurryArgContext*) override { }
    void exitExprCurryArg(sprklr::SCParser::ExprCurryArgContext*) override { }

    void enterExprMsgDotNameKeyArgs(sprklr::SCParser::ExprMsgDotNameKeyArgsContext*) override { }
    void exitExprMsgDotNameKeyArgs(sprklr::SCParser::ExprMsgDotNameKeyArgsContext*) override { }

    void enterExprMultiAssign(sprklr::SCParser::ExprMultiAssignContext*) override { }
    void exitExprMultiAssign(sprklr::SCParser::ExprMultiAssignContext*) override { }

    void enterExprAssignGlobal(sprklr::SCParser::ExprAssignGlobalContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterExprAssignGlobal(ctx);
        });
    }
    void exitExprAssignGlobal(sprklr::SCParser::ExprAssignGlobalContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitExprAssignGlobal(ctx);
        });
    }

    void enterExprArrayRead(sprklr::SCParser::ExprArrayReadContext*) override { }
    void exitExprArrayRead(sprklr::SCParser::ExprArrayReadContext*) override { }

    void enterExprMsgDotName(sprklr::SCParser::ExprMsgDotNameContext*) override { }
    void exitExprMsgDotName(sprklr::SCParser::ExprMsgDotNameContext*) override { }

    void enterExprMsgClassKeyArgs(sprklr::SCParser::ExprMsgClassKeyArgsContext*) override { }
    void exitExprMsgClassKeyArgs(sprklr::SCParser::ExprMsgClassKeyArgsContext*) override { }

    void enterExprArrayWrite(sprklr::SCParser::ExprArrayWriteContext*) override { }
    void exitExprArrayWrite(sprklr::SCParser::ExprArrayWriteContext*) override { }

    void enterExprIndexSeries(sprklr::SCParser::ExprIndexSeriesContext*) override { }
    void exitExprIndexSeries(sprklr::SCParser::ExprIndexSeriesContext*) override { }

    void enterExprName(sprklr::SCParser::ExprNameContext*) override { }
    void exitExprName(sprklr::SCParser::ExprNameContext*) override { }

    void enterExprNumericSeries(sprklr::SCParser::ExprNumericSeriesContext*) override { }
    void exitExprNumericSeries(sprklr::SCParser::ExprNumericSeriesContext*) override { }

    void enterExprAssignTBD(sprklr::SCParser::ExprAssignTBDContext*) override { }
    void exitExprAssignTBD(sprklr::SCParser::ExprAssignTBDContext*) override { }

    void enterExprMsgNewCollection(sprklr::SCParser::ExprMsgNewCollectionContext*) override { }
    void exitExprMsgNewCollection(sprklr::SCParser::ExprMsgNewCollectionContext*) override { }

    void enterExprClassname(sprklr::SCParser::ExprClassnameContext*) override { }
    void exitExprClassname(sprklr::SCParser::ExprClassnameContext*) override { }

    void enterExprMsgDotNamePerform(sprklr::SCParser::ExprMsgDotNamePerformContext*) override { }
    void exitExprMsgDotNamePerform(sprklr::SCParser::ExprMsgDotNamePerformContext*) override { }

    void enterExprBlock(sprklr::SCParser::ExprBlockContext*) override { }
    void exitExprBlock(sprklr::SCParser::ExprBlockContext*) override { }

    void enterExprMsgDot(sprklr::SCParser::ExprMsgDotContext*) override { }
    void exitExprMsgDot(sprklr::SCParser::ExprMsgDotContext*) override { }

    void enterExprExprSeq(sprklr::SCParser::ExprExprSeqContext*) override { }
    void exitExprExprSeq(sprklr::SCParser::ExprExprSeqContext*) override { }

    void enterExprGlobal(sprklr::SCParser::ExprGlobalContext*) override { }
    void exitExprGlobal(sprklr::SCParser::ExprGlobalContext*) override { }

    void enterExprGrave(sprklr::SCParser::ExprGraveContext*) override { }
    void exitExprGrave(sprklr::SCParser::ExprGraveContext*) override { }

    void enterExprMsgClassArgs(sprklr::SCParser::ExprMsgClassArgsContext*) override { }
    void exitExprMsgClassArgs(sprklr::SCParser::ExprMsgClassArgsContext*) override { }

    void enterExprIndexSeriesSuffix(sprklr::SCParser::ExprIndexSeriesSuffixContext*) override { }
    void exitExprIndexSeriesSuffix(sprklr::SCParser::ExprIndexSeriesSuffixContext*) override { }

    void enterExprMsgClassPerform(sprklr::SCParser::ExprMsgClassPerformContext*) override { }
    void exitExprMsgClassPerform(sprklr::SCParser::ExprMsgClassPerformContext*) override { }

    void enterExprNumericSeriesSuffix(sprklr::SCParser::ExprNumericSeriesSuffixContext*) override { }
    void exitExprNumericSeriesSuffix(sprklr::SCParser::ExprNumericSeriesSuffixContext*) override { }

    void enterExprMsgDotArgs(sprklr::SCParser::ExprMsgDotArgsContext*) override { }
    void exitExprMsgDotArgs(sprklr::SCParser::ExprMsgDotArgsContext*) override { }

    void enterExprLiteral(sprklr::SCParser::ExprLiteralContext*) override { }
    void exitExprLiteral(sprklr::SCParser::ExprLiteralContext*) override { }

    void enterExprListComp(sprklr::SCParser::ExprListCompContext*) override { }
    void exitExprListComp(sprklr::SCParser::ExprListCompContext*) override { }

    void enterExprNewEvent(sprklr::SCParser::ExprNewEventContext*) override { }
    void exitExprNewEvent(sprklr::SCParser::ExprNewEventContext*) override { }

    void enterExprAssign(sprklr::SCParser::ExprAssignContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterExprAssign(ctx);
        });
    }
    void exitExprAssign(sprklr::SCParser::ExprAssignContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitExprAssign(ctx);
        });
    }

    void enterExprIndexSeriesAssignSuffix(sprklr::SCParser::ExprIndexSeriesAssignSuffixContext*) override { }
    void exitExprIndexSeriesAssignSuffix(sprklr::SCParser::ExprIndexSeriesAssignSuffixContext*) override { }

    void enterExprMsgDotPerform(sprklr::SCParser::ExprMsgDotPerformContext*) override { }
    void exitExprMsgDotPerform(sprklr::SCParser::ExprMsgDotPerformContext*) override { }

    void enterBlock(sprklr::SCParser::BlockContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterBlock(ctx);
        });
    }
    void exitBlock(sprklr::SCParser::BlockContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitBlock(ctx);
        });
    }

    void enterBlockArgList(sprklr::SCParser::BlockArgListContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterBlockArgList(ctx);
        });
    }
    void exitBlockArgList(sprklr::SCParser::BlockArgListContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitBlockArgList(ctx);
        });
    }

    void enterBody(sprklr::SCParser::BodyContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterBody(ctx);
        });
    }
    void exitBody(sprklr::SCParser::BodyContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitBody(ctx);
        });
    }

    void enterReturnExpr(sprklr::SCParser::ReturnExprContext*) override { }
    void exitReturnExpr(sprklr::SCParser::ReturnExprContext*) override { }

    void enterExprSeq(sprklr::SCParser::ExprSeqContext*) override { }
    void exitExprSeq(sprklr::SCParser::ExprSeqContext*) override { }

    void enterVarDecls(sprklr::SCParser::VarDeclsContext*) override { }
    void exitVarDecls(sprklr::SCParser::VarDeclsContext*) override { }

    void enterListComp(sprklr::SCParser::ListCompContext*) override { }
    void exitListComp(sprklr::SCParser::ListCompContext*) override { }

    void enterQualifiers(sprklr::SCParser::QualifiersContext*) override { }
    void exitQualifiers(sprklr::SCParser::QualifiersContext*) override { }

    void enterQualifier(sprklr::SCParser::QualifierContext*) override { }
    void exitQualifier(sprklr::SCParser::QualifierContext*) override { }

    void enterBinopKey(sprklr::SCParser::BinopKeyContext*) override { }
    void exitBinopKey(sprklr::SCParser::BinopKeyContext*) override { }

    void enterBinop(sprklr::SCParser::BinopContext*) override { }
    void exitBinop(sprklr::SCParser::BinopContext*) override { }

    void enterArgList(sprklr::SCParser::ArgListContext*) override { }
    void exitArgList(sprklr::SCParser::ArgListContext*) override { }

    void enterKeyArgList(sprklr::SCParser::KeyArgListContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->enterKeyArgList(ctx);
        });
    }
    void exitKeyArgList(sprklr::SCParser::KeyArgListContext* ctx) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
            detector->exitKeyArgList(ctx);
        });
    }

    void enterKeyArg(sprklr::SCParser::KeyArgContext*) override { }
    void exitKeyArg(sprklr::SCParser::KeyArgContext*) override { }

    void enterPerformArgList(sprklr::SCParser::PerformArgListContext*) override { }
    void exitPerformArgList(sprklr::SCParser::PerformArgListContext*) override { }

    void enterArrayElems(sprklr::SCParser::ArrayElemsContext*) override { }
    void exitArrayElems(sprklr::SCParser::ArrayElemsContext*) override { }

    void enterArrayElem(sprklr::SCParser::ArrayElemContext*) override { }
    void exitArrayElem(sprklr::SCParser::ArrayElemContext*) override { }

    void enterNumericSeries(sprklr::SCParser::NumericSeriesContext*) override { }
    void exitNumericSeries(sprklr::SCParser::NumericSeriesContext*) override { }

    void enterDictElements(sprklr::SCParser::DictElementsContext*) override { }
    void exitDictElements(sprklr::SCParser::DictElementsContext*) override { }

    void enterDictElement(sprklr::SCParser::DictElementContext*) override { }
    void exitDictElement(sprklr::SCParser::DictElementContext*) override { }

    void enterAdverb(sprklr::SCParser::AdverbContext*) override { }
    void exitAdverb(sprklr::SCParser::AdverbContext*) override { }

    void enterMultiAssignVars(sprklr::SCParser::MultiAssignVarsContext*) override { }
    void exitMultiAssignVars(sprklr::SCParser::MultiAssignVarsContext*) override { }

    void enterPipeDefList(sprklr::SCParser::PipeDefListContext*) override { }
    void exitPipeDefList(sprklr::SCParser::PipeDefListContext*) override { }

    void enterPipeDef(sprklr::SCParser::PipeDefContext*) override { }
    void exitPipeDef(sprklr::SCParser::PipeDefContext*) override { }

    void enterPrimitive(sprklr::SCParser::PrimitiveContext*) override { }
    void exitPrimitive(sprklr::SCParser::PrimitiveContext*) override { }

    void enterClassExtension(sprklr::SCParser::ClassExtensionContext*) override { }
    void exitClassExtension(sprklr::SCParser::ClassExtensionContext*) override { }

    void enterInterpreterCode(sprklr::SCParser::InterpreterCodeContext*) override { }
    void exitInterpreterCode(sprklr::SCParser::InterpreterCodeContext*) override { }

    void enterEveryRule(antlr4::ParserRuleContext*) override { }
    void exitEveryRule(antlr4::ParserRuleContext*) override { }
    void visitTerminal(antlr4::tree::TerminalNode* node) override {
        std::for_each(m_detectors.begin(), m_detectors.end(), [node](auto& detector) {
            detector->visitTerminal(node);
        });
    }
    void visitErrorNode(antlr4::tree::ErrorNode* /*node*/) override { }

private:
    std::vector<std::unique_ptr<sprklr::SCParserListener>> m_detectors;
    std::vector<Issue> m_issues;
};

} // namespace sclint

#endif // SCLINT_SRC_DETECTORS_DETECTOR_HPP_