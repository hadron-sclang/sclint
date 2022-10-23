#ifndef SRC_LINT_LISTENER_MUX_HPP_
#define SRC_LINT_LISTENER_MUX_HPP_

#include "tree/ParseTreeListener.h"

namespace lint {

class Detector;

// A multiplexer for Detectors. For each listener function it calls the same function on every Detector added to it.
// This allows us to use multiple detectors but only walk the tree once. Follows the pattern established in
// https://github.com/antlr/antlr4/issues/841.
class ListenerMux : public antlr4::tree::ParseTreeListener {
public:
    ListenerMux() = default;
    ~ListenerMux() = default;

    void clearDetectors();
    void addDetector(std::unique_ptr<Detector> detector);

    // ParseTreeListener overrides.
    void visitTerminal(antlr4::tree::TerminalNode* node) override;
    void visitErrorNode(antlr4::tree::ErrorNode* node) override;
    void enterEveryRule(antlr4::ParserRuleContext* ctx) override;
    void exitEveryRule(antlr4::ParserRuleContext* ctx) override;

private:
    std::vector<Detector*> m_detectors;
};

} // namespace lint

#endif // SRC_LINT_LISTENER_MUX_HPP_