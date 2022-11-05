#ifndef SRC_LINT_CONFIG_HPP_
#define SRC_LINT_CONFIG_HPP_

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
    std::string readJSON(const std::string& jsonString);

    // Returns a JSON string of the options.
    std::string writeJSON() const;

    bool getOptionNamed(const std::string& name) const;
    void setOptionNamed(const std::string& name, bool value);

private:
    std::unordered_map<std::string, bool> m_options;
};

} // namespace lint

#endif // SRC_LINT_CONFIG_HPP_
