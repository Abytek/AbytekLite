#pragma once

#include "GPUDataStorage.hpp"
#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/GPUData/GPUDataComponentType.hpp"


namespace Abytek
{
    class A_RenderScene;
    class F_GPUDataComponentType;
    class F_GPUDataStorage;
    class F_GPUDataInstanceSet;

    struct F_GPUDataBuildParams
    {
        TW<A_RenderScene> Scene;
        F_Name Name;
        TF_Vector<F_GPUDataComponentTypeConfig> ComponentTypes;
        F_GlobalRenderBinding InstanceSetHeaderBinding;
    };
    class ABYTEK_ENGINE_NFC_API F_GPUData final : public A_RenderObject
    {
    public:
        friend class F_GPUDataInstanceSet;
        friend class F_GPUDataComponentType;
        
    public:
        static F_Name GetBindGroupSlotName_InstanceSetHeaderBuffer(
            const F_Name& DataName, 
            const F_RHIFeatureSupports& FeatureSupport
        )
        {
            return ABYTEK_TEXT("___Abytek_GPUDataInstanceSetHeaderBuffer___DATA___") + *DataName;
        }
        static F_Name GetBindGroupSlotName_InstanceSetHeadersUniformData(
            const F_Name& DataName, 
            const F_RHIFeatureSupports& FeatureSupport
        )
        {
            return ABYTEK_TEXT("___Abytek_GPUDataInstanceSetHeadersUniformData___DATA___") + *DataName;
        }
        
    private:
        TW<A_RenderScene> _Scene;
        F_Name _Name;
        TF_Vector<TS<F_GPUDataComponentType>> _ComponentTypes;
        F_GlobalRenderBinding _InstanceSetHeaderBinding;
        
        TF_Vector<U32> _ComponentIndexToSizeInBytes;
        TF_Vector<U32> _ComponentIndexToAlignmentInBytes;
        TF_Vector<E_GPUDataComponentTypeClass> _ComponentIndexToClass;
        
        F_YieldCriticalSection _CriticalSection;
        
        F_AtomicFlag _IsUpdatePhase;
        
        TS<F_GPUDataStorage> _Storage;
        TF_Vector<TW<F_GPUDataInstanceSet>> _InstanceSets;
        TF_Vector<F_GPUDataInstanceSetHeader> _InstanceSetHeaders;
        
        TF_Set<TS<F_GPUDataInstanceSet>> _DirtyInstanceSets;
        
        TS<A_RHIResource> _InstanceSetHeaderBuffer;
        TS<A_RHIResourceView> _InstanceSetHeaderSRV;
        TS<A_RHIBindGroup> _InstanceSetHeaderBindGroup;
        Sz _InstanceSetHeaderBufferCapacity = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE const auto& GetComponentTypes() const noexcept
        {
            return _ComponentTypes;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstanceSetHeaderBinding() const noexcept
        {
            return _InstanceSetHeaderBinding;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetComponentIndexToSizeInBytes() const noexcept
        {
            return _ComponentIndexToSizeInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetComponentIndexToAlignmentInBytes() const noexcept
        {
            return _ComponentIndexToAlignmentInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetComponentIndexToClass() const noexcept
        {
            return _ComponentIndexToClass;
        }
        
        ABYTEK_FORCE_INLINE auto IsUpdatePhase() const noexcept
        {
            return _IsUpdatePhase.test(boost::memory_order_acquire);
        }
        
        ABYTEK_FORCE_INLINE const auto& GetStorage() const noexcept
        {
            return _Storage;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstanceSets() const noexcept
        {
            return _InstanceSets;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstanceSetHeaders() const noexcept
        {
            return _InstanceSetHeaders;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetInstanceSetHeaderBuffer() const noexcept
        {
            return _InstanceSetHeaderBuffer;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstanceSetHeaderSRV() const noexcept
        {
            return _InstanceSetHeaderSRV;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstanceSetHeaderBindGroup() const noexcept
        {
            return _InstanceSetHeaderBindGroup;
        }
        ABYTEK_FORCE_INLINE auto GetInstanceSetHeaderBufferCapacity() const noexcept
        {
            return _InstanceSetHeaderBufferCapacity;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_GPUData, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_GPUDataBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void FinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    private:
        void _RegisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet);
        void _UnregisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet);
        
    private:
        void _AddDirtyInstanceSet(const TS<F_GPUDataInstanceSet>& InstanceSet);
        void _FlushDirtyInstanceSets(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        B8 HasComponentType(const F_Name& Name) const;
        U32 GetComponentTypeIndex(const F_Name& Name) const;
        U32 GetComponentTypeIndex(const TW_Valid<F_GPUDataComponentType>& ComponentType) const;
        TS<F_GPUDataComponentType> FindComponentType(const F_Name& Name) const;
        TS<F_GPUDataComponentType> GetComponentType(const F_Name& Name) const;
        TS<F_GPUDataComponentType> GetComponentType(U32 Index) const;
        
    public:
        template<typename __F>
        B8 HasComponentType() const
        {
            return HasComponentType(__F::GetStaticName());
        }
        template<typename __F>
        U32 GetComponentTypeIndex() const
        {
            return GetComponentTypeIndex(__F::GetStaticName());
        }
        template<typename __F>
        TS<F_GPUDataComponentType> FindComponentType() const
        {
            return FindComponentType(__F::GetStaticName());
        }
        template<typename __F>
        TS<F_GPUDataComponentType> GetComponentType() const
        {
            return GetComponentType(__F::GetStaticName());
        }
        
    private:
        void _UpdateInstanceSetHeaderBuffer(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
    
    namespace GPUData
    {
        struct ABYTEK_ALIGN(16) F_InstanceSetHeadersUniformData
        {
            U32 Num = 0;
        };
        template<typename __F_GPUData>
        struct ABYTEK_ENGINE_NFC_API TF_InstanceSetHeaderBinding : F_GlobalRenderBinding
        {
            static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
            ABYTEK_GLOBAL_RENDER_BINDING(
                TF_InstanceSetHeaderBinding, 
                ABYTEK_TEXT("Abytek::GPUData::TF_InstanceSetHeaderBinding<")
                + *__F_GPUData::GetStaticName()
                + ABYTEK_TEXT(">")
            );
        
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeResourceView(
                        F_GPUData::GetBindGroupSlotName_InstanceSetHeaderBuffer(
                            __F_GPUData::GetStaticName(),
                            Config.Database->GetFeatureSupports()
                        ),
                        F_RHIResourceAccess::MakeSRV()
                    ) 
                );
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeUniformData<F_InstanceSetHeadersUniformData>(
                        F_GPUData::GetBindGroupSlotName_InstanceSetHeadersUniformData(
                            __F_GPUData::GetStaticName(),
                            Config.Database->GetFeatureSupports()
                        )
                    ) 
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };

        namespace Internal
        {
            template<typename __F_GPUData>
            struct TH_ReflectGPUData
            {
                template<typename __F_GPUDataComponentType>
                static int One(const TW_Valid<F_ReflectionSession>& ReflectionSession, const TW_Valid<F_ReflectionType>& ReflectionType)
                {
                    ReflectionSession->ReferenceType(
                        ReflectionType->ReflectReferenced<TF_ComponentTypeSRVBinding<__F_GPUData, __F_GPUDataComponentType>>()
                    );
                    ReflectionSession->ReferenceType(
                        ReflectionType->ReflectReferenced<TF_ComponentTypeUAVBinding<__F_GPUData, __F_GPUDataComponentType>>()
                    );
                    return 0;
                }
                template<typename... __F_GPUDataComponentTypes>
                static void Invoke(const TW_Valid<F_ReflectionSession>& ReflectionSession, const TW_Valid<F_ReflectionType>& ReflectionType)
                {
                    ReflectionSession->ReferenceType(
                        ReflectionType->ReflectReferenced<TF_InstanceSetHeaderBinding<__F_GPUData>>()
                    );
                    int _[] = {
                        0,
                        One<__F_GPUDataComponentTypes>(ReflectionSession, ReflectionType)...
                    };
                }
            };
            template<typename __F_GPUData>
            struct TH_InitGPUData
            {
                template<typename... __F_GPUDataComponentTypes>
                static void Invoke(
                    const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
                    const TS<F_GPUData>& GPUData,
                    const TW_Valid<A_RenderScene>& Scene
                )
                {
                    F_GPUDataBuildParams BuildParams;
                    BuildParams.Scene = Scene;
                    BuildParams.Name = __F_GPUData::GetStaticName();
                    BuildParams.ComponentTypes = { 
                        F_GPUDataComponentTypeConfig::Make<__F_GPUData, __F_GPUDataComponentTypes>(
                            Scene->GetRenderRegistryRuntime()
                        )... 
                    };
                    BuildParams.InstanceSetHeaderBinding = static_cast<F_GlobalRenderBinding>(
                        TF_InstanceSetHeaderBinding<__F_GPUData>::Instantiate(Scene->GetRenderRegistryRuntime())
                    );
                    GPUData->Init(SubmissionItemContainer, BuildParams);
                }
            };

            template<typename __F_GPUData>
            static F_FeedbackStatus AddInstanceSetHeaderBindGroupToPipelineState(
                F_RHIPipelineStateTemplateCompileParams& PipelineStateTemplateCompileParams
            )
            {
                PipelineStateTemplateCompileParams.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        TF_InstanceSetHeaderBinding<__F_GPUData>::GetTemplateHashCode()
                    )
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
            
            template<typename __F_GPUData, typename __F_GPUDataComponentType>
            static TS<A_RHIBindGroup> GetComponentTypeBindGroup(
                const TS<F_GPUData>& GPUData,
                const F_RHIResourceAccess Access
            )
            {
                const auto& Storage = GPUData->GetStorage();
                auto ComponentTypeIndex = GPUData->GetComponentTypeIndex<__F_GPUDataComponentType>();
                if (FlagHas(Access.GPU, E_RHIResourceGPUAccess::SRV))
                {
                    return Storage->GetGlobalSRVBindGroup(ComponentTypeIndex);
                }
                if (FlagHas(Access.GPU, E_RHIResourceGPUAccess::UAV))
                {
                    return Storage->GetGlobalUAVBindGroup(ComponentTypeIndex);
                }
                return {};
            }
        }
    }
}

#define ABYTEK_GPU_DATA(Name, StaticName, Canonical, ...) \
             \
            static Abytek::F_Name GetStaticName() { return StaticName; } \
             \
            static void Init( \
                const Abytek::TS<Abytek::A_RHISubmissionItemContainer>& SubmissionItemContainer, \
                const Abytek::TS<Abytek::F_GPUData>& GPUData, \
                const Abytek::TW_Valid<Abytek::A_RenderScene>& Scene \
            ) \
            { \
                Abytek::GPUData::Internal::TH_InitGPUData<F_Reflected>::Invoke<__VA_ARGS__>( \
                    SubmissionItemContainer, \
                    GPUData, \
                    Scene \
                ); \
            } \
             \
            static Abytek::F_FeedbackStatus AddInstanceSetHeaderBindGroupToPipelineState( \
                Abytek::F_RHIPipelineStateTemplateCompileParams& PipelineStateTemplateCompileParams \
            ) \
            { \
                return Abytek::GPUData::Internal::AddInstanceSetHeaderBindGroupToPipelineState<Name>( \
                    PipelineStateTemplateCompileParams \
                ); \
            } \
             \
            ABYTEK_BEGIN_REFLECTOR() \
            ABYTEK_END_REFLECTOR(Name) \
            { \
                ABYTEK_REFLECT_CANONICAL(Canonical); \
                Abytek::GPUData::Internal::TH_ReflectGPUData<F_Reflected>::Invoke<__VA_ARGS__>( \
                    ReflectionSession, \
                    ReflectionType \
                ); \
            }
