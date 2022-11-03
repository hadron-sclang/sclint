#ifndef SRC_LINT_CONFIG_HPP_
#define SRC_LINT_CONFIG_HPP_

#ifdef EMSCRIPTEN
#    include <emscripten/bind.h>
#endif

#include <string_view>
#include <string>
#include <unordered_map>

namespace lint {

// Holds the current configuration of the linter.
class Config {
public:
    // Build a default configuration (SCStyle)
    Config() { initDefaults(); }
    ~Config() = default;

    // Set all options to their default values. Called by the constructor.
    void initDefaults();

    // Returns an empty string on success or an error message if the JSON didn't parse correctly.
    std::string readJSON(std::string_view jsonString);

    // Returns a JSON string of the options.
    std::string writeJSON() const;

    bool getOptionNamed(const std::string& name) const;
    void setOptionNamed(const std::string& name, bool value);

private:
    std::unordered_map<std::string, bool> m_options;
};

#ifdef EMSCRIPTEN
EMSCRIPTEN_BINDINGS(sclint_config_binding) {
    emscripten::class_<Config>("Config")
        .constructor()
        .function("initDefaults", &Config::initDefaults)
        .function("readJSON", &Config::readJSON)
        .function("writeJSON", &Config::writeJSON)
        .function("getOptionNamed", &Config::getOptionNamed)
        .function("setOptionNamed", &Config::setOptionNamed);
}
#endif

} // namespace lint

#endif // SRC_LINT_CONFIG_HPP_
