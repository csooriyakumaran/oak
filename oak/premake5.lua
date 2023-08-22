
project "Oak"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")
    
    pchheader "oakpch.h"
    pchsource "src/oakpch.cpp"

    files 
    { 
        "src/**.h", 
        "src/**.cpp",
        "include/**.h"
    }

    includedirs
    {  
        "src",
        "../../wi/wi/src", 
        "../../wi/wi/include", 
        "../vendor/glad/include",
        "../vendor/glfw/include",
        "../vendor/glm",
        "../vendor/stb",
        "../vendor/spdlog/include",
        "../vendor/ImGui",
        "../vendor/implot",
        "../vendor/imnodes",
        "../vendor/yaml-cpp/include"
    }

    links
    {
        "wi",
        "glad",
        "GLFW",
        "opengl32",
        "implot",
        "imnodes",
        "yaml-cpp"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "_USE_MATH_DEFINES",
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE",
    }


    filter "system:windows"
        systemversion "latest"
        defines "WI_PLATFORM_WINDOWS"

    filter  "configurations:Debug" 
        defines { "WI_DEBUG" }
        symbols "On"

    filter  "configurations:Release"
        defines { "WI_RELEASE" }
        optimize "On"

    filter  "configurations:Dist"
        defines { "WI_DIST" }
        optimize "On"