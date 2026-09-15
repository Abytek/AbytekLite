#pragma once

#include "Abytek/Engine.RHI.config.hpp"
#include "Abytek/Engine.Core.hpp"
#include "Abytek/Engine.Assets.hpp"
#include "Abytek/Engine.Window.hpp"

#ifdef ABYTEK_ENGINE_RHI_ENABLE_DXGI
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#endif

#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX11
    #include <d3d11.h>
#endif
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
    #include <directx/d3d12.h>
    #include <directx/d3dx12.h>
    #include <wrl.h>
    #include <dxcapi.h>
#endif

#ifdef ABYTEK_ENGINE_RHI_ENABLE_SLANG
#include "slang.h"
#include "slang-com-ptr.h"
#include "slang-com-helper.h"
#endif

#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
    #include "WinPixEventRuntime/pix3.h"
#endif

#ifdef ABYTEK_ENGINE_RHI_ENABLE_NVTX
    #include "nvtx3/nvtx3.hpp"
#endif


namespace Abytek
{
}