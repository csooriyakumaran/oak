workspace "Oak"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
VULKAN_SDK = os.getenv("VULKAN_SDK")

include "dependencies.lua"
include "oak"