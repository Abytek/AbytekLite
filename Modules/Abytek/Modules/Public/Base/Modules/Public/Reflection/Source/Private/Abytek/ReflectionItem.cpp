#include "Abytek/ReflectionItem.hpp"
#include "Abytek/ReflectionContext.hpp"


namespace Abytek
{
    A_ReflectionItem::A_ReflectionItem(const F_GBMObjectConfig& GBMConfig, const F_ReflectionItemBuildParams& BuildParams) :
        A_GBMObject(GBMConfig),
        _Context(BuildParams.Context)
    {
        SetCanonical(BuildParams.Canonical);
    }
    A_ReflectionItem::~A_ReflectionItem()
    {
        SetCanonical({});
    }

    void A_ReflectionItem::SetCanonical(const F_Name& Canonical)
    {
        if (_Canonical)
        {
            _Context->_UnregisterCanonical(_Canonical);
            _Canonical = {};
        }
        if (Canonical)
        {
            _Context->_RegisterCanonical(Canonical, ABYTEK_WTHIS());
            _Canonical = Canonical;
        }
    }
}