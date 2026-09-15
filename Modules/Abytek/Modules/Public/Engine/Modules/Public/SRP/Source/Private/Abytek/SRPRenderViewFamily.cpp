#include "Abytek/SRPRenderViewFamily.hpp"


namespace Abytek
{
    F_SRPRenderViewFamily::F_SRPRenderViewFamily(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderViewFamilyBuildParams& BuildParams) :
        A_RenderViewFamily(WorldRenderResource, BuildParams)
    {
    }
    F_SRPRenderViewFamily::~F_SRPRenderViewFamily()
    {
    }
}
