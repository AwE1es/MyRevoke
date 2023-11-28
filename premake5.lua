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
    IncludeDir ["STB_IMAGE"] = "MyRevoke/vendor/stb_image"

    group "Dependencies"
        include "MyRevoke/vendor/GLFW"
        include "MyRevoke/vendor/GLAD"
        include "MyRevoke/vendor/imgui"

    group ""

    project "MyRevoke"
        location "MyRevoke"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "rvpch.h"
        pchsource "MyRevoke/src/rvpch.cpp"

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/vendor/glm/glm/**.hpp",
            "%{prj.name}/vendor/glm/glm/**.inl",
            "%{prj.name}/vendor/stb_image/stb_image.h",
            "%{prj.name}/vendor/stb_image/stb_image.cpp",
           
        }
        defines
        {
            "_CRT_SECURE_NO_WARNINGS"
        }

        includedirs
        {
            "%{prj.name}/src",
            "%{prj.name}/vendor/spdlog/include",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.GLAD}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.GLM}",
            "%{IncludeDir.STB_IMAGE}"
        }

        links
        {
            "GLAD",
            "GLFW",
            "ImGui",
            "opengl32.lib"
        }

        filter "system:windows"
            systemversion "latest"

            defines
            {
                "RV_PLATFORM_WINDOWS",
                "RV_BUILD_DLL",
                "GLFW_INCLUDE_NONE",
                "IMGUI_DEFINE_MATH_OPERATORS",
                "RV_ASSERTS_ENABLE",
                "_CRT_SECURE_NO_WARNINGS",
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
        cppdialect "C++20"
        language "C++"
        staticruntime "on"
    
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
          
        }
    
        includedirs
        {
            "MyRevoke/vendor/spdlog/include",
            "MyRevoke/src",
            "%{IncludeDir.GLM}",
            "%{IncludeDir.ImGui}",
        }
    
        links
        {
            "MyRevoke"
        }
    
        filter "system:windows"
            
            systemversion "latest"
    
            defines
            {
                "RV_PLATFORM_WINDOWS",
                "_CRT_SECURE_NO_WARNINGS"
            }
    
        filter "configurations:Debug"
            defines "RV_DEBUG"
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            defines "RV_RELEASE"
            runtime "Release"
            optimize "On"
