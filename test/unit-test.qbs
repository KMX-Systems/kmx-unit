import qbs

CppApplication {
    Depends
    {
        name: 'unit'
    }
    consoleApplication: true
    cpp.debugInformation: true

    files: [
        "inc/kmx/unit/testing.hpp",
        "src/kmx/unit/acceleration.cpp",
        "src/kmx/unit/angle.cpp",
        "src/kmx/unit/angular_velocity.cpp",
        "src/kmx/unit/area.cpp",
        "src/kmx/unit/derived.cpp",
        "src/kmx/unit/distance.cpp",
        "src/kmx/unit/duration.cpp",
        "src/kmx/unit/electromagnetism.cpp",
        "src/kmx/unit/energy.cpp",
        "src/kmx/unit/force.cpp",
        "src/kmx/unit/illuminance.cpp",
        "src/kmx/unit/luminous_intensity.cpp",
        "src/kmx/unit/mass.cpp",
        "src/kmx/unit/power.cpp",
        "src/kmx/unit/pressure.cpp",
        "src/kmx/unit/temperature.cpp",
        "src/kmx/unit/velocity.cpp",
        "src/kmx/unit/volume.cpp",
    ]
    cpp.cxxLanguageVersion: "c++23"
    cpp.enableRtti: false
    //cpp.cxxFlags: "-gdwarf-4"
    cpp.includePaths: [
        "inc",
        "inc_dep"
    ]
    //cpp.includePaths: ["/usr/local/include"]
    cpp.libraryPaths: ["/usr/local/lib"]
    cpp.dynamicLibraries: ["Catch2Main", "Catch2"]
}
