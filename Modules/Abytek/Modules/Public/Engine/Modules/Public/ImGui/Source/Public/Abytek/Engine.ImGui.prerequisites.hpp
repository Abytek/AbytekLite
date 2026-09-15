#pragma once

#include "Abytek/Engine.ImGui.config.hpp"
#include "Abytek/Engine.Core.hpp"
#include "Abytek/Engine.RHI.hpp"
#include "Abytek/Engine.RenderCore.hpp"

#include "imgui.h"
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_WIN32
#include "imgui_impl_win32.h"
#endif
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
#include "imgui_impl_dx12.h"
#endif


namespace Abytek
{
}