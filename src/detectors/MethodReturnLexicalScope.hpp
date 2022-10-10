#ifndef SCLINT_SRC_DETECTORS_METHOD_RETURN_LEXICAL_SCOPE_HPP_
#define SCLINT_SRC_DETECTORS_METHOD_RETURN_LEXICAL_SCOPE_HPP_

#include "Detector.hpp"

#include "SCParserBaseListener.h"

#include <string>

namespace sclint {

/*
Test Code:

True Positive:
Function {
    prTry {
        var result, thread = thisThread;
        var next = thread.exceptionHandler,
        wasInProtectedFunc = Exception.inProtectedFunction;
        thread.exceptionHandler = {|error|
            thread.exceptionHandler = next; // pop
            ^error  // HERE
        };
        Exception.inProtectedFunction = true;
        result = this.value;
        Exception.inProtectedFunction = wasInProtectedFunc;
        thread.exceptionHandler = next; // pop
        ^result
    }
}

False Positive:
Quark {
    dependencies {
        var deps = this.data['dependencies'] ?? {^[]};  // HERE
        if(deps.isSequenceableCollection.not, {
            ("Invalid dependencies " + this + deps).warn;
            ^[]
        });
        ^deps.collect({ |dep|
            var q = Quark.parseDependency(dep, this);
            if(q.isNil, {
                "% not found".format(dep).warn;
            });
            q
        }).select({ |it| it.notNil });
    }
}
*/

class MethodReturnLexicalScope : public Detector {
public:
    MethodReturnLexicalScope(): Detector(), m_inBlockCount(0), m_inNamedAssignCount(0), m_inArgsCount(0) { }
    virtual ~MethodReturnLexicalScope() = default;

    void enterBlock(sprklr::SCParser::BlockContext*) override { ++m_inBlockCount; }
    void exitBlock(sprklr::SCParser::BlockContext*) override { --m_inBlockCount; }

    void enterBody(sprklr::SCParser::BodyContext* ctx) override {
        if (!ctx->returnExpr() || m_inArgsCount > 0 || m_inBlockCount == 0 || m_inNamedAssignCount == 0)
            return;

        m_issues->emplace_back(ctx->returnExpr()->start->getLine(), ctx->returnExpr()->start->getCharPositionInLine(),
                               std::string("method return in named function"));
    }

    void enterExprAssignDotName(sprklr::SCParser::ExprAssignDotNameContext*) override { ++m_inNamedAssignCount; }
    void exitExprAssignDotName(sprklr::SCParser::ExprAssignDotNameContext*) override { --m_inNamedAssignCount; }

    void enterExprAssignGlobal(sprklr::SCParser::ExprAssignGlobalContext*) override { ++m_inNamedAssignCount; }
    void exitExprAssignGlobal(sprklr::SCParser::ExprAssignGlobalContext*) override { --m_inNamedAssignCount; }

    void enterExprAssign(sprklr::SCParser::ExprAssignContext*) override { ++m_inNamedAssignCount; }
    void exitExprAssign(sprklr::SCParser::ExprAssignContext*) override { --m_inNamedAssignCount; }

    void enterVarDef(sprklr::SCParser::VarDefContext*) override { ++m_inNamedAssignCount; }
    void exitVarDef(sprklr::SCParser::VarDefContext*) override { --m_inNamedAssignCount; }

    // Block arguments to messages are safe to contain method returns because the method return is guaranteed to
    // respect the caller frame.
    void enterBlockArgList(sprklr::SCParser::BlockArgListContext*) override { ++m_inArgsCount; }
    void exitBlockArgList(sprklr::SCParser::BlockArgListContext*) override { --m_inArgsCount; }

    void enterArgList(sprklr::SCParser::ArgListContext*) override { ++m_inArgsCount; }
    void exitArgList(sprklr::SCParser::ArgListContext*) override { --m_inArgsCount; }

    void enterKeyArgList(sprklr::SCParser::KeyArgListContext*) override { ++m_inArgsCount; }
    void exitKeyArgList(sprklr::SCParser::KeyArgListContext*) override { --m_inArgsCount; }

    void enterExprBinop(sprklr::SCParser::ExprBinopContext*) override { ++m_inArgsCount; }
    void exitExprBinop(sprklr::SCParser::ExprBinopContext*) override { --m_inArgsCount; }

private:
    int m_inBlockCount;
    int m_inNamedAssignCount;
    int m_inArgsCount;
};

} // namespace sclint

#endif // SCLINT_SRC_DETECTORS_METHOD_RETURN_LEXICAL_SCOPE_HPP_
