#ifndef SRC_LINT_DETECTORS_DETECTOR_LIST_HPP_
#define SRC_LINT_DETECTORS_DETECTOR_LIST_HPP_

#include "detectors/ExampleDetector.hpp"

namespace lint {

template <typename ...Ts>
struct TypeList { };

using DetectorList = TypeList<
    ExampleDetector
>;

} // namespace lint

#endif // SRC_LINT_DETECTORS_DETECTOR_LIST_HPP_
