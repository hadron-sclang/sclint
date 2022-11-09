#ifndef SRC_LINT_DETECTORS_DETECTOR_LIST_HPP_
#define SRC_LINT_DETECTORS_DETECTOR_LIST_HPP_

// TODO: add your detector header here in alphabetical order.
#include "detectors/AvoidCurryArgument.hpp"
#include "detectors/AvoidMethodReturnInNamedFunctions.hpp"
#include "detectors/ExampleDetector.hpp"
#include "detectors/LintTest.hpp"
#include "detectors/OmitTrailingWhitespace.hpp"
#include "detectors/OneNewlineAtEndOfFile.hpp"

namespace lint {

template <typename T, typename... Ts> struct TypeList;

// TODO: add your detector class name to this list in alphabetical order.
using DetectorList = TypeList<AvoidCurryArgument, AvoidMethodReturnInNamedFunctions, ExampleDetector, LintTest,
                              OmitTrailingWhitespace, OneNewlineAtEndOfFile>;

} // namespace lint

#endif // SRC_LINT_DETECTORS_DETECTOR_LIST_HPP_
