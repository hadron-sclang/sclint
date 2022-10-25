#ifndef SRC_LINT_DETECTORS_DETECTOR_LIST_HPP_
#define SRC_LINT_DETECTORS_DETECTOR_LIST_HPP_

#include "detectors/ExampleDetector.hpp"
#include "detectors/LintTest.hpp"
#include "detectors/OneNewlineAtEndOfFile.hpp"

namespace lint {

template <typename T, typename... Ts> struct TypeList;


using DetectorList = TypeList<ExampleDetector, LintTest, OneNewlineAtEndOfFile>;

} // namespace lint

#endif // SRC_LINT_DETECTORS_DETECTOR_LIST_HPP_
