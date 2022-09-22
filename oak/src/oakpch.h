#pragma once

#ifdef OAK_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // OAK_PLATFORM_WINDOWS

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <math.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>


#include <oak/core/Base.h>
#include <oak/core/Log.h>
#include <oak/events/Event.h>


#include "glm/glm.hpp"
// define extra conversion here before including imgui, don't do it in the imconfig.h
#define IM_VEC2_CLASS_EXTRA \
    constexpr ImVec2(glm::vec2& f) : x(f.x), y(f.y) {} \
    operator glm::vec2() const { return glm::vec2(x, y); }

#define IM_VEC4_CLASS_EXTRA \
        constexpr ImVec4(const glm::vec4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {} \
        operator glm::vec4() const { return glm::vec4(x,y,z,w); }

