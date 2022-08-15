project "oakApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
   
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "src/**.h", 
        "src/**.cpp",
    }
    
    includedirs
    {  
        "%{wks.location}/oak/vendor/spdlog/include",
        "%{wks.location}/oak/oak/src",
        "%{wks.location}/oak/vendor",
        "%{wks.location}/oak/vendor/glad/include",
        "%{wks.location}/oak/vendor/glfw/include",
        "%{wks.location}/oak/vendor/glm",
    }

    links
    {
        "oak"
    }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")

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