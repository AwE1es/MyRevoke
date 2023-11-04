workspace "MyRevoke"
    architecture "x64"
    startproject "SandBox"

    configurations { "Debug", "Release" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
    IncludeDir = {};
    IncludeDir ["GLFW"] = "MyRevoke/vendor/GLFW/include"
    IncludeDir ["ImGui"] = "MyRevoke/vendor/imgui"
    IncludeDir ["GLAD"] = "MyRevoke/vendor/GLAD/include"
    IncludeDir ["GLM"] = "MyRevoke/vendor/glm"

    group "Dependencies"
        include "MyRevoke/vendor/GLFW"
        include "MyRevoke/vendor/GLAD"
        include "MyRevoke/vendor/imgui"

    group ""

    project "MyRevoke"
        location "MyRevoke"
        kind "SharedLib"
        language "C++"
        staticruntime "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "rvpch.h"
        pchsource "MyRevoke/src/rvpch.cpp"

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
           
        }


        includedirs
        {
            "%{prj.name}/src",
            "%{prj.name}/vendor/spdlog/include",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.GLAD}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.GLM}"
        }

        links
        {
            "GLAD",
            "GLFW",
            "ImGui",
            "opengl32.lib"
        }

        filter "system:windows"
            cppdialect "C++20"
            systemversion "latest"

            defines
            {
                "RV_PLATFORM_WINDOWS",
                "RV_BUILD_DLL",
                "GLFW_INCLUDE_NONE",
                "IMGUI_DEFINE_MATH_OPERATORS",
                "RV_ASSERTS_ENABLE"
            }

            postbuildcommands
            {
               ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
            }

        filter "configurations:Debug"
            defines "RV_DEBUG"
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines "RV_RELEASE"
            runtime "Release"
            optimize "On"


    project "SandBox"
        location "SandBox"
        kind "ConsoleApp"
    
        language "C++"
        staticruntime "off"
    
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }
    
        includedirs
        {
            "MyRevoke/vendor/spdlog/include",
            "MyRevoke/src",
            "%{IncludeDir.GLM}"
        }
    
        links
        {
            "MyRevoke"
        }
    
        filter "system:windows"
            cppdialect "C++20"
            systemversion "latest"
    
            defines
            {
                "RV_PLATFORM_WINDOWS"
            }
    
        filter "configurations:Debug"
            defines "RV_DEBUG"
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            defines "RV_RELEASE"
            runtime "Release"
            optimize "On"
