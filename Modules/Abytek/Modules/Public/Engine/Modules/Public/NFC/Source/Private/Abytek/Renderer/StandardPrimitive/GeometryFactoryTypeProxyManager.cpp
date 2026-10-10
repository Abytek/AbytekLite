#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxyManager.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxy.hpp"


namespace Abytek
{
    void F_GeometryFactoryTypeProxyManager::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_GeometryFactoryTypeProxyManagerBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Scene = BuildParams.Scene;
    }
    void F_GeometryFactoryTypeProxyManager::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _Scene = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }
    
    void F_GeometryFactoryTypeProxyManager::_RegisterTypeProxy(const TW_Valid<A_GeometryFactoryTypeProxy>& Type)
    {
        F_GeometryFactoryTypeId Id = INVALID_GEOMETRY_FACTORY_TYPE_ID;
        if (!_FreeGeometryFactoryTypeIds.empty())
        {
            Id = _FreeGeometryFactoryTypeIds.back();
            _FreeGeometryFactoryTypeIds.pop_back();
        }
        else
        {
            Id = _NextGeometryFactoryTypeId;
            ++_NextGeometryFactoryTypeId;
        }
        ABYTEK_ENGINE_NFC_ASSERT(Id != INVALID_GEOMETRY_FACTORY_TYPE_ID);
        Type->_Id = Id;
        _TypeProxies.push_back(Type);
    }
    void F_GeometryFactoryTypeProxyManager::_UnregisterTypeProxy(const TW_Valid<A_GeometryFactoryTypeProxy>& Type)
    {
        _TypeProxies.erase(
            std::find(
                _TypeProxies.begin(), 
                _TypeProxies.end(), 
                Type
            )  
        );
        _FreeGeometryFactoryTypeIds.push_back(Type->_Id);
        Type->_Id = INVALID_GEOMETRY_FACTORY_TYPE_ID;
    }

    B8 F_GeometryFactoryTypeProxyManager::HasTypeProxy(F_GeometryFactoryTypeHashCode HashCode) const
    {
        return _HashCodeToTypeProxies.find(HashCode) != _HashCodeToTypeProxies.end();
    }
    TW<A_GeometryFactoryTypeProxy> F_GeometryFactoryTypeProxyManager::FindTypeProxy(F_GeometryFactoryTypeHashCode HashCode) const
    {
        auto It = _HashCodeToTypeProxies.find(HashCode);
        if (It == _HashCodeToTypeProxies.end())
        {
            return {};
        }
        return It->second;
    }
    TW_Valid<A_GeometryFactoryTypeProxy> F_GeometryFactoryTypeProxyManager::GetTypeProxy(F_GeometryFactoryTypeHashCode HashCode) const
    {
        auto It = _HashCodeToTypeProxies.find(HashCode);
        ABYTEK_ENGINE_NFC_ASSERT(It != _HashCodeToTypeProxies.end()) << "Not found geometry factory type proxy with hash code: " << HashCode;
        return It->second;
    }
}
