#include "Abytek/ReflectionAlias.hpp"
#include "Abytek/ReflectionContext.hpp"


namespace Abytek
{
    F_ReflectionAlias::F_ReflectionAlias(const F_GBMObjectConfig& GBMConfig, const F_ReflectionAliasBuildParams& BuildParams) :
        A_ReflectionItem(GBMConfig, BuildParams),
        _Key(BuildParams.Key),
        _Value(BuildParams.Value)
    {
        auto Target = GetContext()->SearchItem(_Value);
        ABYTEK_BASE_REFLECTION_ASSERT(Target) << "Invalid alias value: " << _Value;
        GetContext()->_RegisterAlias(ABYTEK_WTHIS());
        Target->GBMAddReference(ABYTEK_WTHIS());
    }
    F_ReflectionAlias::~F_ReflectionAlias()
    {
        GetContext()->_UnregisterAlias(ABYTEK_WTHIS());
    }
}
