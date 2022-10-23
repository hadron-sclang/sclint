#ifndef SRC_LINT_CONFIG_HPP_
#define SRC_LINT_CONFIG_HPP_

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

    // ** Whitespace **

    // https://github.com/supercollider/supercollider/wiki/Code-style-guidelines#rule-use-exactly-one-newline-at-the-end-of-a-file
    bool oneNewlineAtEndOfFile = true;
    static constexpr const char* kOneNewlineAtEndOfFileName = "oneNewlineAtEndOfFile";

    // https://github.com/supercollider/supercollider/wiki/Code-style-guidelines#rule-dont-end-lines-with-whitespace-characters
    bool noTrailingWhitespace = true;

    // https://github.com/supercollider/supercollider/wiki/Code-style-guidelines#rule-dont-end-lines-with-whitespace-characters
    int spacesAroundBinaryOperators = 1;

    // https://github.com/supercollider/supercollider/wiki/Code-style-guidelines#rule-add-spaces-after-commas
    int spacesAfterComma = 1;
    int spacesBeforeComma = 0;

    // https://github.com/supercollider/supercollider/wiki/Code-style-guidelines#rule-dont-use-spaces-before-semicolons
    int spacesBeforeSemicolons = 0;

    // https://github.com/supercollider/supercollider/wiki/Code-style-guidelines#rule-add-spaces-within-curly-brackets--but-not-parentheses--square-brackets--or-argument-lists-
    int spacesWithinSingleLineCurlyBraces = 1;
    int spacesWithinSingleLineParens = 0;
    int spacesWithinSingleLineSquareBrackets = 0;
    int spacesWithinSingleLineArgumentPipes = 0;

    // https://github.com/supercollider/supercollider/wiki/Code-style-guidelines#rule-use-tabs-for-indentation
    bool useTabsForIndentation = true;
    int spacesForIndentation = 0; // Ignored if useTabsForIndentation is true

    // https://github.com/supercollider/supercollider/wiki/Code-style-guidelines#rule-use-kr-style-for-multi-line-blocks
    enum IndentingStyle { KandR = 0, Allman = 1 };
    IndentingStyle indentingStyle = IndentingStyle::KandR;

    // ** Method Calls **

    // ** Unofficial Style Rules (default disabled) **
    bool noMethodReturnWithLexicalScope = false;
    static constexpr const char* kNoMethodReturnWithLexicalScopeName = "noMethodReturnWithLexicalScope";

    bool noSingleLetterVariableNames = false;

    bool lintTest = false;
    static constexpr const char* kLintTestName = "lintTest";

    bool warnOnCurryArgument = false;
    static constexpr const char* kWarnOnCurryArgumentName = "warnOnCurryArgument";

private:
    std::unordered_map<std::string, bool> m_options;
};

} // namespace sclint

#endif // SRC_LINT_CONFIG_HPP_
