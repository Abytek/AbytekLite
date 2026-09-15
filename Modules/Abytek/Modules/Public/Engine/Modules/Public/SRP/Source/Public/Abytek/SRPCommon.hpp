#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"


namespace Abytek
{
    namespace SRP
    {
        inline B8 SupportSingleChannelVisibilityFormat(const F_RHIFeatureSupports& FeatureSupports)
        {
            return false;
        };
        inline E_RHIFormat GetVisibilityFormat(const F_RHIFeatureSupports& FeatureSupports)
        {
            return E_RHIFormat::R32G32_UINT;
        };
    }
}
