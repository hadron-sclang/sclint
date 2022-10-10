#include "detectors/Detector.hpp"
#include "detectors/MethodReturnLexicalScope.hpp"
#include "detectors/Whitespace.hpp"

#include "fmt/format.h"
#include "gflags/gflags.h"
#include "SCLexer.h"
#include "SCParser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace {

std::unique_ptr<char[]> readFile(std::string fileName, size_t& codeSize) {
    codeSize = std::filesystem::file_size(fileName);
    auto code = std::make_unique<char[]>(codeSize);
    std::ifstream inFile(fileName, std::ifstream::binary);
    if (!inFile) {
        std::cerr << "Error opening input file: " << fileName << "\n";
        return nullptr;
    }
    inFile.read(code.get(), codeSize);
    if (!inFile) {
        std::cerr << "Error reading input file: " << fileName << "\n";
        return nullptr;
    }
    inFile.close();
    return code;
}

} // namespace

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (argc != 2) {
        std::cerr << "Missing input file path.\n";
        return -1;
    }

    auto fileName = std::string(argv[1]);
    size_t codeSize = 0;
    auto code = readFile(fileName, codeSize);
    if (!code)
        return -1;

    antlr4::ANTLRInputStream input(code.get(), codeSize);
    sprklr::SCLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    sprklr::SCParser parser(&tokens);
    auto parseTree = parser.root();

    if (parser.getNumberOfSyntaxErrors()) {
        std::cerr << fmt::format("{} had {} syntax errors\n", fileName, parser.getNumberOfSyntaxErrors());
        return -1;
    }

    sclint::DetectorMux mux;
    mux.addDetector(std::make_unique<sclint::MethodReturnLexicalScope>());
    mux.addDetector(std::make_unique<sclint::Whitespace>(&tokens));

    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&mux, parseTree);

    for (const auto& issue : mux.issues()) {
        std::cout << fmt::format("{} line {} col {}: {}\n", fileName, issue.lineNumber, issue.columnNumber,
                                 issue.message);
    }

    return 0;
}