#include "Abytek/Renderer/VertexFactory/VertexFactoryTypeManager.hpp"
#include "Abytek/Renderer/VertexFactory/VertexFactoryTypeProxy.hpp"


namespace Abytek
{
    void F_VertexFactoryTypeManager::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_VertexFactoryTypeManagerBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Scene = BuildParams.Scene;
    }
    void F_VertexFactoryTypeManager::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _Scene = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }
    
    void F_VertexFactoryTypeManager::_RegisterType(const TW_Valid<A_VertexFactoryTypeProxy>& Type)
    {
        F_VertexFactoryTypeId Id = INVALID_VERTEX_FACTORY_TYPE_ID;
        if (!_FreeVertexFactoryTypeIds.empty())
        {
            Id = _FreeVertexFactoryTypeIds.back();
            _FreeVertexFactoryTypeIds.pop_back();
        }
        else
        {
            Id = _NextVertexFactoryTypeId;
            ++_NextVertexFactoryTypeId;
        }
        ABYTEK_ENGINE_NFC_ASSERT(Id != INVALID_VERTEX_FACTORY_TYPE_ID);
        Type->_Id = Id;
        _Types.push_back(Type);
    }
    void F_VertexFactoryTypeManager::_UnregisterType(const TW_Valid<A_VertexFactoryTypeProxy>& Type)
    {
        _Types.erase(
            std::find(
                _Types.begin(), 
                _Types.end(), 
                Type
            )  
        );
        _FreeVertexFactoryTypeIds.push_back(Type->_Id);
        Type->_Id = INVALID_VERTEX_FACTORY_TYPE_ID;
    }
}
