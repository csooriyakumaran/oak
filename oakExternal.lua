-- oakExternak.lua
VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["glm"] = "../vendor/glm"
IncludeDir["glad"] = "../vendor/Glad/include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"


group "dependencies"
    include "vendor/glad"
    include "vendor/glfw"
    include "vendor/ImGui"
    include "vendor/implot"
group ""
group "core"
    include "oak"
group""

