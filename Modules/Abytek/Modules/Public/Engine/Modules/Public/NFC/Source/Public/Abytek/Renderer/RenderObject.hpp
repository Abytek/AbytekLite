#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"


namespace Abytek
{
    class F_WorldRenderResource;
    class A_RenderObjectFactory;
    class F_RenderRegistryRuntime;
    struct F_RenderCoreRHIConfig;
    
    class ABYTEK_ENGINE_NFC_API A_RenderObject : public A_Object
    {
    private:
        TW<F_WorldRenderResource> _WorldRenderResource;
        B8 _IsInitialized = false;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetWorldRenderResource() const noexcept
        {
            return _WorldRenderResource;
        }
        ABYTEK_FORCE_INLINE const auto& IsInitialized() const noexcept
        {
            return _IsInitialized;
        }
        
    protected:
        A_RenderObject(const TW_Valid<F_WorldRenderResource>& WorldRenderResource);
        
    public:
        ~A_RenderObject() override;
        
    public:
        const TS<A_RenderObjectFactory>& GetRenderObjectFactory() const noexcept;
        const TS<F_RenderRegistryRuntime>& GetRenderRegistryRuntime() const noexcept;
        
    public:
        const F_RenderCoreRHIConfig& GetRHIConfig() const noexcept;
        const F_RHIFeatureSupports& GetRHIFeatureSupports() const noexcept;
        
    protected:
        void InitMinimal(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        virtual void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}

#define ABYTEK_RENDER_OBJECT(Name, ...) \
            Name(const Abytek::TW_Valid<Abytek::F_WorldRenderResource>& WorldRenderResource) : __VA_ARGS__(WorldRenderResource) {} \
            ~Name() override = default

#ifdef ABYTEK_DEBUG_INFO
#define ABYTEK_INTERNAL_RENDER_OBJECT_ENABLE_IF_ENABLE_DEBUG_INFO(...) __VA_ARGS__
#define ABYTEK_INTERNAL_RENDER_OBJECT_ENABLE_IF_DISABLE_DEBUG_INFO(...)
#else
#define ABYTEK_INTERNAL_RENDER_OBJECT_ENABLE_IF_ENABLE_DEBUG_INFO(...)
#define ABYTEK_INTERNAL_RENDER_OBJECT_ENABLE_IF_DISABLE_DEBUG_INFO(...) __VA_ARGS__
#endif

#define ABYTEK_RENDER_OBJECT_CREATABLE(Name, ...) \
            ABYTEK_RENDER_OBJECT(Name, __VA_ARGS__); \
            static Abytek::TS<Name> Create( \
                const Abytek::TW_Valid<Abytek::F_WorldRenderResource>& WorldRenderResource \
            ) \
            { \
                return Abytek::TS<Name>()(WorldRenderResource); \
            } \
            template<typename... __F_Args> \
            static Abytek::TS<Name> CreateAndInit( \
                const Abytek::TW_Valid<Abytek::F_WorldRenderResource>& WorldRenderResource, \
                const Abytek::TS<Abytek::A_RHISubmissionItemContainer>& SubmissionItemContainer, \
                __F_Args&&... Args \
            ) \
            { \
                auto Result = Create(WorldRenderResource); \
                Result->Init(SubmissionItemContainer, ABYTEK_FORWARD(Args)...); \
                return Result; \
            } \
            ABYTEK_INTERNAL_RENDER_OBJECT_ENABLE_IF_ENABLE_DEBUG_INFO( \
                template<typename... __F_Args> \
                static Abytek::TS<Name> CreateAndInit_WithDebugName( \
                    const Abytek::F_Name& DebugName, \
                    const Abytek::TW_Valid<Abytek::F_WorldRenderResource>& WorldRenderResource, \
                    const Abytek::TS<Abytek::A_RHISubmissionItemContainer>& SubmissionItemContainer, \
                    __F_Args&&... Args \
                ) \
                { \
                    auto Result = Create(WorldRenderResource); \
                    Result->SetDebugName(DebugName); \
                    Result->Init(SubmissionItemContainer, ABYTEK_FORWARD(Args)...); \
                    return Result; \
                } \
            )