#ifndef SCLINT_SRC_METHOD_RETURN_WITH_LEXICAL_SCOPE_LISTENER_HPP_
#define SCLINT_SRC_METHOD_RETURN_WITH_LEXICAL_SCOPE_LISTENER_HPP_

#include "SCParserBaseListener.h"

#include <iostream>
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

class MethodReturnWithLexicalScopeListener : public sprklr::SCParserBaseListener {
public:
    MethodReturnWithLexicalScopeListener() = delete;
    explicit MethodReturnWithLexicalScopeListener(std::string fileName):
        sprklr::SCParserBaseListener(),
        m_fileName(fileName),
        m_inBlockCount(0),
        m_inNamedAssignCount(0),
        m_inArgsCount(0) {}
    virtual ~MethodReturnWithLexicalScopeListener() = default;

    void enterBlock(sprklr::SCParser::BlockContext*) override { ++m_inBlockCount; }
    void exitBlock(sprklr::SCParser::BlockContext*) override { --m_inBlockCount; }

    void enterBody(sprklr::SCParser::BodyContext* ctx) override {
        if (m_inArgsCount > 0 || m_inBlockCount == 0 || m_inNamedAssignCount == 0)
            return;

        if (ctx->returnExpr()) {
            std::cerr << m_fileName << ": line "
                      << ctx->returnExpr()->start->getLine() << " character "
                      << ctx->returnExpr()->start->getCharPositionInLine() << std::endl;
        }
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
    std::string m_fileName;
    int m_inBlockCount;
    int m_inNamedAssignCount;
    int m_inArgsCount;
};

} // namespace sclint

#endif // SCLINT_SRC_METHOD_RETURN_WITH_LEXICAL_SCOPE_LISTENER_HPP_
