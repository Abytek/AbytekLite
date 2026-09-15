#pragma once

#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHICompileDataHandle.hpp"
#include "Abytek/DirectX12/RHIResourcePlacement.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class A_DirectX12RHIPassExtension;
    class A_RHIResourceProxy;
    struct F_DirectX12RHISubresource;
    struct F_DirectX12RHISubresourceBinding;
    struct F_DirectX12RHISubresourcePassTracking;
    struct F_DirectX12RHIResourceUse;
    
    namespace Internal::DirectX12RHIResource
    {
        extern ABYTEK_ENGINE_RHI_API D3D12_RESOURCE_STATES RHIResourceAccessToD3D12ResourceStates_Simple(
            const F_RHIResourceAccess& Access
        );
        extern ABYTEK_ENGINE_RHI_API D3D12_RESOURCE_STATES RHIResourceAccessToD3D12ResourceStates_Detailed(
            const F_RHIResourceAccess& Access
        );
        ABYTEK_FORCE_INLINE D3D12_RESOURCE_STATES RHIResourceAccessToInitialD3D12ResourceStates_Simple(
            const F_RHIResourceAccess& Access,
            const TF_Optional<F_RHIResourceAccess>& InitialAccessOpt
        )
        {
            if (InitialAccessOpt)
            {
                return RHIResourceAccessToD3D12ResourceStates_Simple(*InitialAccessOpt);
            }
            if (RHIHasCPUWriteAccess(Access.CPU))
            {
                return D3D12_RESOURCE_STATE_GENERIC_READ;
            }
            return D3D12_RESOURCE_STATE_COMMON;
        }

        ABYTEK_ENGINE_RHI_API D3D12_RESOURCE_FLAGS RHIResourceAccessToD3D12ResourceFlags(const F_RHIResourceAccess& Access);

        inline B8 IsWriteD3D12ResourceState(D3D12_RESOURCE_STATES A)
        {
            return (
                FlagHas(A, D3D12_RESOURCE_STATE_COPY_DEST)
                || FlagHas(A, D3D12_RESOURCE_STATE_RENDER_TARGET)
                || FlagHas(A, D3D12_RESOURCE_STATE_DEPTH_WRITE)
                || FlagHas(A, D3D12_RESOURCE_STATE_RESOLVE_DEST)
                || FlagHas(A, D3D12_RESOURCE_STATE_STREAM_OUT)
                || FlagHas(A, D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
                || FlagHas(A, D3D12_RESOURCE_STATE_VIDEO_ENCODE_WRITE)
                || FlagHas(A, D3D12_RESOURCE_STATE_VIDEO_PROCESS_WRITE)
            );
        }
        inline B8 CanCombineD3D12ResourceStates(D3D12_RESOURCE_STATES A, D3D12_RESOURCE_STATES B)
        {
            return !(
                IsWriteD3D12ResourceState(A)
                || IsWriteD3D12ResourceState(B)
            );
        }
    }
    
    struct F_DirectX12RHIResourceUseReference
    {
        TW<A_DirectX12RHIPassExtension> PassExtension;
        U32 Index = ~U32(0);
        
        static F_DirectX12RHIResourceUseReference Make(const TW_Valid<A_DirectX12RHIPassExtension>& InPassExtension, U32 InIndex)
        {
            F_DirectX12RHIResourceUseReference Result;
            Result.PassExtension = InPassExtension;
            Result.Index = InIndex;
            return Result;
        }
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return (
                PassExtension
                && (Index != ~U32(0))
            );
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        F_DirectX12RHIResourceUse& Get() const noexcept;
        F_DirectX12RHIResourceUse* operator -> () const noexcept;
        F_DirectX12RHIResourceUse& operator * () const noexcept;
        
        B8 operator == (const F_DirectX12RHIResourceUseReference& X) const noexcept
        {
            return (
                (PassExtension == X.PassExtension)    
                && (Index == X.Index)    
            );
        }
        B8 operator != (const F_DirectX12RHIResourceUseReference& X) const noexcept
        {
            return (
                (PassExtension != X.PassExtension)    
                || (Index != X.Index)    
            );
        }
    };
    struct F_DirectX12RHIResourceAliasingDependency : F_DirectX12RHIResourceUseReference
    {
    };
    struct F_DirectX12RHIResourcePassTracking : F_DirectX12RHIResourceUseReference
    {
    };
    
    struct F_DirectX12RHIResourcePassTrackingReference
    {
        TW<A_RHIResource> Resource;
        U32 Index = ~U32(0);
        
        static F_DirectX12RHIResourcePassTrackingReference Make(const TW_Valid<A_RHIResource>& InResource, U32 InIndex)
        {
            F_DirectX12RHIResourcePassTrackingReference Result;
            Result.Resource = InResource;
            Result.Index = InIndex;
            return Result;
        }
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return (
            Resource
                && (Index != ~U32(0))
            );
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        F_DirectX12RHIResourcePassTracking& Get() const noexcept;
        F_DirectX12RHIResourcePassTracking* operator -> () const noexcept;
        F_DirectX12RHIResourcePassTracking& operator * () const noexcept;
        
        B8 operator == (const F_DirectX12RHIResourcePassTrackingReference& X) const noexcept
        {
            return (
                (Resource == X.Resource)    
                && (Index == X.Index)    
            );
        }
        B8 operator != (const F_DirectX12RHIResourcePassTrackingReference& X) const noexcept
        {
            return (
                (Resource != X.Resource)    
                || (Index != X.Index)    
            );
        }
    };
    
    struct F_DirectX12RHIResourceUse
    {
        TW<A_RHIResource> Resource;
        
        static F_DirectX12RHIResourceUse MakeCore(const TW_Valid<A_RHIResource>& InResource)
        {
            F_DirectX12RHIResourceUse Result;
            Result.Resource = InResource;
            return Result;
        }
        
        F_RHIResourceAccess Access;
        E_RHIResourceAccessCategory AccessCategories = E_RHIResourceAccessCategory::NONE;
        
        F_DirectX12RHIResourcePassTrackingReference PassTrackingReference;
        F_DirectX12RHIResourceAliasingDependency AliasingDependency;
        
        // If true, add UAV barrier before
        B8 IsUAVSyncPoint = false;
    };
    using F_DirectX12RHIResourceUseSet = TF_SmallVector<F_DirectX12RHIResourceUse, 8>;
    
    struct ABYTEK_ENGINE_RHI_API F_DirectX12RHISubresourceReference
    {
        TW<A_RHIResource> Resource;
        U32 Index = 0;
        
        static F_DirectX12RHISubresourceReference Make(const TW_Valid<A_RHIResource>& InResource, U32 InIndex)
        {
            F_DirectX12RHISubresourceReference Result;
            Result.Resource = InResource;
            Result.Index = InIndex;
            return Result;
        }
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return (
                Resource
                && (Index != ~U32(0))
            );
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        F_DirectX12RHISubresource& Get() const noexcept;
        F_DirectX12RHISubresource* operator -> () const noexcept;
        F_DirectX12RHISubresource& operator * () const noexcept; 
        
        F_Text GetInfoText(U32 NumTabs = 0) const;
        
        B8 operator == (const F_DirectX12RHISubresourceReference& X) const noexcept
        {
            return (
                (Resource == X.Resource)    
                && (Index == X.Index)    
            );
        }
        B8 operator != (const F_DirectX12RHISubresourceReference& X) const noexcept
        {
            return (
                (Resource != X.Resource)    
                || (Index != X.Index)    
            );
        }
    };
    
    struct F_DirectX12RHISubresourceBindingReference
    {
        TW<A_DirectX12RHIPassExtension> PassExtension;
        U32 Index = ~U32(0);
        
        static F_DirectX12RHISubresourceBindingReference Make(const TW_Valid<A_DirectX12RHIPassExtension>& InPassExtension, U32 InIndex)
        {
            F_DirectX12RHISubresourceBindingReference Result;
            Result.PassExtension = InPassExtension;
            Result.Index = InIndex;
            return Result;
        }
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return (
                PassExtension
                && (Index != ~U32(0))
            );
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        F_DirectX12RHISubresourceBinding& Get() const noexcept;
        F_DirectX12RHISubresourceBinding* operator -> () const noexcept;
        F_DirectX12RHISubresourceBinding& operator * () const noexcept;
        
        B8 operator == (const F_DirectX12RHISubresourceBindingReference& X) const noexcept
        {
            return (
                (PassExtension == X.PassExtension)    
                && (Index == X.Index)    
            );
        }
        B8 operator != (const F_DirectX12RHISubresourceBindingReference& X) const noexcept
        {
            return (
                (PassExtension != X.PassExtension)    
                || (Index != X.Index)    
            );
        }
    };
    struct F_DirectX12RHISubresourceStateDependency : F_DirectX12RHISubresourceBindingReference
    {
    };
    struct F_DirectX12RHISubresourcePrologueStateDependency : F_DirectX12RHISubresourceBindingReference
    {
    };
    struct F_DirectX12RHISubresourceWriteDependency : F_DirectX12RHISubresourceBindingReference
    {
    };
    
    struct F_DirectX12RHISubresourcePassTrackingReference
    {
        F_DirectX12RHISubresourceReference SubresourceReference;
        U32 Index = ~U32(0);
        
        static F_DirectX12RHISubresourcePassTrackingReference Make(const F_DirectX12RHISubresourceReference& InSubresourceReference, U32 InIndex)
        {
            F_DirectX12RHISubresourcePassTrackingReference Result;
            Result.SubresourceReference = InSubresourceReference;
            Result.Index = InIndex;
            return Result;
        }
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return (
                SubresourceReference
                && (Index != ~U32(0))
            );
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        F_DirectX12RHISubresourcePassTracking& Get() const noexcept;
        F_DirectX12RHISubresourcePassTracking* operator -> () const noexcept;
        F_DirectX12RHISubresourcePassTracking& operator * () const noexcept;
        
        B8 operator == (const F_DirectX12RHISubresourcePassTrackingReference& X) const noexcept
        {
            return (
                (SubresourceReference == X.SubresourceReference)    
                && (Index == X.Index)    
            );
        }
        B8 operator != (const F_DirectX12RHISubresourcePassTrackingReference& X) const noexcept
        {
            return (
                (SubresourceReference != X.SubresourceReference)    
                || (Index != X.Index)    
            );
        }
    };
    struct F_DirectX12RHISubresourceReverseStateJoinDependency : F_DirectX12RHISubresourcePassTrackingReference
    {
    };
    struct F_DirectX12RHISubresourcePassTracking : F_DirectX12RHISubresourceBindingReference
    {
        F_DirectX12RHISubresourceReverseStateJoinDependency ReverseStateJoinDependency;
    };
    
    struct F_DirectX12RHISubresourceBinding
    {
        // Core data
        F_DirectX12RHISubresourceReference SubresourceReference;
        F_RHIResourceAccess Access;
        
        static F_DirectX12RHISubresourceBinding MakeCore(
            const F_DirectX12RHISubresourceReference& InSubresourceReference,
            const F_RHIResourceAccess& InAccess
        )
        {
            F_DirectX12RHISubresourceBinding Result;
            Result.SubresourceReference = InSubresourceReference;
            Result.Access = InAccess;
            return Result;
        }
        
        // Render graph lower level data
        E_RHIResourceAccessCategory AccessCategories = E_RHIResourceAccessCategory::NONE;
        D3D12_RESOURCE_STATES BaseD3D12State = D3D12_RESOURCE_STATE_COMMON;
        F_DirectX12RHISubresourcePassTrackingReference PassTrackingReference;
        F_DirectX12RHISubresourceStateDependency StateDependency;
        TF_SmallVector<F_DirectX12RHISubresourceWriteDependency, 1> WriteDependencies;
        F_DirectX12RHISubresourcePrologueStateDependency PrologueStateDependency;
        B8 NeedPrologueStateTransitionBarrier = false;
        
        // If true, add state transition barrier after
        B8 IsStateJoinPoint = false;
        
        F_DirectX12RHIResourceUseReference ResourceUseReference;
    };
    using F_DirectX12RHISubresourceBindingSet = TF_SmallVector<F_DirectX12RHISubresourceBinding, 6>;
    
    enum class E_DirectX12RHIResourceBarrierProxyType : U8
    {
        NONE,
        TRANSITION,
        UAV,
        ALIASING
    };
    struct F_DirectX12RHIResourceBarrierProxyTransition
    {
        TS<A_RHIResourceProxy> ResourceProxy;
        U32 SubresourceIndex = 0;
        D3D12_RESOURCE_STATES D3D12StateBefore = D3D12_RESOURCE_STATE_COMMON;
        D3D12_RESOURCE_STATES D3D12StateAfter = D3D12_RESOURCE_STATE_COMMON;
    };
    struct F_DirectX12RHIResourceBarrierProxyUAV
    {
        TS<A_RHIResourceProxy> ResourceProxy;
    };
    struct F_DirectX12RHIResourceBarrierProxyAliasing
    {
        TS<A_RHIResourceProxy> ResourceProxyBefore;
        TS<A_RHIResourceProxy> ResourceProxyAfter;
    };
    struct ABYTEK_ENGINE_RHI_API F_DirectX12RHIResourceBarrierProxy
    {
        E_DirectX12RHIResourceBarrierProxyType Type = E_DirectX12RHIResourceBarrierProxyType::NONE;
        F_DirectX12RHIResourceBarrierProxyTransition Transition;
        F_DirectX12RHIResourceBarrierProxyUAV UAV;
        F_DirectX12RHIResourceBarrierProxyAliasing Aliasing;
        
        D3D12_RESOURCE_BARRIER ConvertToD3D12ResourceBarrier() const noexcept;
    };
    using F_DirectX12RHIResourceBarrierProxySet = TF_SmallVector<F_DirectX12RHIResourceBarrierProxy, 4>;
    
    struct ABYTEK_ENGINE_RHI_API H_DirectX12RHIResourceBarrierProxy
    {
        static void AddResourceBarrier(
            const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& InD3D12CommandList,
            const F_DirectX12RHIResourceBarrierProxySet& BarrierProxies
        );
    };
    
    struct F_DirectX12RHISubresourceCompileData
    {
        U32 GlobalIndex = ~U32(0);
        U32 IndexInSection = ~U32(0);
        
        // Has original graph-based order
        TF_SmallVector<F_DirectX12RHISubresourcePassTracking, 16> PassTrackings;
        U32 CurrentSection_BeginPassTrackingIndex = 0;
        U32 CurrentSection_EndPassTrackingIndex = 0;
        
        TF_SmallVector<D3D12_RESOURCE_STATES, 18> D3D12States;
        
        static constexpr U32 PassTrackingIndexToD3D12StateIndex(U32 PassTrackingIndex)
        {
            return 1 + PassTrackingIndex;
        }
    };
    
    struct F_DirectX12RHIResourceAutoPlacedCompileData
    {
        U32 IndexInSection = ~U32(0);
        
        B8 NeedAllocate = false;
        B8 NeedDeallocate = false;
        
        U32 MinPassDependencyLevel = ~U32(0);
        U32 MaxPassDependencyLevel = ~U32(0);
        U32 AllocationLevel = ~U32(0);
        U32 DeallocationLevel = ~U32(0);
        
        // For aliasing dependency resolving
        F_DirectX12RHIResourcePlacement Placement;
        U32 EndDeallocationIndexToCheck = ~U32(0);
    };
    
    struct F_DirectX12RHIResourceCompileData
    {
        U32 GlobalIndex = ~U32(0);
        U32 IndexInSection = ~U32(0);
        
        // Has original graph-based order
        TF_SmallVector<F_DirectX12RHIResourcePassTracking, 16> PassTrackings;
        U32 CurrentSection_BeginPassTrackingIndex = 0;
        U32 CurrentSection_EndPassTrackingIndex = 0;
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        F_AtomicFlag IsUsedAsStatic;
#endif
        
        // 
        F_DirectX12RHIResourceAutoPlacedCompileData AutoPlacedData;
    };
    
    enum class E_RHISubresourceType : U8
    {
        NONE,
        BUFFER,
        TEXTURE
    };
    struct F_DirectX12RHISubresourceBuffer
    {
    };
    struct F_DirectX12RHISubresourceTexture
    {
        U32 ArraySlice = 0;
        U32 MipIndex = 0;
        U32 PlaneIndex = 0;
    };
    struct ABYTEK_ENGINE_RHI_API F_DirectX12RHISubresource
    {
        TW<A_RHIContext> Context;
        
        E_RHISubresourceType Type = E_RHISubresourceType::NONE;
        U32 Index = 0;
    
        D3D12_RESOURCE_STATES LastD3D12State = D3D12_RESOURCE_STATE_COMMON;
        
        F_DirectX12RHISubresourceBuffer Buffer;
        F_DirectX12RHISubresourceTexture Texture;
        
        F_Text GetInfoText(U32 NumTabs = 0) const;
        
        TF_RHICompileDataHandle<F_DirectX12RHISubresourceCompileData> CompileDataHandle;
        ABYTEK_FORCE_INLINE F_DirectX12RHISubresourceCompileData* GetCompileData() const
        {
            return CompileDataHandle.Get(Context->GetCurrentProcess());
        }
    };
    using F_DirectX12RHISubresourceSet = TF_SmallVector<F_DirectX12RHISubresource, 1>;
    
    struct ABYTEK_ENGINE_RHI_API H_DirectX12RHISubresourceSet
    {
        static F_DirectX12RHISubresourceSet MakeBuffer(
            const TW<A_RHIContext>& Context,
            E_RHIFormat Format,
            const F_RHIBufferAspect& BufferAspect,
            D3D12_RESOURCE_STATES InitialD3D12State
        );
        static F_DirectX12RHISubresourceSet MakeTexture(
            const TW<A_RHIContext>& Context,
            E_RHIFormat Format,
            const F_RHITextureAspect& TextureAspect,
            D3D12_RESOURCE_STATES InitialD3D12State
        );
    };
    
    struct ABYTEK_ENGINE_RHI_API H_DirectX12RHIResource
    {
        static D3D12_HEAP_TYPE GetD3D12HeapType(const F_RHIResourceAccess& AccessCapabilities);
        static D3D12_HEAP_PROPERTIES GetD3D12HeapProperties(const F_RHIResourceAccess& AccessCapabilities);
        static D3D12_RESOURCE_STATES GetInitialD3D12State(
            const F_RHIResourceAccess& AccessCapabilities,
            const TF_Optional<F_RHIResourceAccess>& InitialAccessOpt
        );
        static D3D12_RESOURCE_DESC GetD3D12ResourceDesc(
            const F_RHIResourceAccess& AccessCapabilities,
            E_RHIFormat Format,
            const F_RHIBufferAspect& BufferAspect
        );
        static D3D12_RESOURCE_DESC GetD3D12ResourceDesc(
            const F_RHIResourceAccess& AccessCapabilities,
            E_RHIFormat Format,
            const F_RHITextureAspect& TextureAspect
        );
        static TF_Optional<D3D12_CLEAR_VALUE> GetD3D12ClearValue(
            const F_RHIResourceAccess& AccessCapabilities,
            E_RHIFormat Format,
            const F_RHITextureAspect& TextureAspect
        );
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIResource : public A_RHIResource
    {
    private:
        F_DirectX12RHISubresourceSet _SubresourceSet;
        F_DirectX12RHIResourcePlacement _Placement;
        
    public:
        TF_RHICompileDataHandle<F_DirectX12RHIResourceCompileData> CompileDataHandle;
        ABYTEK_FORCE_INLINE F_DirectX12RHIResourceCompileData* GetCompileData() const
        {
            return CompileDataHandle.Get(GetContext()->GetCurrentProcess());
        }
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetSubresourceSet() const noexcept
        {
            return _SubresourceSet;
        }
        ABYTEK_FORCE_INLINE auto& InjectSubresourceSet() noexcept
        {
            return _SubresourceSet;
        }
        ABYTEK_FORCE_INLINE const auto& GetPlacement() const noexcept
        {
            return _Placement;
        }
        ABYTEK_FORCE_INLINE auto& InjectPlacement() noexcept
        {
            return _Placement;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIResource);
        virtual void Release() override;

    public:
        virtual void Build(const F_RHIBufferBuildParams& BuildParams) override;
        virtual void Build(const F_RHITextureBuildParams& BuildParams) override;
        virtual void Build(const F_RHIRTASBuildParams& BuildParams) override;

    protected:
        virtual void Build(
            const F_RHIResourceBuildParamsBase& BuildParamsBase,
            const F_RHIBufferAspect& BufferAspect
        ) override;
        virtual void Build(
            const F_RHIResourceBuildParamsBase& BuildParamsBase,
            const F_RHITextureAspect& TextureAspect
        ) override;
        virtual void Build(
            const F_RHIResourceBuildParamsBase& BuildParamsBase,
            const F_RHIRTASAspect& RTASAspect
        ) override;
        
    public:
        void BuildExternal(
            const F_RHIResourceBuildParamsBase& BuildParamsBase, 
            const F_RHIBufferAspect& BufferAspect, 
            const Microsoft::WRL::ComPtr<ID3D12Resource>& ExternalD3D12Resource
        );
        void BuildExternal(
            const F_RHIResourceBuildParamsBase& BuildParamsBase, 
            const F_RHITextureAspect& TextureAspect, 
            const Microsoft::WRL::ComPtr<ID3D12Resource>& ExternalD3D12Resource
        );
        void BuildExternal(
            const F_RHIResourceBuildParamsBase& BuildParamsBase, 
            const F_RHIRTASAspect& RTASAspect, 
            const Microsoft::WRL::ComPtr<ID3D12Resource>& ExternalD3D12Resource
            );
        
    public:
        void BuildWithoutD3D12Resource(
            const F_RHIResourceBuildParamsBase& BuildParamsBase, 
            const F_RHIBufferAspect& BufferAspect
        );
        void BuildWithoutD3D12Resource(
            const F_RHIResourceBuildParamsBase& BuildParamsBase, 
            const F_RHITextureAspect& TextureAspect
        );
        void BuildWithoutD3D12Resource(
            const F_RHIResourceBuildParamsBase& BuildParamsBase, 
            const F_RHIRTASAspect& RTASAspect
        );
        
    protected:
        virtual void Build(const F_RHIResourceBuildParamsBase& BuildParams);
        
    public:
        virtual void ReleaseAfterSubmit() override;
        
    public:
        virtual void EnableStaticAfterSubmit(const F_RHIResourceAccess& StaticAccess) override;
        virtual void DisableStatic() override;
        
    public:
        void D3D12EarlySetupBuffer();
        void D3D12EarlySetupTexture();
        void D3D12EarlySetupRTAS();

    public:
        void D3D12Upload(const F_RHIBufferDataView& DataView);
        void D3D12Upload(const F_RHITextureDataView& DataView);
        void D3D12Upload(const F_RHIRTASDataView& DataView);
        
    public:
        void D3D12AssignExternal(const Microsoft::WRL::ComPtr<ID3D12Resource>& ExternalD3D12Resource);
        void D3D12CreateCommitted();
        void D3D12CreateAutoPlaced();
        void D3D12CreatePlaced();
        
    public:
        void D3D12ReleaseAfterSubmit();
        void D3D12EnableStaticAfterSubmit();
        
    public:
        D3D12_HEAP_TYPE GetD3D12HeapType();
        D3D12_HEAP_PROPERTIES GetD3D12HeapProperties();
        D3D12_RESOURCE_STATES GetInitialD3D12State();
        D3D12_RESOURCE_DESC GetD3D12ResourceDesc();
        TF_Optional<D3D12_CLEAR_VALUE> GetD3D12ClearValue();
        D3D12_RESOURCE_ALLOCATION_INFO GetD3D12ResourceAllocationInfo();
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_DirectX12RHISubresourceReference>
    {
        size_t operator() (const Abytek::F_DirectX12RHISubresourceReference& A) const noexcept
        {
            size_t Result = 0;
            hash_combine(Result, A.Resource);
            hash_combine(Result, A.Index);
            return Result;
        }
    };
    template<>
    struct hash<Abytek::F_DirectX12RHISubresourceBindingReference>
    {
        size_t operator() (const Abytek::F_DirectX12RHISubresourceBindingReference& A) const noexcept
        {
            size_t Result = 0;
            hash_combine(Result, A.PassExtension);
            hash_combine(Result, A.Index);
            return Result;
        }
    };
    template<>
    struct hash<Abytek::F_DirectX12RHISubresourcePassTrackingReference>
    {
        size_t operator() (const Abytek::F_DirectX12RHISubresourcePassTrackingReference& A) const noexcept
        {
            size_t Result = 0;
            hash_combine(Result, A.SubresourceReference);
            hash_combine(Result, A.Index);
            return Result;
        }
    };
    template<>
    struct hash<Abytek::F_DirectX12RHIResourceUseReference>
    {
        size_t operator() (const Abytek::F_DirectX12RHIResourceUseReference& A) const noexcept
        {
            size_t Result = 0;
            hash_combine(Result, A.PassExtension);
            hash_combine(Result, A.Index);
            return Result;
        }
    };
    template<>
    struct hash<Abytek::F_DirectX12RHIResourcePassTrackingReference>
    {
        size_t operator() (const Abytek::F_DirectX12RHIResourcePassTrackingReference& A) const noexcept
        {
            size_t Result = 0;
            hash_combine(Result, A.Resource);
            hash_combine(Result, A.Index);
            return Result;
        }
    };
}
#endif