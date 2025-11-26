add_rules("mode.debug", "mode.release")
add_languages("cxx20")
set_warnings("extra", "all", "error")

add_requires("vulkan-headers", "vulkan-loader", "libsdl3")

add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })
target("vulkan_test")
    set_kind("binary")

    add_packages("vulkan-headers", "libsdl3")
    if is_plat("windows") then
        add_packages("vulkan-loader")
    end

    if is_mode("debug") then
        set_optimize("none")
    end

    add_files("src/*.cpp")
    add_files("src/VulkanHelpers/CreateInfos/*.cpp")
    add_files("src/VulkanHelpers/Flags/*.cpp")
    add_files("src/VulkanHelpers/Handles/*.cpp")
    add_files("src/VulkanHelpers/ParameterEnums/*.cpp")
    add_files("src/VulkanHelpers/ParameterInfos/*.cpp")
    add_files("src/VulkanHelpers/ParameterStructs/*.cpp")
    add_files("src/SDLHelpers/*.cpp")

    -- add_headerfiles("inc/*.hpp") -- useless for now
    add_headerfiles("inc/VulkanHelpers/*.hpp")
    add_headerfiles("inc/VulkanHelpers/CreateInfos/*.hpp")
    add_headerfiles("inc/VulkanHelpers/Flags/*.hpp")
    add_headerfiles("inc/VulkanHelpers/Handles/*.hpp")
    add_headerfiles("inc/VulkanHelpers/ParameterEnums/*.hpp")
    add_headerfiles("inc/VulkanHelpers/ParameterInfos/*.hpp")
    add_headerfiles("inc/VulkanHelpers/ParameterStructs/*.hpp")
    add_headerfiles("inc/SDLHelpers/*.hpp")

    set_rundir(".")

    set_installdir("$(builddir)/$(plat)/$(arch)/$(mode)/")
    add_installfiles("resources/shaders/*", { prefixdir = "bin/resources/shaders" })

    add_includedirs("inc")
target_end()
