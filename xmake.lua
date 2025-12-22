add_rules("mode.debug", "mode.release")
add_languages("cxx20")
set_warnings("extra", "all", "error")
add_requires("vulkansdk", "vulkan-headers", "vulkan-loader", "libsdl3", "glm")

rule("vertex_shader")
    set_extensions(".vertex.glsl")

    on_build_file(function (target, sourcefile, opt)
        print("Compiling vertex shader '" .. path.filename(sourcefile) .. "'...")
        local project_path = path.absolute(os.scriptdir())
        local input = path.join(project_path, sourcefile)
        local output_relative_path = path.join(path.directory(sourcefile), path.basename(sourcefile) .. ".spv")
        local output = path.join(project_path, output_relative_path)
        print("\tInput: " .. input)
        print("\tOutput: " .. output)
        os.runv("glslc", { "-fshader-stage=vertex", input, "-o", output })
    end)
rule_end()

rule("fragment_shader")
    set_extensions(".fragment.glsl")

    on_build_file(function (target, sourcefile, opt)
        print("Compiling fragment shader '" .. path.filename(sourcefile) .. "'...")
        local project_path = path.absolute(os.scriptdir())
        local input = path.join(project_path, sourcefile)
        local output_relative_path = path.join(path.directory(sourcefile), path.basename(sourcefile) .. ".spv")
        local output = path.join(project_path, output_relative_path)
        print("\tInput: " .. input)
        print("\tOutput: " .. output)
        os.runv("glslc", { "-fshader-stage=fragment", input, "-o", output })
    end)
rule_end()

add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })
target("vulkan_test")
    set_kind("binary")

    add_rules("vertex_shader", "fragment_shader")
    add_files("resources/shaders/*.vertex.glsl")
    add_files("resources/shaders/*.fragment.glsl")

    add_packages("vulkansdk", "vulkan-headers", "libsdl3", "vulkan-loader", "glm")

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
target_end()
