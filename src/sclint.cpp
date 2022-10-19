#include "lint/Config.hpp"
#include "lint/Linter.hpp"

#include "fmt/format.h"
#include "gflags/gflags.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace {

std::unique_ptr<char[]> readFile(std::string fileName, size_t& codeSize) {
    codeSize = std::filesystem::file_size(fileName);
    auto code = std::make_unique<char[]>(codeSize + 1);
    std::ifstream inFile(fileName, std::ifstream::binary);
    if (!inFile) {
        std::cerr << "Error opening input file: " << fileName << "\n";
        return nullptr;
    }
    inFile.read(code.get(), codeSize);
    code.get()[codeSize] = '\0';
    if (!inFile) {
        std::cerr << "Error reading input file: " << fileName << "\n";
        return nullptr;
    }
    inFile.close();
    return code;
}

} // namespace

DEFINE_bool(printConfig, false,
            "If true, sclint will process whatever style commands are present and then print the "
            "current configuration to stdout before exiting.");
DEFINE_string(style, "",
              "If unspecified this means the default SCLang style as documented on the SuperCollider "
              "website.\n\n"
              "-style can also be the keyword \"file\", in which case sclint will search in the current "
              "directory and upwards to find the first .sclint file, which is expected to be a JSON "
              "dictionary of style configuration settings.\n\n"
              "Lastly, the style flag can be a JSON string in which case sclint will parse it directly.");
DEFINE_string(checkAgainst, "",
              "Used for testing. Instead of printing the formatted code to stdout, sclint compares the formatted code "
              "to that provided in the file path, returning an error if they are different.");

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // Configuration Setup
    lint::Config config;
    if (FLAGS_style == "file") {
        auto configPath = std::filesystem::current_path() / ".sclint";
        while (!std::filesystem::exists(configPath)) {
            // Remove the filename first
            configPath = configPath.parent_path();
            if (!configPath.has_parent_path()) {
                std::cerr << "sclint got the -style=file command line, but couldn't find an .sclint file.\n";
                return -1;
            }
            configPath = configPath.parent_path() / ".sclint";
        }
        size_t configFileSize = 0;
        auto configFile = readFile(configPath.string(), configFileSize);
        if (!configFile)
            return -1;
        auto results = config.readJSON(std::string_view(configFile.get(), configFileSize));
        if (results != "") {
            std::cerr << fmt::format("Failed to read JSON .sclint file at '{}': {}\n", configPath.string(), results);
            return -1;
        }
    } else if (FLAGS_style != "") {
        // Expect an inline JSON string.
        auto results = config.readJSON(FLAGS_style);
        if (results != "") {
            std::cerr << fmt::format("Failed to parse inline style string '{}': {}\n", FLAGS_style, results);
            return -1;
        }
    }
    if (FLAGS_printConfig) {
        std::cout << config.writeJSON() << std::endl;
        return 0;
    }

    // Linting
    if (argc != 2) {
        std::cerr << "Missing input file path.\n";
        return -1;
    }

    auto fileName = std::string(argv[1]);
    if (!std::filesystem::exists(fileName)) {
        std::cerr << fmt::format("'{}': file not found.\n", fileName);
        return -1;
    }
    size_t codeSize = 0;
    auto code = readFile(fileName, codeSize);
    if (!code)
        return -1;

    lint::Linter linter(&config, std::string_view(code.get(), codeSize));
    auto severity = linter.lint();
    if (severity == lint::IssueSeverity::kFatal) {
        std::cerr << fmt::format("{} had fatal parsing errors\n", fileName);
        return -1;
    }

    for (const auto& issue : linter.issues()) {
        if (issue.issueSeverity > lint::IssueSeverity::kWarning)
            continue;
        std::cerr << fmt::format("{} line {} col {}: {}\n", fileName, issue.lineNumber, issue.columnNumber,
                                 lint::kIssueTextTable[issue.issueNumber]);
    }

    if (!FLAGS_checkAgainst.empty()) {
        size_t checkFileSize = 0;
        auto checkFile = readFile(FLAGS_checkAgainst, checkFileSize);
        if (!checkFile) {
            std::cerr << fmt::format("Failed to read checkAgainst file at {}\n", FLAGS_checkAgainst);
            return -1;
        }
        if (linter.rewrittenString().compare(std::string_view(checkFile.get(), checkFileSize)) != 0) {
            std::cerr << fmt::format("linter string mismatch against check file {}\n", FLAGS_checkAgainst);
            return -1;
        }
        return 0;
    }

    if (severity > lint::IssueSeverity::kError) {
        std::cout << linter.rewrittenString();
    }

    return severity < lint::IssueSeverity::kWarning ? 1 : 0;
}
