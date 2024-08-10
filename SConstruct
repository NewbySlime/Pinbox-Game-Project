#!/usr/bin/env python
import os
import sys


# User variables
build_path = "./godot-workspace/bin"
base_source_path = "src"
godotcpp_path = "godot-cpp-4.2"

env = SConscript(godotcpp_path+"/SConstruct")


# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=[base_source_path+"/"])
env.Append(CXXFLAGS=["/EHsc"])
sources = Glob(base_source_path+"/*.cpp")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "{}/{}.{}.{}.framework/{}.{}.{}".format(
            build_path, "libgdex", env["platform"], env["target"], "libgdex", env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "{}/{}{}{}".format(
            build_path, "libgdex", env["suffix"], env["SHLIBSUFFIX"]
        ),
        source=sources,
    )

Default(library)