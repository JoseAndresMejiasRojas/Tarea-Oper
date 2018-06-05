import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        consoleApplication: true
        files: [
            "../controlador.cpp",
            "../controlador.h",
            "../main.cpp",
            "controlador.cpp",
            "controlador.h",
            "main.cpp",
        ]

        Group {     // Properties for the produced executable
            fileTagsFilter: product.type
            qbs.install: true
        }
    }
}
