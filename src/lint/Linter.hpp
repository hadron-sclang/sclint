#ifndef SRC_LINT_LINTER_HPP_
#define SRC_LINT_LINTER_HPP_

#include "detectors/Detector.hpp"
#include "Issue.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace lint {

struct Config;
class DetectorMux;

class Linter {
public:
    Linter() = delete;
    Linter(const Config* config, std::string_view code);
    ~Linter() = default;

    // Returns true on successful lint, false on failure (usually parsing errors)
    bool lint();

    const std::vector<Issue>& issues() const { return m_mux.issues(); }

private:
    const Config* m_config;
    std::string_view m_code;
    DetectorMux m_mux;
};

} // namespace lint

#endif // SRC_LINT_LINTER_HPP_