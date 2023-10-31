workspace "MyRevoke"
    architecture "x64"
    startproject "SandBox"

    configurations { "Debug", "Release" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
    IncludeDir = {};
    IncludeDir ["GLFW"] = "MyRevoke/vendor/GLFW/include"
    IncludeDir ["GLAD"] = "MyRevoke/vendor/GLAD/include"

    include "MyRevoke/vendor/GLFW"
    include "MyRevoke/vendor/GLAD"

    project "MyRevoke"
        location "MyRevoke"
        kind "SharedLib"
        language "C++"

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
            "%{IncludeDir.GLAD}" 
        }

        links
        {
            "GLAD",
            "GLFW",
            "opengl32.lib"
        }

        filter "system:windows"
            cppdialect "C++20"
            staticruntime "On"
            systemversion "latest"

            defines
            {
                "RV_PLATFORM_WINDOWS",
                "RV_BUILD_DLL",
                "GLFW_INCLUDE_NONE"
            }

            postbuildcommands
            {
               ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
            }

        filter "configurations:Debug"
            defines "RV_DEBUG"
            buildoptions"/MDd"
            symbols "On"

        filter "configurations:Release"
            defines "RV_RELEASE"
            buildoptions"/MD"
            optimize "On"

        filter { "system:windows", "configurations:Release" }
            buildoptions "/MT"

    project "SandBox"
        location "SandBox"
        kind "ConsoleApp"
    
        language "C++"
    
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
            "MyRevoke/src"
        }
    
        links
        {
            "MyRevoke"
        }
    
        filter "system:windows"
            cppdialect "C++20"
            staticruntime "On"
            systemversion "latest"
    
            defines
            {
                "RV_PLATFORM_WINDOWS"
            }
    
        filter "configurations:Debug"
            defines "RV_DEBUG"
            buildoptions"/MDd"
            symbols "On"
    
        filter "configurations:Release"
            defines "RV_RELEASE"
            buildoptions"/MD"
            optimize "On"
    
        filter { "system:windows", "configurations:Release" }
            buildoptions "/MTd"