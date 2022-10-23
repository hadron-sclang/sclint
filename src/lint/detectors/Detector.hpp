#ifndef SRC_LINT_DETECTORS_DETECTOR_HPP_
#define SRC_LINT_DETECTORS_DETECTOR_HPP_

#include "Issue.hpp"

#include "SCParserListener.h"
#include "SCParserBaseListener.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace antlr4 {
class TokenStreamRewriter;
}

namespace lint {

struct Config;
class Linter;

class Detector : public sprklr::SCParserBaseListener {
public:
    Detector() = delete;
    Detector(Linter* linter, antlr4::TokenStreamRewriter* rewriter): m_linter(linter), m_rewriter(rewriter) { }
    virtual ~Detector() = default;

protected:
    Linter* m_linter;
    antlr4::TokenStreamRewriter* m_rewriter;
};

} // namespace lint

#endif // SRC_LINT_DETECTORS_DETECTOR_HPP_
