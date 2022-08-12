project "oakApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    files { "src/**.h", "src/**.cpp" }

    includedirs
    {
       "../vendor/imgui",
       "../vendor/glfw/include",

       "../oak/src",

       "%{IncludeDir.VulkanSDK}",
       "%{IncludeDir.glm}",
    }

     links
     {
         "oak"
     }

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../build/" .. outputdir .. "/%{prj.name}")

    filter "system:windows"
       systemversion "latest"
       defines { "WL_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
       defines { "WL_DEBUG" }
       runtime "Debug"
       symbols "On"

    filter "configurations:Release"
       defines { "WL_RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

    filter "configurations:Dist"
       kind "WindowedApp"
       defines { "WL_DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"