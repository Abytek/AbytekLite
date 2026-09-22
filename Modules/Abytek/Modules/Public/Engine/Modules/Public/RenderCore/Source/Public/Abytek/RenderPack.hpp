#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/RenderCoreHelper.hpp"
#include "Abytek/Cookable.hpp"
#include "Abytek/Renderable.hpp"


namespace Abytek
{
    class F_RenderRegistry;
    
    struct F_RenderPackBulkHeader
    {
        U64 PayloadOffsetInBytes = 0;
        U64 PayloadSizeInBytes = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RenderPackBulkHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RenderPackBulkHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    class ABYTEK_ENGINE_RENDER_CORE_API A_RenderPackTemplateMap
    {
    private:
        TF_Map<F_RHITemplateHashCode, TS<A_RHITemplate>> _Templates;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetTemplates() const noexcept
        {
            return _Templates;
        }
        ABYTEK_FORCE_INLINE auto& InjectTemplates() noexcept
        {
            return _Templates;
        }
        
    protected:
        A_RenderPackTemplateMap();
        
    public:
        virtual ~A_RenderPackTemplateMap();
        
    public:
        B8 HasTemplate(F_RHITemplateHashCode HashCode) const;
        TS<A_RHITemplate> FindTemplate(F_RHITemplateHashCode HashCode);
        TS<A_RHITemplate> GetTemplate(F_RHITemplateHashCode HashCode);
        void AddTemplate(const TS<A_RHITemplate>& Template);
        void RemoveTemplate(F_RHITemplateHashCode HashCode);
        void EnsureTemplate(const TS<A_RHITemplate>& Template);
        void RemoveUnusedTemplates();
        
    protected:
        virtual void OnAddTemplate(const TS<A_RHITemplate>& Template);
        virtual void OnRemoveTemplate(F_RHITemplateHashCode HashCode);
        virtual void OnModifyTemplates();
    };
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderPackTemplateMap : public A_Object, public A_RenderPackTemplateMap
    {
    public:
        F_RenderPackTemplateMap();
        ~F_RenderPackTemplateMap() override;
    };
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderPack : public A_WorldContext, public A_Renderable, public A_RenderPackTemplateMap, public I_Cookable
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_RenderPack);
    
    private:
        TS<F_RenderRegistry> _Registry;
        
        B8 _IsTemplatesLoaded = false;
        B8 _IsInitialTemplateAdding = false;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<F_RenderPackTemplateMap> _CookedTemplateMap;
#endif
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRegistry() const noexcept
        {
            return _Registry;
        }
        F_RHITemplateHashCode GetBaseDependencyHashCodeForTemplates() const noexcept
        {
            return H_RenderCore::GenerateBaseDependencyHashCodeForTemplates<F_RenderPack>(
                GetName().GetHashCode()
            );
        }
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        ABYTEK_FORCE_INLINE const auto& GetCookedTemplateMap() const noexcept
        {
            return _CookedTemplateMap;
        }
#endif
        
    public:
        F_RenderPack(const F_SerializableObjectInitParams& InitParams);
        ~F_RenderPack() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    protected:
        F_FeedbackStatus BinarySerialize(F_SerializableObjectBinarySerializeParams& Params) override;
        F_FeedbackStatus BinaryDeserialize(F_SerializableObjectBinaryDeserializeParams& Params) override;
        
    protected:
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
        TS<A_RenderProxy> CreateRenderProxy() override;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    public:
        virtual void PrepareTemplates(
            const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
        );
#endif
        
    protected:
        void OnAddTemplate(const TS<A_RHITemplate>& Template) override;
        void OnRemoveTemplate(F_RHITemplateHashCode HashCode) override;
        void OnModifyTemplates() override;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    protected:
        void PrepareForCooking() override;
        void Cook() override;
        void CleanUpAfterCooking() override;
#endif
    };
}