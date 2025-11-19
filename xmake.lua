add_rules("mode.debug", "mode.release")
add_languages("cxx20")
set_warnings("extra", "all", "error")

add_requires("vulkan-headers", "vulkan-loader", "libsdl3")

add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })
target("vulkan_test")
    set_kind("binary")
    
    add_packages("vulkan-headers", "vulkan-loader", "libsdl3")

    if is_mode("debug") then
        set_optimize("none")
    end

    add_files("src/*.cpp")
    add_files("src/VulkanHelpers/*.cpp")
    add_files("src/SDLHelpers/*.cpp")

    add_headerfiles("inc/*.hpp")
    add_headerfiles("inc/VulkanHelpers/*.hpp")
    add_headerfiles("inc/SDLHelpers/*.hpp")

    set_rundir(".")

    add_includedirs("inc")
