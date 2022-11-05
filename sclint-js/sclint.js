#! /usr/bin/env node

const { program } = require('commander')
var Module = require('lint.js');

program
    .name('sclint')
    .description('node.js implementation of sclint, the SuperCollider linter')
    .version('1.0.0');

program
    .option('--printConfig', 'Print config and exit.', false)
    .option('--style <json>', 'A JSON style string')
    .option('--checkAgainst', 'A file to check the output against');

program.parse();

const options = program.opts();
console.log(options);

Module.onRuntimeInitialized = async function(){
    console.log(Module.inspect());
    var config = new Module['Config']();
    console.log(config);
}
