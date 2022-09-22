#pragma once


#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
// For use by oak applications

#include "oak/core/Application.h"
#include "oak/core/Base.h"
#include "oak/core/Log.h"
#include "oak/core/Assert.h"
#include "oak/core/Layer.h"
#include "oak/core/UUID.h"
#include "oak/core/Input.h"
#include "oak/core/Timestep.h"
#include "oak/core/KeyCodes.h"
#include "oak/core/MouseCodes.h"


#include "oak/UI/Panel.h"
#include "oak/UI/PanelManager.h"
#include "oak/UI/Node.h"
#include "oak/UI/NodeStack.h"

#include "oak/plotting/DataSeries.h"


// ---IMGUI----------------------------
#include "oak/imgui/ImGuiBaseLayer.h"
#include "oak/imgui/ImguiUI.h"
#include "oak/imgui/ImguiUtils.h"
//-------------------------------------

// ---Renderer------------------------
#include "oak/renderer/Renderer.h"
#include "oak/renderer/RenderCommand.h"
#include "oak/renderer/Texture.h"
// -----------------------------------



// ---FILE I/O + SERIALIZATION--------
#include "oak/utilities/FileSystem.h"
#include "oak/utilities/YAMLSerialization.h"
#include "oak/serialization/ImGuiStyleSerializer.h"