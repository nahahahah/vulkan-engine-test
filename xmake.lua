add_rules("mode.debug", "mode.release")
add_languages("cxx20")
set_warnings("extra", "all", "error")

add_requires("vulkan-headers", "vulkan-loader", "libsdl3", "glm")

add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })
target("vulkan_test")
    set_kind("binary")

    add_packages("vulkan-headers", "libsdl3")
    add_packages("vulkan-loader", "glm")

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
    add_files("src/Math/*.cpp")

    add_headerfiles("inc/*.hpp")
    add_headerfiles("inc/VulkanHelpers/*.hpp")
    add_headerfiles("inc/VulkanHelpers/CreateInfos/*.hpp")
    add_headerfiles("inc/VulkanHelpers/Flags/*.hpp")
    add_headerfiles("inc/VulkanHelpers/Handles/*.hpp")
    add_headerfiles("inc/VulkanHelpers/ParameterEnums/*.hpp")
    add_headerfiles("inc/VulkanHelpers/ParameterInfos/*.hpp")
    add_headerfiles("inc/VulkanHelpers/ParameterStructs/*.hpp")
    add_headerfiles("inc/SDLHelpers/*.hpp")
    add_headerfiles("inc/Math/*.hpp")

    set_rundir(".")

    set_configdir("$(builddir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("resources/shaders/*", { onlycopy = true, prefixdir = "resources/shaders" })

    set_installdir("$(builddir)/$(plat)/$(arch)/$(mode)/")
    add_installfiles("resources/shaders/*", { prefixdir = "bin/resources/shaders" })

    add_includedirs("inc")

    before_run(function (target)
        local project_path = path.absolute(os.scriptdir())
        os.cd(project_path)

        local fragment_shader_input_path = path.join(project_path, "resources/shaders/triangle.fragment.glsl")
        local fragment_shader_output_path = path.join(project_path, "resources/shaders/triangle.fragment.spv")
        print("Compiling fragment shader...")
        print("\tInput: " .. fragment_shader_input_path)
        print("\tOutput: " .. fragment_shader_output_path)
        os.runv("glslc", { "-fshader-stage=fragment", fragment_shader_input_path, "-o", fragment_shader_output_path })

        local vertex_shader_input_path = path.join(project_path, "resources/shaders/triangle.vertex.glsl")
        local vertex_shader_output_path = path.join(project_path, "resources/shaders/triangle.vertex.spv")
        print("Compiling vertex shader...")
        print("\tInput: " .. vertex_shader_input_path)
        print("\tOutput: " .. vertex_shader_output_path)
        os.runv("glslc", { "-fshader-stage=vertex", vertex_shader_input_path, "-o", vertex_shader_output_path })
    end)
target_end()
