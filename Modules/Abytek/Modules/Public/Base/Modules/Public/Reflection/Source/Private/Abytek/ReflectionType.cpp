#include "Abytek/ReflectionType.hpp"
#include "Abytek/ReflectionContext.hpp"


namespace Abytek
{
    F_ReflectionType::F_ReflectionType(const F_GBMObjectConfig& GBMConfig, const F_ReflectionTypeBuildParams& BuildParams) :
        A_ReflectionItem(GBMConfig, BuildParams),
        TypeFlags(BuildParams.TypeFlags),
        AdditionalInfo(BuildParams.AdditionalInfo),
        PrimitiveType(BuildParams.PrimitiveType),
        Desc(BuildParams.Desc),
        IsRightValueReference(BuildParams.IsRightValueReference)
    {
        GetContext()->_RegisterType(ABYTEK_WTHIS());
    }
    F_ReflectionType::~F_ReflectionType()
    {
        GetContext()->_UnregisterType(ABYTEK_WTHIS());
    }

    B8 F_ReflectionType::HasBase(const TW_Valid<F_ReflectionType>& X) const
    {
        for (const auto& Base : AdditionalInfo.Bases)
        {
            if (Base.Type == X)
            {
                return true;
            }
        }
        for (const auto& Base : AdditionalInfo.Bases)
        {
            if (Base.Type->HasBase(X))
            {
                return true;
            }
        }
        return false;
    }
    B8 F_ReflectionType::SupportImplicitPolymorphismCast(const TW_Valid<F_ReflectionType>& X) const
    {
        if (X == ABYTEK_WTHIS())
        {
            return true;
        }
        return HasBase(X);
    }
}