
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
    }

    includedirs
    {  
        "src",
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
        "_CRT_SECURE_NO_WARNINGS"
    }


    filter "system:windows"
        systemversion "latest"
        defines "OAK_PLATFORM_WINDOWS"

    filter  "configurations:Debug" 
        defines { "OAK_DEBUG" }
        symbols "On"

    filter  "configurations:Release"
        defines { "OAK_RELEASE" }
        optimize "On"

    filter  "configurations:Dist"
        defines { "OAK_DIST" }
        optimize "On"