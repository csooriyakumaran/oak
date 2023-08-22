#pragma once
// For use by oak applications

#include "../src/oak/core/Application.h"
#include "../src/oak/core/Base.h"
#include "../src/oak/core/Layer.h"
#include "../src/oak/core/Input.h"
#include "../src/oak/core/KeyCodes.h"
#include "../src/oak/core/MouseCodes.h"


#include "../src/oak/UI/Panel.h"
#include "../src/oak/UI/PanelManager.h"
#include "../src/oak/UI/Node.h"
#include "../src/oak/UI/NodeStack.h"



// ---IMGUI----------------------------
#include "../src/oak/imgui/ImGuiBaseLayer.h"
#include "../src/oak/imgui/ImguiUI.h"
#include "../src/oak/imgui/ImguiUtils.h"
//-------------------------------------

// ---Renderer------------------------
#include "../src/oak/renderer/Renderer.h"
#include "../src/oak/renderer/RenderCommand.h"
#include "../src/oak/renderer/Texture.h"
// -----------------------------------



// ---FILE I/O + SERIALIZATION--------
#include "../src/oak/utilities/FileSystem.h"
#include "../src/oak/utilities/YAMLSerialization.h"
#include "../src/oak/serialization/ImGuiStyleSerializer.h"