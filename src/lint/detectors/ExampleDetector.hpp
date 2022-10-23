#ifndef SRC_LIST_DETECTORS_EXAMPLE_DETECTOR_HPP_
#define SRC_LIST_DETECTORS_EXAMPLE_DETECTOR_HPP_

#include "detectors/Detector.hpp"

namespace lint {

// TODO: Do a global find/replace to rename ExampleDetector to the name of your class.
class ExampleDetector : public Detector {
public:
    ExampleDetector() = delete;
    ExampleDetector(Linter* linter, antlr4::TokenStreamRewriter* rewriter): Detector(linter, rewriter) { }
    virtual ~ExampleDetector() = default;

    // TODO: provide a JSON-compatible string option name, should be the name of your class with a lower-case first
    // letter.
    static constexpr const char* kOptionName = "exampleDetector";

    // TODO: provide a default value for the option. Detectors in the sclang style guide
    // at https://github.com/supercollider/supercollider/wiki/Code-style-guidelines are on/true by default, any option
    // outside of the official style guide must be default off/false.
    static constexpr bool kDefaultValue = false;

private:
    // TODO: add any needed member variables here.
};

} // namespace lint

#endif // SRC_LIST_DETECTORS_EXAMPLE_DETECTOR_HPP_