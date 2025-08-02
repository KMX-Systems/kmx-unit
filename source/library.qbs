import qbs

StaticLibrary {
    name: "unit"
    Depends {
        name: "cpp"
    }
    files: [
        "inc/kmx/unit/acceleration.hpp",
        "inc/kmx/unit/all.hpp",
        "inc/kmx/unit/angle.hpp",
        "inc/kmx/unit/angular_velocity.hpp",
        "inc/kmx/unit/area.hpp",
        "inc/kmx/unit/base.hpp",
        "inc/kmx/unit/data_rate.hpp",
        "inc/kmx/unit/distance.hpp",
        "inc/kmx/unit/duration.hpp",
        "inc/kmx/unit/electromagnetism.hpp",
        "inc/kmx/unit/energy.hpp",
        "inc/kmx/unit/force.hpp",
        "inc/kmx/unit/illuminance.hpp",
        "inc/kmx/unit/information.hpp",
        "inc/kmx/unit/luminous_intensity.hpp",
        "inc/kmx/unit/mass.hpp",
        "inc/kmx/unit/power.hpp",
        "inc/kmx/unit/pressure.hpp",
        "inc/kmx/unit/temperature.hpp",
        "inc/kmx/unit/text.hpp",
        "inc/kmx/unit/velocity.hpp",
        "inc/kmx/unit/volume.hpp",
    ]
    cpp.cxxLanguageVersion: "c++23"
    //cpp.cxxFlags: "-gdwarf-4"
    cpp.enableRtti: false
    cpp.includePaths: [
        "api",
        "inc",
        "inc_dep",
    ]
    //cpp.staticLibraries: ["pthread"]
}
