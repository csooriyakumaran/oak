-- oakExternak.lua
print("%{wks.location}")

group "dependencies"
    include "vendor/glad"
    include "vendor/glfw"
    include "vendor/imgui"
    include "vendor/implot"
    include "vendor/imnodes"
group ""
group "core"
    include "oak"
group""

