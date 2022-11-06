#! /usr/bin/env node

// You will need to provide the path to the lint.js build product as part of the NODE_PATH environment variable to
// node, or this program will not find it.

const { program } = require('commander')
const fs = require('fs');
var Module = require('lint.js');

program
    .name('sclint')
    .description('node.js implementation of sclint, the SuperCollider linter')
    .version('1.0.0');

program
    .option('--printConfig', 'Print config and exit.', false)
    .option('--style <json>', 'A JSON style string. TODO: add .sclint support.')
    .option('--checkAgainst <filename>', 'A file to check the output against');

program.parse();

const options = program.opts();
console.log(options);

if (program.args.length != 1) {
    console.error("Error: missing input file argument");
    process.exit(1);
}

Module.onRuntimeInitialized = async function(){
    var config = new Module['Config']();

    // Parse style JSON if provided.
    if (options.style != undefined) {
        var result = config.readJSON(options.style);
        if (result.length > 0) {
            console.error(result);
            process.exit(1);
        }
    }

    // Print config and exit if requested.
    if (options.printConfig) {
        console.log(config.writeJSON());
        process.exit(0);
    }

    fs.readFile(program.args[0], 'utf8', (err, code) => {
        if (err) {
            console.error(err);
            process.exit(1);
        }

        var linter = new Module['Linter']();
        var severity = linter.lint(config, code);
        if (severity == Module.IssueSeverity.kFatal) {
            console.error(program.args[0] + ' had fatal parsing errors.');
            process.exit(1);
        }

        var issues = linter.issues();
        for (var i = 0; i < issues.size(); i++) {
            var issue = issues.get(i)
            if (issue.issueSeverity == Module.IssueSeverity.kWarning ||
                issue.issueSeverity == Module.IssueSeverity.kFatal) {
                console.log(program.args[0] + ' line ' + issue.lineNumber + ' col ' + issue.columnNumber +
                    ' (' + issue.detectorName + '): ' + issue.message);
            }
        }

        var rewritten = linter.rewrittenString();

        if (options.checkAgainst != undefined) {
            fs.readFile(options.checkAgainst, 'utf8', (err, check) => {
                if (err) {
                    console.error(err);
                    process.exit(1);
                }

                if (check != rewritten) {
                    console.error('linter format mismatch against check file ' + options.checkAgainst);
                    process.exit(1);
                }
            });

            process.exit(0);
        }

        if (severity != Module.IssueSeverity.kError) {
            console.log(rewritten);
        }

        if (severity == Module.IssueSeverity.kError ||
            severity == Module.IssueSeverity.kWarning) {
            process.exit(1);
        }
    });
}
