import qbs

CppApplication {
    name: "example"
    Depends
    {
        name: 'unit'
    }
    consoleApplication: true
    cpp.debugInformation: true

    files: [
        "src/kmx/unit/example.cpp",
    ]
    cpp.cxxLanguageVersion: "c++23"
    cpp.enableRtti: false
    cpp.warningLevel: "all"
    cpp.treatWarningsAsErrors: true
}
