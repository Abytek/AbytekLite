#include "Abytek/Renderer/StandardPrimitive/MaterialTargetTypeProxyManager.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetTypeProxy.hpp"


namespace Abytek
{
    void F_MaterialTargetTypeProxyManager::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_MaterialTargetTypeProxyManagerBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Scene = BuildParams.Scene;
    }
    void F_MaterialTargetTypeProxyManager::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _Scene = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }
    
    void F_MaterialTargetTypeProxyManager::_RegisterTypeProxy(const TW_Valid<A_MaterialTargetTypeProxy>& Type)
    {
        F_MaterialTargetTypeId Id = INVALID_GEOMETRY_FACTORY_TYPE_ID;
        if (!_FreeMaterialTargetTypeIds.empty())
        {
            Id = _FreeMaterialTargetTypeIds.back();
            _FreeMaterialTargetTypeIds.pop_back();
        }
        else
        {
            Id = _NextMaterialTargetTypeId;
            ++_NextMaterialTargetTypeId;
        }
        ABYTEK_ENGINE_NFC_ASSERT(Id != INVALID_GEOMETRY_FACTORY_TYPE_ID);
        Type->_Id = Id;
        _TypeProxies.push_back(Type);
    }
    void F_MaterialTargetTypeProxyManager::_UnregisterTypeProxy(const TW_Valid<A_MaterialTargetTypeProxy>& Type)
    {
        _TypeProxies.erase(
            std::find(
                _TypeProxies.begin(), 
                _TypeProxies.end(), 
                Type
            )  
        );
        _FreeMaterialTargetTypeIds.push_back(Type->_Id);
        Type->_Id = INVALID_GEOMETRY_FACTORY_TYPE_ID;
    }

    B8 F_MaterialTargetTypeProxyManager::HasTypeProxy(F_MaterialTargetTypeHashCode HashCode) const
    {
        return _HashCodeToTypeProxies.find(HashCode) != _HashCodeToTypeProxies.end();
    }
    TW<A_MaterialTargetTypeProxy> F_MaterialTargetTypeProxyManager::FindTypeProxy(F_MaterialTargetTypeHashCode HashCode) const
    {
        auto It = _HashCodeToTypeProxies.find(HashCode);
        if (It == _HashCodeToTypeProxies.end())
        {
            return {};
        }
        return It->second;
    }
    TW_Valid<A_MaterialTargetTypeProxy> F_MaterialTargetTypeProxyManager::GetTypeProxy(F_MaterialTargetTypeHashCode HashCode) const
    {
        auto It = _HashCodeToTypeProxies.find(HashCode);
        ABYTEK_ENGINE_NFC_ASSERT(It != _HashCodeToTypeProxies.end()) << "Not found geometry factory type proxy with hash code: " << HashCode;
        return It->second;
    }
}
