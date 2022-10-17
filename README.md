# sclint
Prototype linter/formatter for SuperCollider code

## Setup

Requirements: CMake, ANTLR 4.11+. Optional: Ninja.

To build:

```
    mkdir build
    cd build
    cmake ..
    cmake --build .
```

The GitHub actions scripts contain detailed examples of building on macOS, Linux, and Windows.

## Adding New Linter Features

Linter features are subclasses of `Detector`, defined in src/lint/detectors/Detector.hpp.

1. Decide on a name for your new detector. We typically name detectors after the behavior we want the user to follow,
   for example, `WriteGreatCode`. The option name will be `writeGreatCode`, the first character is lower case.
2. Write tests first! Create a new directory in tests/WriteGreatCode and include at least one test in it.
3. Make a copy of src/lint/detectors/ExampleDetector.hpp to your detector name, for example
   src/lint/detectors/WriteGreatCode.hpp.
4. Resolve all the TODOs in your copy of example file.
5. Add your Detector to the list of files in src/lint/CMakeLists.txt
6. Add your Detector to the list of files in src/lint/DetectorList.hpp

## WASM Build (experimental)

**macOS**: Can install emscripten SDK or `brew install emscripten`.

```
    mkdir build_em && cd build_em
    emcmake cmake ..
    cmake --build .
```

