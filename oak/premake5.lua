
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
        "../vendor/glad/include",
        "../vendor/glfw/include",
        "../vendor/glm",
        "../vendor/stb",
        "../vendor/spdlog/include",
        "../vendor/imgui",
        "../vendor/implot",
    }

    links
    {
        "glad",
        "GLFW",
        "gdi32", 
        "Dwmapi",
        "opengl32",
        "ImPlot",
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
    }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")

    -- postbuildcommands{
       
    --     "{MKDIR} build/" ..outputdir .."/oak-obj",
    --     "ar -xv  %{cfg.buildtarget.abspath} --output build/" .. outputdir .. "/oak-obj",
              
    --     "{MKDIR} build/" ..outputdir .."/ImPlot-obj",
    --     "{CHDIR} .. && ar -xv vendor/implot/bin/" .. outputdir .. "/ImPlot/ImPlot.lib --output oak/build/" .. outputdir .. "/ImPlot-obj",
       
    --     "{MKDIR} build/" ..outputdir .."/GLFW-obj",
    --     "CHDIR .. && ar -xv vendor/glfw/bin/" .. outputdir .. "/GLFW/GLFW.lib --output oak/build/" .. outputdir .. "/GLFW-obj",
        
    --     -- "{MKDIR} build/" ..outputdir .."/gdi32-obj",
    --     -- "ar -xv C:/msys64/mingw64/lib/libgdi32.a build" .. outputdir .. "/gdi32-obj",
       
    --     "ar -rcs %{cfg.buildtarget.abspath} build/" .. outputdir .."/ImPlot-obj/*.o build/" .. outputdir .."/GLFW-obj/*.o build/" .. outputdir .."/oak-obj/*.o",

    --     -- "{MKDIR} build/" ..outputdir .."/vulkan-obj",
    --     -- "ar -xv C:/VulkanSDK/1.3.216.0/Lib/vulkan-1.lib --output build/" .. outputdir .. "/vulkan-obj",

    --     "{RMDIR} build/" ..outputdir .."/ImPlot-obj",
    --     "{RMDIR} build/" ..outputdir .."/GLFW-obj",
    --     -- "{RMDIR} build/" ..outputdir .."/gdi32-obj",
    --     "{RMDIR} build/" ..outputdir .."/oak-obj",
    -- }

    filter "system:windows"
        systemversion "latest"
        defines "OAK_PLATFORM_WINDOWS"

    filter  "configurations:Debug" 
        defines { "DEBUG" }
        symbols "On"

    filter  "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter  "configurations:Release"
        defines { "WL_DIST" }
        optimize "On"