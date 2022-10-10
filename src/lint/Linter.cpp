#include "Linter.hpp"

#include "Config.hpp"
#include "detectors/MethodReturnLexicalScope.hpp"
#include "detectors/Whitespace.hpp"

#include "SCLexer.h"
#include "SCParser.h"

namespace lint {

Linter::Linter(const Config* config, std::string_view code): m_config(config), m_code(code), m_mux(config) { }

bool Linter::lint() {
    antlr4::ANTLRInputStream input(m_code.data(), m_code.size());
    sprklr::SCLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    sprklr::SCParser parser(&tokens);
    if (parser.getNumberOfSyntaxErrors())
        return false;

    m_mux.clearDetectors();
    m_mux.addDetector(std::make_unique<Whitespace>(&tokens));

    if (m_config->noMethodReturnWithLexicalScope)
        m_mux.addDetector(std::make_unique<MethodReturnLexicalScope>());

    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&m_mux, parser.root());

    return true;
}

} // namespace lint
