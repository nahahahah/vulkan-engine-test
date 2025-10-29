add_rules("mode.debug", "mode.release")
add_languages("cxx20")
set_warnings("extra", "all", "error")

add_requires("vulkan-headers", "vulkan-loader", "libsdl3")

add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })
target("vulkan_test")
    add_packages("vulkan-headers", "vulkan-loader", "libsdl3")

    if is_mode("debug") then
        set_optimize("none")
    end

    set_rundir(".")
    set_kind("binary")
    add_files("src/*.cpp")
