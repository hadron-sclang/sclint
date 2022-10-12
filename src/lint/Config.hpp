#ifndef SRC_LINT_CONFIG_HPP_
#define SRC_LINT_CONFIG_HPP_

#include <string_view>
#include <string>

namespace lint {

// Holds the current configuration of the linter.
struct Config {
public:
    // Build a default configuration (SCStyle)
    Config() = default;
    ~Config() = default;

    std::string readJSON(std::string_view jsonString);
    std::string writeJSON() const;

    // These flags are all named for the *desired* code to pass linting.

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

    bool noCarriageReturnsInFile = false;
    static constexpr const char* kNoCarriageReturnsInFileName = "noCarriageReturnsInFile";

    bool noSingleLetterVariableNames = false;

    bool lintTest = false;
    static constexpr const char* kLintTestName = "lintTest";
};

} // namespace sclint

#endif // SRC_LINT_CONFIG_HPP_