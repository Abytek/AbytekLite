#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/RHIResourceView.hpp"
#include "Abytek/RHIResourceViewProxy.hpp"
#include "Abytek/RHISampler.hpp"
#include "Abytek/RHISamplerProxy.hpp"


namespace Abytek
{
    class A_RHIBindGroupProxy;
    class A_RHIBindGroupTemplateRuntimeProxy;
    struct F_RHIBindGroupSlotDataProxy;
    
    using F_RHIBindGroupProxySet = TF_SmallVector<TS<A_RHIBindGroupProxy>, 4>;
    using F_RHIBindGroupProxySet_Weak = TF_SmallVector<TW<A_RHIBindGroupProxy>, 4>;

    using F_RHIResourceViewProxySet = TF_SmallVector<TS<A_RHIResourceViewProxy>, 8>;
    using F_RHISamplerProxySet = TF_SmallVector<TS<A_RHISamplerProxy>, 8>;

    struct F_RHIVertexBufferBindingProxy
    {
        TS<A_RHIResourceProxy> ResourceProxy;
        Sz OffsetInBytes = 0;
        Sz StrideInBytes = 0;
        Sz SizeInBytes = 0;

        ABYTEK_FORCE_INLINE bool IsValid() const noexcept
        {
            return (
                ResourceProxy
                && (StrideInBytes != 0)
                && (SizeInBytes != 0)
            );
        }
        ABYTEK_FORCE_INLINE explicit operator bool() const noexcept
        {
            return IsValid();
        }
        
        static F_RHIVertexBufferBindingProxy From(const F_RHIVertexBufferBinding& X)
        {
            F_RHIVertexBufferBindingProxy Result;
            Result.ResourceProxy = X.Resource->GetProxy().FastCast<A_RHIResourceProxy>();
            Result.OffsetInBytes = X.OffsetInBytes;
            Result.StrideInBytes = X.StrideInBytes;
            Result.SizeInBytes = X.SizeInBytes;
            return Result;
        }
    };
    struct F_RHIIndexBufferBindingProxy
    {
        TS<A_RHIResourceProxy> ResourceProxy; 
        Sz OffsetInBytes = 0;
        E_RHIFormat Format = E_RHIFormat::R32_UINT;
        Sz SizeInBytes = 0;

        ABYTEK_FORCE_INLINE bool IsValid() const noexcept
        {
            return (
                ResourceProxy
                && (Format != E_RHIFormat::NONE)
                && (SizeInBytes != 0)
            );
        }
        ABYTEK_FORCE_INLINE explicit operator bool() const noexcept
        {
            return IsValid();
        }
        
        static F_RHIIndexBufferBindingProxy From(const F_RHIIndexBufferBinding& X)
        {
            F_RHIIndexBufferBindingProxy Result;
            Result.ResourceProxy = X.Resource->GetProxy().FastCast<A_RHIResourceProxy>();
            Result.OffsetInBytes = X.OffsetInBytes;
            Result.Format = X.Format;
            Result.SizeInBytes = X.SizeInBytes;
            return Result;
        }
    };
    
    struct F_RHIBindGroupSlotResourceViewProxy
    {
        TS<A_RHIResourceViewProxy> ResourceViewProxy;
        
        static F_RHIBindGroupSlotResourceViewProxy From(const F_RHIBindGroupSlotResourceView& X)
        {
            F_RHIBindGroupSlotResourceViewProxy Result;
            Result.ResourceViewProxy = X.ResourceView->GetProxy().FastCast<A_RHIResourceViewProxy>();
            return Result;
        }
    };
    struct F_RHIBindGroupSlotResourceViewSetProxy
    {
        F_RHIResourceViewProxySet ResourceViewProxySet;
        
        static F_RHIBindGroupSlotResourceViewSetProxy From(const F_RHIBindGroupSlotResourceViewSet& X)
        {
            F_RHIBindGroupSlotResourceViewSetProxy Result;
            for (const auto& ResourceView : X.ResourceViewSet)
            {
                Result.ResourceViewProxySet.push_back(ResourceView->GetProxy().FastCast<A_RHIResourceViewProxy>());
            }
            return Result;
        }
    };
    struct F_RHIBindGroupSlotSamplerProxy
    {
        TS<A_RHISamplerProxy> SamplerProxy;
        
        static F_RHIBindGroupSlotSamplerProxy From(const F_RHIBindGroupSlotSampler& X)
        {
            F_RHIBindGroupSlotSamplerProxy Result;
            Result.SamplerProxy = X.Sampler->GetProxy().FastCast<A_RHISamplerProxy>();
            return Result;
        }
    };
    struct F_RHIBindGroupSlotSamplerSetProxy
    {
        F_RHISamplerProxySet SamplerProxySet;
        
        static F_RHIBindGroupSlotSamplerSetProxy From(const F_RHIBindGroupSlotSamplerSet& X)
        {
            F_RHIBindGroupSlotSamplerSetProxy Result;
            for (const auto& Sampler : X.SamplerSet)
            {
                Result.SamplerProxySet.push_back(Sampler->GetProxy().FastCast<A_RHISamplerProxy>());
            }
            return Result;
        }
    };
    struct F_RHIBindGroupSlotRTVProxy
    {
        TS<A_RHIResourceViewProxy> RTVProxy;
        
        static F_RHIBindGroupSlotRTVProxy From(const F_RHIBindGroupSlotRTV& X)
        {
            F_RHIBindGroupSlotRTVProxy Result;
            Result.RTVProxy = X.RTV->GetProxy().FastCast<A_RHIResourceViewProxy>();
            return Result;
        }
    };
    struct F_RHIBindGroupSlotDSVProxy
    {
        TS<A_RHIResourceViewProxy> DSVProxy;
        
        static F_RHIBindGroupSlotDSVProxy From(const F_RHIBindGroupSlotDSV& X)
        {
            F_RHIBindGroupSlotDSVProxy Result;
            Result.DSVProxy = X.DSV->GetProxy().FastCast<A_RHIResourceViewProxy>();
            return Result;
        }
    };
    struct F_RHIBindGroupSlotVertexBufferProxy
    {
        F_RHIVertexBufferBindingProxy VertexBufferBindingProxy;
        
        static F_RHIBindGroupSlotVertexBufferProxy From(const F_RHIBindGroupSlotVertexBuffer& X)
        {
            F_RHIBindGroupSlotVertexBufferProxy Result;
            Result.VertexBufferBindingProxy = F_RHIVertexBufferBindingProxy::From(X.VertexBufferBinding);
            return Result;
        }
    };
    struct F_RHIBindGroupSlotIndexBufferProxy
    {
        F_RHIIndexBufferBindingProxy IndexBufferBindingProxy;
        
        static F_RHIBindGroupSlotIndexBufferProxy From(const F_RHIBindGroupSlotIndexBuffer& X)
        {
            F_RHIBindGroupSlotIndexBufferProxy Result;
            Result.IndexBufferBindingProxy = F_RHIIndexBufferBindingProxy::From(X.IndexBufferBinding);
            return Result;
        }
    };
    struct F_RHIBindGroupSlotUniformDataProxy
    {
        U16 SizeInBytes = 0;
        void* GetPayloadPtr() const noexcept
        {
            return (void*)(this + 1);
        }
    };
    
    struct F_RHIBindGroupSlotProxyInfo
    {
        E_RHIBindGroupSlotType Type = E_RHIBindGroupSlotType::NONE;
        U16 BinaryOffset = 0;
        
        F_RHIBindGroupSlotResourceViewProxy& ResolveResourceViewProxy(const F_RHIBindGroupSlotDataProxy& Data) const;
        F_RHIBindGroupSlotResourceViewSetProxy& ResolveResourceViewSetProxy(const F_RHIBindGroupSlotDataProxy& Data) const;
        
        F_RHIBindGroupSlotSamplerProxy& ResolveSamplerProxy(const F_RHIBindGroupSlotDataProxy& Data) const;
        F_RHIBindGroupSlotSamplerSetProxy& ResolveSamplerSetProxy(const F_RHIBindGroupSlotDataProxy& Data) const;
        
        F_RHIBindGroupSlotRTVProxy& ResolveRTVProxy(const F_RHIBindGroupSlotDataProxy& Data) const;
        F_RHIBindGroupSlotDSVProxy& ResolveDSVProxy(const F_RHIBindGroupSlotDataProxy& Data) const;
        
        F_RHIBindGroupSlotVertexBufferProxy& ResolveVertexBufferProxy(const F_RHIBindGroupSlotDataProxy& Data) const;
        F_RHIBindGroupSlotIndexBufferProxy& ResolveIndexBufferProxy(const F_RHIBindGroupSlotDataProxy& Data) const;
        
        F_RHIBindGroupSlotUniformDataProxy& ResolveUniformDataProxy(const F_RHIBindGroupSlotDataProxy& Data) const;
    };
    struct F_RHIBindGroupSlotDataProxy
    { 
        TF_SmallVector<F_RHIBindGroupSlotProxyInfo, 8> SlotInfos;
        TF_SmallVector<U8, 256> Binary;
        
        F_RHIBindGroupSlotDataProxy() noexcept = default;
        ~F_RHIBindGroupSlotDataProxy() noexcept
        {
            Destruct();
        }
        
        F_RHIBindGroupSlotDataProxy(const F_RHIBindGroupSlotDataProxy& X) noexcept
        {
            CopyFrom(X);
        }
        F_RHIBindGroupSlotDataProxy& operator = (const F_RHIBindGroupSlotDataProxy& X) noexcept 
        {
            if (this != &X)
            {
                CopyFrom(X);
            }
            return *this;
        }
        
        F_RHIBindGroupSlotDataProxy(const F_RHIBindGroupSlotData& X) noexcept
        {
            CopyFrom(X);
        }
        F_RHIBindGroupSlotDataProxy& operator = (const F_RHIBindGroupSlotData& X) noexcept 
        {
            CopyFrom(X);
            return *this;
        }
        
        F_RHIBindGroupSlotDataProxy(F_RHIBindGroupSlotDataProxy&& X) noexcept
        {
            MoveFrom(ABYTEK_MOVE(X));
        }
        F_RHIBindGroupSlotDataProxy& operator = (F_RHIBindGroupSlotDataProxy&& X) noexcept
        {
            if (this != &X)
            {
                MoveFrom(ABYTEK_MOVE(X));
            }
            return *this;
        }
        
        void CopyFrom(const F_RHIBindGroupSlotData& X)
        {
            Destruct();
            
            U32 NumSlots = X.SlotInfos.size();
            SlotInfos.reserve(NumSlots);
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                const auto& SlotInfo = X.SlotInfos[SlotIndex];
                switch (SlotInfo.Type)
                {
                case E_RHIBindGroupSlotType::RESOURCE_VIEW:
                    AddResourceView() = F_RHIBindGroupSlotResourceViewProxy::From(
                        X.GetResourceView(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    AddResourceViewSet() = F_RHIBindGroupSlotResourceViewSetProxy::From(
                        X.GetResourceViewSet(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::SAMPLER:
                    AddSampler() = F_RHIBindGroupSlotSamplerProxy::From(
                        X.GetSampler(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::SAMPLER_SET:
                    AddSamplerSet() = F_RHIBindGroupSlotSamplerSetProxy::From(
                        X.GetSamplerSet(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::RTV:
                    AddRTV() = F_RHIBindGroupSlotRTVProxy::From(
                        X.GetRTV(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::DSV:
                    AddDSV() = F_RHIBindGroupSlotDSVProxy::From(
                        X.GetDSV(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                    AddVertexBuffer() = F_RHIBindGroupSlotVertexBufferProxy::From(
                        X.GetVertexBuffer(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::INDEX_BUFFER:
                    AddIndexBuffer() = F_RHIBindGroupSlotIndexBufferProxy::From(
                        X.GetIndexBuffer(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::UNIFORM_DATA:
                    {
                        auto& SrcUniformData = X.GetUniformData(SlotIndex);
                        auto& UniformData = AddUniformData(SrcUniformData.SizeInBytes);
                        UniformData.SizeInBytes = SrcUniformData.SizeInBytes;
                        memcpy(
                            UniformData.GetPayloadPtr(),
                            SrcUniformData.GetPayloadPtr(),
                            SrcUniformData.SizeInBytes
                        );
                    }
                    break;
                    
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid slot type";
                    break;
                }
            }
        }
        
        void CopyFrom(const F_RHIBindGroupSlotDataProxy& X)
        {
            Destruct();
            
            Binary.resize(X.Binary.size());
            
            U32 NumSlots = X.SlotInfos.size();
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                const auto& SlotInfo = X.SlotInfos[SlotIndex];
                switch (SlotInfo.Type)
                {
                case E_RHIBindGroupSlotType::RESOURCE_VIEW:
                    new (&SlotInfo.ResolveResourceViewProxy(*this)) F_RHIBindGroupSlotResourceViewProxy(
                        X.GetResourceView(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    new (&SlotInfo.ResolveResourceViewSetProxy(*this)) F_RHIBindGroupSlotResourceViewSetProxy(
                        X.GetResourceViewSet(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::SAMPLER:
                    new (&SlotInfo.ResolveSamplerProxy(*this)) F_RHIBindGroupSlotSamplerProxy(
                        X.GetSampler(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::SAMPLER_SET:
                    new (&SlotInfo.ResolveSamplerSetProxy(*this)) F_RHIBindGroupSlotSamplerSetProxy(
                        X.GetSamplerSet(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::RTV:
                    new (&SlotInfo.ResolveRTVProxy(*this)) F_RHIBindGroupSlotRTVProxy(
                        X.GetRTV(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::DSV:
                    new (&SlotInfo.ResolveDSVProxy(*this)) F_RHIBindGroupSlotDSVProxy(
                        X.GetDSV(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                    new (&SlotInfo.ResolveVertexBufferProxy(*this)) F_RHIBindGroupSlotVertexBufferProxy(
                        X.GetVertexBuffer(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::INDEX_BUFFER:
                    new (&SlotInfo.ResolveIndexBufferProxy(*this)) F_RHIBindGroupSlotIndexBufferProxy(
                        X.GetIndexBuffer(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::UNIFORM_DATA:
                    {
                        auto& SrcUniformData = X.GetUniformData(SlotIndex);
                        auto& UniformData = *(
                            new (&SlotInfo.ResolveUniformDataProxy(*this)) F_RHIBindGroupSlotUniformDataProxy(SrcUniformData)
                        );
                        memcpy(
                            UniformData.GetPayloadPtr(),
                            SrcUniformData.GetPayloadPtr(),
                            SrcUniformData.SizeInBytes
                        );
                    }
                    break;
                    
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid slot type";
                    break;
                }
            }
            SlotInfos = X.SlotInfos;
        }
        void MoveFrom(F_RHIBindGroupSlotDataProxy&& X)
        {
            Destruct();
            
            Binary.resize(X.Binary.size());
            
            U32 NumSlots = X.SlotInfos.size();
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                const auto& SlotInfo = X.SlotInfos[SlotIndex];
                switch (SlotInfo.Type)
                {
                case E_RHIBindGroupSlotType::RESOURCE_VIEW:
                    new (&SlotInfo.ResolveResourceViewProxy(*this)) F_RHIBindGroupSlotResourceViewProxy(
                        ABYTEK_MOVE(X.GetResourceView(SlotIndex))
                    );
                    break;
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    new (&SlotInfo.ResolveResourceViewSetProxy(*this)) F_RHIBindGroupSlotResourceViewSetProxy(
                        ABYTEK_MOVE(X.GetResourceViewSet(SlotIndex))
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::SAMPLER:
                    new (&SlotInfo.ResolveSamplerProxy(*this)) F_RHIBindGroupSlotSamplerProxy(
                        ABYTEK_MOVE(X.GetSampler(SlotIndex))
                    );
                    break;
                case E_RHIBindGroupSlotType::SAMPLER_SET:
                    new (&SlotInfo.ResolveSamplerSetProxy(*this)) F_RHIBindGroupSlotSamplerSetProxy(
                        ABYTEK_MOVE(X.GetSamplerSet(SlotIndex))
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::RTV:
                    new (&SlotInfo.ResolveRTVProxy(*this)) F_RHIBindGroupSlotRTVProxy(
                        ABYTEK_MOVE(X.GetRTV(SlotIndex))
                    );
                    break;
                case E_RHIBindGroupSlotType::DSV:
                    new (&SlotInfo.ResolveDSVProxy(*this)) F_RHIBindGroupSlotDSVProxy(
                        ABYTEK_MOVE(X.GetDSV(SlotIndex))
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                    new (&SlotInfo.ResolveVertexBufferProxy(*this)) F_RHIBindGroupSlotVertexBufferProxy(
                        ABYTEK_MOVE(X.GetVertexBuffer(SlotIndex))
                    );
                    break;
                case E_RHIBindGroupSlotType::INDEX_BUFFER:
                    new (&SlotInfo.ResolveIndexBufferProxy(*this)) F_RHIBindGroupSlotIndexBufferProxy(
                        ABYTEK_MOVE(X.GetIndexBuffer(SlotIndex))
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::UNIFORM_DATA:
                    {
                        auto& SrcUniformData = X.GetUniformData(SlotIndex);
                        auto& UniformData = *(
                            new (&SlotInfo.ResolveUniformDataProxy(*this)) F_RHIBindGroupSlotUniformDataProxy(SrcUniformData)
                        );
                        memcpy(
                            UniformData.GetPayloadPtr(),
                            SrcUniformData.GetPayloadPtr(),
                            SrcUniformData.SizeInBytes
                        );
                    }
                    break;
                    
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid slot type";
                    break;
                }
            }
            X.DestructDataOnlyUnsafe();
            SlotInfos = ABYTEK_MOVE(X.SlotInfos);
        }
        
        void Destruct()
        {
            DestructDataOnlyUnsafe();
            SlotInfos.clear();
        }
        void DestructDataOnlyUnsafe()
        {
            U32 NumSlots = SlotInfos.size();
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                const auto& SlotInfo = SlotInfos[SlotIndex];
                switch (SlotInfo.Type)
                {
                case E_RHIBindGroupSlotType::RESOURCE_VIEW:
                    GetResourceView(SlotIndex).~F_RHIBindGroupSlotResourceViewProxy();
                    break;
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    GetResourceViewSet(SlotIndex).~F_RHIBindGroupSlotResourceViewSetProxy();
                    break;
                
                case E_RHIBindGroupSlotType::SAMPLER:
                    GetSampler(SlotIndex).~F_RHIBindGroupSlotSamplerProxy();
                    break;
                case E_RHIBindGroupSlotType::SAMPLER_SET:
                    GetSamplerSet(SlotIndex).~F_RHIBindGroupSlotSamplerSetProxy();
                    break;
                
                case E_RHIBindGroupSlotType::RTV:
                    GetRTV(SlotIndex).~F_RHIBindGroupSlotRTVProxy();
                    break;
                case E_RHIBindGroupSlotType::DSV:
                    GetDSV(SlotIndex).~F_RHIBindGroupSlotDSVProxy();
                    break;
                
                case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                    GetVertexBuffer(SlotIndex).~F_RHIBindGroupSlotVertexBufferProxy();
                    break;
                case E_RHIBindGroupSlotType::INDEX_BUFFER:
                    GetIndexBuffer(SlotIndex).~F_RHIBindGroupSlotIndexBufferProxy();
                    break;
                
                case E_RHIBindGroupSlotType::UNIFORM_DATA:
                    break;
                
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid slot type";
                    break;
                }
            }
            Binary.clear();
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceViewProxy& AddResourceView()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotResourceViewProxy)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotResourceViewProxy));
            auto Data = (F_RHIBindGroupSlotResourceViewProxy*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotResourceViewProxy();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::RESOURCE_VIEW, BinaryOffset });
            return *Data;
        }
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceViewSetProxy& AddResourceViewSet()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotResourceViewSetProxy)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotResourceViewSetProxy));
            auto Data = (F_RHIBindGroupSlotResourceViewSetProxy*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotResourceViewSetProxy();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::RESOURCE_VIEW_SET, BinaryOffset });
            return *Data;
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSamplerProxy& AddSampler()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotSamplerProxy)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotSamplerProxy));
            auto Data = (F_RHIBindGroupSlotSamplerProxy*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotSamplerProxy();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::SAMPLER, BinaryOffset });
            return *Data;
        }
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSamplerSetProxy& AddSamplerSet()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotSamplerSetProxy)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotSamplerSetProxy));
            auto Data = (F_RHIBindGroupSlotSamplerSetProxy*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotSamplerSetProxy();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::SAMPLER_SET, BinaryOffset });
            return *Data;
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotRTVProxy& AddRTV()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotRTVProxy)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotRTVProxy));
            auto Data = (F_RHIBindGroupSlotRTVProxy*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotRTVProxy();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::RTV, BinaryOffset });
            return *Data;
        }
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotDSVProxy& AddDSV()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotDSVProxy)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotDSVProxy));
            auto Data = (F_RHIBindGroupSlotDSVProxy*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotDSVProxy();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::DSV, BinaryOffset });
            return *Data;
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotVertexBufferProxy& AddVertexBuffer()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotVertexBufferProxy)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotVertexBufferProxy));
            auto Data = (F_RHIBindGroupSlotVertexBufferProxy*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotVertexBufferProxy();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::VERTEX_BUFFER, BinaryOffset });
            return *Data;
        }
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotIndexBufferProxy& AddIndexBuffer()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotIndexBufferProxy)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotIndexBufferProxy));
            auto Data = (F_RHIBindGroupSlotIndexBufferProxy*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotIndexBufferProxy();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::INDEX_BUFFER, BinaryOffset });
            return *Data;
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotUniformDataProxy& AddUniformData(U16 SizeInBytes)
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotUniformDataProxy)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotUniformDataProxy) + SizeInBytes);
            auto Data = (F_RHIBindGroupSlotUniformDataProxy*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotUniformDataProxy { SizeInBytes };
            SlotInfos.push_back({ E_RHIBindGroupSlotType::UNIFORM_DATA, BinaryOffset });
            return *Data;
        }
        
        ABYTEK_FORCE_INLINE auto GetSize() const noexcept
        {
            return SlotInfos.size();
        }
        ABYTEK_FORCE_INLINE auto GetType(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].Type;
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceViewProxy& GetResourceView(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveResourceViewProxy(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotResourceViewProxy& GetResourceView(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveResourceViewProxy(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceViewSetProxy& GetResourceViewSet(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveResourceViewSetProxy(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotResourceViewSetProxy& GetResourceViewSet(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveResourceViewSetProxy(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSamplerProxy& GetSampler(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveSamplerProxy(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotSamplerProxy& GetSampler(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveSamplerProxy(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSamplerSetProxy& GetSamplerSet(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveSamplerSetProxy(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotSamplerSetProxy& GetSamplerSet(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveSamplerSetProxy(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotRTVProxy& GetRTV(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveRTVProxy(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotRTVProxy& GetRTV(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveRTVProxy(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotDSVProxy& GetDSV(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveDSVProxy(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotDSVProxy& GetDSV(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveDSVProxy(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotVertexBufferProxy& GetVertexBuffer(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveVertexBufferProxy(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotVertexBufferProxy& GetVertexBuffer(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveVertexBufferProxy(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotIndexBufferProxy& GetIndexBuffer(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveIndexBufferProxy(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotIndexBufferProxy& GetIndexBuffer(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveIndexBufferProxy(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotUniformDataProxy& GetUniformData(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveUniformDataProxy(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotUniformDataProxy& GetUniformData(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveUniformDataProxy(*this);
        }
    };
    
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceViewProxy& F_RHIBindGroupSlotProxyInfo::ResolveResourceViewProxy(const F_RHIBindGroupSlotDataProxy& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::RESOURCE_VIEW) << "Invalid type";
        return *(F_RHIBindGroupSlotResourceViewProxy*)&Data.Binary[BinaryOffset];
    }
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceViewSetProxy& F_RHIBindGroupSlotProxyInfo::ResolveResourceViewSetProxy(const F_RHIBindGroupSlotDataProxy& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::RESOURCE_VIEW_SET) << "Invalid type";
        return *(F_RHIBindGroupSlotResourceViewSetProxy*)&Data.Binary[BinaryOffset];
    }
        
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSamplerProxy& F_RHIBindGroupSlotProxyInfo::ResolveSamplerProxy(const F_RHIBindGroupSlotDataProxy& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::SAMPLER) << "Invalid type";
        return *(F_RHIBindGroupSlotSamplerProxy*)&Data.Binary[BinaryOffset];
    }
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSamplerSetProxy& F_RHIBindGroupSlotProxyInfo::ResolveSamplerSetProxy(const F_RHIBindGroupSlotDataProxy& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::SAMPLER_SET) << "Invalid type";
        return *(F_RHIBindGroupSlotSamplerSetProxy*)&Data.Binary[BinaryOffset];
    }
        
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotRTVProxy& F_RHIBindGroupSlotProxyInfo::ResolveRTVProxy(const F_RHIBindGroupSlotDataProxy& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::RTV) << "Invalid type";
        return *(F_RHIBindGroupSlotRTVProxy*)&Data.Binary[BinaryOffset];
    }
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotDSVProxy& F_RHIBindGroupSlotProxyInfo::ResolveDSVProxy(const F_RHIBindGroupSlotDataProxy& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::DSV) << "Invalid type";
        return *(F_RHIBindGroupSlotDSVProxy*)&Data.Binary[BinaryOffset];
    }
        
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotVertexBufferProxy& F_RHIBindGroupSlotProxyInfo::ResolveVertexBufferProxy(const F_RHIBindGroupSlotDataProxy& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::VERTEX_BUFFER) << "Invalid type";
        return *(F_RHIBindGroupSlotVertexBufferProxy*)&Data.Binary[BinaryOffset];
    }
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotIndexBufferProxy& F_RHIBindGroupSlotProxyInfo::ResolveIndexBufferProxy(const F_RHIBindGroupSlotDataProxy& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::INDEX_BUFFER) << "Invalid type";
        return *(F_RHIBindGroupSlotIndexBufferProxy*)&Data.Binary[BinaryOffset];
    }
        
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotUniformDataProxy& F_RHIBindGroupSlotProxyInfo::ResolveUniformDataProxy(const F_RHIBindGroupSlotDataProxy& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::UNIFORM_DATA) << "Invalid type";
        return *(F_RHIBindGroupSlotUniformDataProxy*)&Data.Binary[BinaryOffset];
    }
    
    class ABYTEK_ENGINE_RHI_API A_RHIBindGroupProxy :
        public A_RHIContextChildProxy
    {
    private:
        TS<A_RHIBindGroupTemplateRuntimeProxy> _TemplateRuntimeProxy;
        
    public:
        ABYTEK_FORCE_INLINE auto GetBindGroup() const noexcept
        {
            return GetContextChild().FastCast<A_RHIBindGroup>();
        }
        
        ABYTEK_FORCE_INLINE const auto& GetTemplateRuntimeProxy() const noexcept
        {
            return _TemplateRuntimeProxy;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIBindGroupProxy);
        void LateBuildBindGroup();
        virtual void Release() override;
    };
}
