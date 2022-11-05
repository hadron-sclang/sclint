#ifndef EMSCRIPTEN
#error "This file is for Emscripten builds only"
#endif

#include "Config.hpp"
#include "Issue.hpp"
#include "Linter.hpp"

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(Module) {
    emscripten::class_<lint::Config>("Config")
        .constructor()
        .function("initDefaults", &lint::Config::initDefaults)
        .function("readJSON", &lint::Config::readJSON)
        .function("writeJSON", &lint::Config::writeJSON)
        .function("getOptionNamed", &lint::Config::getOptionNamed)
        .function("setOptionNamed", &lint::Config::setOptionNamed);

    emscripten::enum_<lint::IssueSeverity>("IssueSeverity")
        .value("kFatal", lint::IssueSeverity::kFatal)
        .value("kError", lint::IssueSeverity::kError)
        .value("kWarning", lint::IssueSeverity::kWarning)
        .value("kLint", lint::IssueSeverity::kLint)
        .value("kNote", lint::IssueSeverity::kNote)
        .value("kNone", lint::IssueSeverity::kNone);

    emscripten::value_object<lint::Issue>("Issue")
        .field("issueSeverity", &lint::Issue::issueSeverity)
        .field("columnNumber", &lint::Issue::columnNumber)
        .field("detectorName", &lint::Issue::detectorName)
        .field("message", &lint::Issue::message);

    emscripten::register_vector<lint::Issue>("vector<Issue>");

    emscripten::class_<lint::Linter>("Linter")
        .constructor()
        .function("lint", &lint::Linter::lint)
        .function("issues", &lint::Linter::issues)
        .function("rewrittenString", &lint::Linter::rewrittenString);
}
