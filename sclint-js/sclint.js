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
    .option('--checkAgainst', 'A file to check the output against');

program.parse();

if (program.args.length != 1) {
    console.error("Error: missing input file argument");
    process.exit(1);
}

const options = program.opts();

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
        var issues = linter.issues();
        console.log(linter.issues.length);
        if (severity <= Module.IssueSeverity.kWarning) {
            console.error(issues);
            for (var i = 0; i < issues.length; i++) {
                var issue = issues[i];
                console.log(issue.message);
                console.log("f00");
            }
            process.exit(1);
        }
        console.log(linter.rewrittenString());
    });
}
