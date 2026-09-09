import qbs 1.0

Project {
    // Everything a build produces belongs under `output/`, so that a checkout has one directory to ignore
    // and one to delete for a clean slate. QBS defaults its build directory to the current directory, which
    // is what drops a stray `release/` beside the sources, and the VS Code extension defaults to `build/`;
    // neither can be redirected from the project file, so a build directory that lands inside the source
    // tree anywhere but under `output/` is refused instead. One outside the tree writes nothing here and is
    // left alone. `references` is evaluated every time the project resolves, which is what runs the check.
    references: {
        var output = sourceDirectory + "/output";
        var inSourceTree = buildDirectory.indexOf(sourceDirectory + "/") === 0;
        var inOutput = buildDirectory === output || buildDirectory.indexOf(output + "/") === 0;
        if (inSourceTree && !inOutput) {
            throw new Error("kmx-unit writes every build artifact under '" + output + "', but the build "
                + "directory is '" + buildDirectory + "'.\n"
                + "Pass --build-directory output/qbs on the command line, or set qbs.buildDirectory to "
                + "\"${sourceDirectory}/output/qbs/${profileName}_${configurationName}\" in the VS Code "
                + "QBS extension.");
        }
        return [
            "source/library.qbs",
            "example/example.qbs",
            "test/unit-test.qbs",
        ];
    }
}
