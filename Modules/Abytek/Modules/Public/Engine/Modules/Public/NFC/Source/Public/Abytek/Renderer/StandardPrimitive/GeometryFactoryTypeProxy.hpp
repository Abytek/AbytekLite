#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxyManager.hpp"
#include "Abytek/Renderer/WorldRenderResourceChild.hpp"
#include "Abytek/Renderer/RenderScene.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API A_GeometryFactoryTypeProxy : public A_WorldRenderResourceChild
    {
    public:
        friend class F_GeometryFactoryTypeProxyManager;
        friend class A_GeometryFactoryType;
        
    private:
        F_GeometryFactoryTypeHashCode _HashCode = 0;
        F_GeometryFactoryTypeId _Id = INVALID_GEOMETRY_FACTORY_TYPE_ID;
        
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
        A_GeometryFactoryTypeProxy(const TW_Valid<A_GeometryFactoryType>& GeometryFactoryType);
        
    public:
        ~A_GeometryFactoryTypeProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        template<typename __F_GeometryFactoryType>
        static TW<A_GeometryFactoryTypeProxy> Find(const TW_Valid<A_RenderScene>& Scene)
        {
            return Scene->GetGeometryFactoryTypeProxyManager()->FindTypeProxy(
                A_GeometryFactoryType::GenerateHashCode<__F_GeometryFactoryType>()
            );
        }
        template<typename __F_GeometryFactoryType>
        static TW_Valid<A_GeometryFactoryTypeProxy> Get(const TW_Valid<A_RenderScene>& Scene)
        {
            return Scene->GetGeometryFactoryTypeProxyManager()->FindTypeProxy(
                A_GeometryFactoryType::GenerateHashCode<__F_GeometryFactoryType>()
            );
        }
    };
}
