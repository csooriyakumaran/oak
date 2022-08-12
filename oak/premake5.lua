
project "Oak"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    pchheader "oakpch.h"

    files 
    { 
        "src/**.h", 
        "src/**.cpp",
    }

    includedirs
    {  
        "src",
        "../vendor/ImGui",
        "../vendor/implot",
        "../vendor/glfw/include",
        "../vendor/stb",
        "../vendor/spdlog/include",
        "C:/VulkanSDK/1.3.216.0/Include",
    }
    links
    {
        "GLFW",
        "gdi32",
        "Dwmapi",
        "ImPlot",
        "C:/VulkanSDK/1.3.216.0/Lib/vulkan-1",
    }
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")

    postbuildcommands{
        "{MOVE} %{cfg.buildtarget.abspath} temp.a",
       
        "{MKDIR} build/" ..outputdir .."/ImPlot-obj",
        "{CHDIR} .. && ar -xv vendor/implot/bin/" .. outputdir .. "/ImPlot/ImPlot.lib --output oak/build/" .. outputdir .. "/ImPlot-obj",
       
        "{MKDIR} build/" ..outputdir .."/GLFW-obj",
        "CHDIR .. && ar -xv vendor/glfw/bin/" .. outputdir .. "/GLFW/GLFW.lib --output oak/build/" .. outputdir .. "/GLFW-obj",
       
        "ar -rcs %{cfg.buildtarget.abspath} build/" .. outputdir .."/ImPlot-obj/*.o build/" .. outputdir .."/GLFW-obj/*.o build/" .. outputdir .."/Oak/*.o",

        -- "{MKDIR} build/" ..outputdir .."/vulkan-obj",
        -- "ar -xv C:/VulkanSDK/1.3.216.0/Lib/vulkan-1.lib --output build/" .. outputdir .. "/vulkan-obj",

        "{RMDIR} build/" ..outputdir .."/ImPlot-obj",
        "{RMDIR} build/" ..outputdir .."/GLFW-obj",
    }

    filter "system:windows"
        systemversion "latest"
        defines "WL_PLATFORM_WINDOWS"

    filter  "configurations:Debug" 
        defines { "DEBUG" }
        symbols "On"

    filter  "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter  "configurations:Release"
        defines { "WL_DIST" }
        optimize "On"