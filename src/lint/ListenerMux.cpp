#include "ListenerMux.hpp"

#include "detectors/Detector.hpp"

namespace lint {

void ListenerMux::clearDetectors() { m_detectors.clear(); }

void ListenerMux::addDetector(std::unique_ptr<Detector> detector) { m_detectors.push_back(std::move(detector)); }

void ListenerMux::visitTerminal(antlr4::tree::TerminalNode* node) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [node](auto& detector) {
        detector->visitTerminal(node);
    });
}

void ListenerMux::visitErrorNode(antlr4::tree::ErrorNode* node) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [node](auto& detector) {
        detector->visitErrorNode(node);
    });
}

void ListenerMux::enterEveryRule(antlr4::ParserRuleContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
        detector->enterEveryRule(ctx);
        ctx->enterRule(detector.get());
    });
}

void ListenerMux::exitEveryRule(antlr4::ParserRuleContext* ctx) {
    std::for_each(m_detectors.begin(), m_detectors.end(), [ctx](auto& detector) {
        detector->exitEveryRule(ctx);
        ctx->exitRule(detector.get());
    });
}

} // namespace lint