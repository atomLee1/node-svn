{
    "variables": {
        "clang": 0
    },
    "targets": [
        {
            "target_name": "svn",
            "dependencies": [
                "deps/apr/apr.gyp:apr",
                "deps/subversion/client.gyp:libsvn_client"
            ],
            "include_dirs": [
                "src"
            ],
            "defines!": [
                "_HAS_EXCEPTIONS=0"
            ],
            "sources": [
                "src/cpp/client.cpp",
                "src/cpp/malloc.cpp",
                "src/cpp/svn_error.cpp",
                "src/node/auth/simple.cpp",
                "src/node/export.cpp",
                "src/node/node_client.cpp"
            ],
            "cflags_cc": [
                "-std=gnu++17",
                "-fexceptions"
            ],
            "cflags_cc!": [
                "-fno-rtti"
            ],
            "ldflags": [
                "-static-libstdc++",
                "-static-libgcc"
            ],
            "xcode_settings": {
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "CLANG_CXX_LANGUAGE_STANDARD": "gnu++17",
                "MACOSX_DEPLOYMENT_TARGET": "10.13",
                "OTHER_LDFLAGS": [
                    "-static-libstdc++",
                    "-static-libgcc"
                ]
            },
            "msvs_settings": {
                "VCCLCompilerTool": {
                    "AdditionalOptions": [
                        "/std:c++17"
                    ],
                    "DisableSpecificWarnings": [
                        "4005"
                    ],
                    "ExceptionHandling": 1
                }
            },
            "conditions": [
                [
                    "OS == 'win'",
                    {
                        "libraries": [
                            "ws2_32.lib",
                            "Mincore.lib"
                        ]
                    },
                    {
                        "libraries": [
                            "-liconv"
                        ]
                    }
                ]
            ]
        }
    ]
}
