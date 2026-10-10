#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetType.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetTypeProxyManager.hpp"
#include "Abytek/Renderer/WorldRenderResourceChild.hpp"
#include "Abytek/Renderer/RenderScene.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API A_MaterialTargetTypeProxy : public A_WorldRenderResourceChild
    {
    public:
        friend class F_MaterialTargetTypeProxyManager;
        friend class A_MaterialTargetType;
        
    private:
        F_MaterialTargetTypeHashCode _HashCode = 0;
        F_MaterialTargetTypeId _Id = INVALID_GEOMETRY_FACTORY_TYPE_ID;
        
    public:
        ABYTEK_FORCE_INLINE auto GetHashCode() const noexcept
        {
            return _HashCode;
        }
        ABYTEK_FORCE_INLINE auto GetId() const noexcept
        {
            return _Id;
        }
        
    protected:
        A_MaterialTargetTypeProxy(const TW_Valid<A_MaterialTargetType>& MaterialTargetType);
        
    public:
        ~A_MaterialTargetTypeProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        template<typename __F_MaterialTargetType>
        static TW<A_MaterialTargetTypeProxy> Find(const TW_Valid<A_RenderScene>& Scene)
        {
            return Scene->GetMaterialTargetTypeProxyManager()->FindTypeProxy(
                A_MaterialTargetType::GenerateHashCode<__F_MaterialTargetType>()
            );
        }
        template<typename __F_MaterialTargetType>
        static TW_Valid<A_MaterialTargetTypeProxy> Get(const TW_Valid<A_RenderScene>& Scene)
        {
            return Scene->GetMaterialTargetTypeProxyManager()->FindTypeProxy(
                A_MaterialTargetType::GenerateHashCode<__F_MaterialTargetType>()
            );
        }
    };
}
