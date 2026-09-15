#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIResourceAccess.hpp"
#include "Abytek/RHIFormat.hpp"
#include "Abytek/RHIBindless.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIResourceView.hpp"
#include "Abytek/RHISampler.hpp"


namespace Abytek
{
    class A_RHIResource;
    class A_RHIResourceView;
    class A_RHISampler;
    class A_RHIBindGroup;
    class A_RHIBindGroupTemplateRuntime;
    
    using F_RHIBindGroupSet = TF_SmallVector<TS<A_RHIBindGroup>, 4>;
    using F_RHIBindGroupSet_Weak = TF_SmallVector<TW<A_RHIBindGroup>, 4>;

    struct F_RHIVertexBufferBinding
    {
        TS<A_RHIResource> Resource;
        Sz OffsetInBytes = 0;
        Sz StrideInBytes = 0;
        Sz SizeInBytes = 0;

        ABYTEK_FORCE_INLINE bool IsValid() const noexcept
        {
            return (
                Resource
                && (StrideInBytes != 0)
                && (SizeInBytes != 0)
            );
        }
        ABYTEK_FORCE_INLINE explicit operator bool() const noexcept
        {
            return IsValid();
        }
    };
    struct F_RHIIndexBufferBinding
    {
        TS<A_RHIResource> Resource; 
        Sz OffsetInBytes = 0;
        E_RHIFormat Format = E_RHIFormat::R32_UINT;
        Sz SizeInBytes = 0;

        ABYTEK_FORCE_INLINE bool IsValid() const noexcept
        {
            return (
                Resource
                && (Format != E_RHIFormat::NONE)
                && (SizeInBytes != 0)
            );
        }
        ABYTEK_FORCE_INLINE explicit operator bool() const noexcept
        {
            return IsValid();
        }
    };

    enum class E_RHIVertexInputElementClass : U8
    {
        NONE,
        PER_VERTEX_DATA,
        PER_INSTANCE_DATA,
        DEFAULT = PER_VERTEX_DATA
    };
    struct F_RHIVertexInputElement
    {
        F_Name Name;
        E_RHIFormat Format = E_RHIFormat::NONE;
        U32 OffsetInBytes = 0;
        E_RHIVertexInputElementClass Class = E_RHIVertexInputElementClass::NONE;

        static F_RHIVertexInputElement Make(
            const F_Name& InName,
            E_RHIFormat InFormat,
            U32 InOffsetInBytes = 0,
            E_RHIVertexInputElementClass InClass = E_RHIVertexInputElementClass::PER_VERTEX_DATA
        )
        {
            F_RHIVertexInputElement Result;
            Result.Name = InName;
            Result.Format = InFormat;
            Result.OffsetInBytes = InOffsetInBytes;
            Result.Class = InClass;
            return Result;
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIVertexInputElement& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Name);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Format);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.OffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Class);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIVertexInputElement& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Name);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Format);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.OffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Class);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIVertexInputElement& A, const F_RHIVertexInputElement& B)
        {
            return (
                (A.Name == B.Name)
                && (A.Format == B.Format)
                && (A.OffsetInBytes == B.OffsetInBytes)
                && (A.Class == B.Class)
            );
        }
        friend B8 operator != (const F_RHIVertexInputElement& A, const F_RHIVertexInputElement& B)
        {
            return (
                (A.Name != B.Name)
                || (A.Format != B.Format)
                || (A.OffsetInBytes != B.OffsetInBytes)
                || (A.Class != B.Class)
            );
        }
    };
    struct F_RHIVertexInputLayout
    {
        TF_SmallVector<F_RHIVertexInputElement, 4> Elements; 
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIVertexInputLayout& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Elements);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIVertexInputLayout& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Elements);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIVertexInputLayout& A, const F_RHIVertexInputLayout& B)
        {
            return (
                (A.Elements == B.Elements)
            );
        }
        friend B8 operator != (const F_RHIVertexInputLayout& A, const F_RHIVertexInputLayout& B)
        {
            return (
                (A.Elements != B.Elements)
            );
        }
    };

    using F_RHIResourceViewSet = TF_SmallVector<TS<A_RHIResourceView>, 8>;
    using F_RHISamplerSet = TF_SmallVector<TS<A_RHISampler>, 8>;
    
    enum class E_RHIUniformDataFlag : U8
    {
        NONE = 0x0,
        FORCE_PUSH_CONSTANTS = 0x1,
        DENY_PUSH_CONSTANTS = 0x2,
        DEFAULT = NONE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RHIUniformDataFlag);
    using F_RHIUniformData = TF_SmallVector<U8, 256>;
    using F_RHIUniformDataView = TF_Span<U8>;
    
    enum class E_RHIBindGroupSlotType : U16
    {
        NONE,
        RESOURCE_VIEW,
        RESOURCE_VIEW_SET,
        SAMPLER,
        SAMPLER_SET,
        
        VERTEX_BUFFER,
        INDEX_BUFFER,
        RTV,
        DSV,
        
        UNIFORM_DATA
    };
    
    static constexpr Sz RHIBindGroupSlotDataAlignmentInBytes = ABYTEK_MIN_MEMORY_ALIGNMENT;
    
    struct F_RHIBindGroupSlotResourceView
    {
        TS<A_RHIResourceView> ResourceView;
    };
    struct F_RHIBindGroupSlotResourceViewSet
    {
        F_RHIResourceViewSet ResourceViewSet;
    };
    struct F_RHIBindGroupSlotSampler
    {
        TS<A_RHISampler> Sampler;
    };
    struct F_RHIBindGroupSlotSamplerSet
    {
        F_RHISamplerSet SamplerSet;
    };
    struct F_RHIBindGroupSlotRTV
    {
        TS<A_RHIResourceView> RTV;
    };
    struct F_RHIBindGroupSlotDSV
    {
        TS<A_RHIResourceView> DSV;
    };
    struct F_RHIBindGroupSlotVertexBuffer
    {
        F_RHIVertexBufferBinding VertexBufferBinding;
    };
    struct F_RHIBindGroupSlotIndexBuffer
    {
        F_RHIIndexBufferBinding IndexBufferBinding;
    };
    struct F_RHIBindGroupSlotUniformData
    {
        U16 SizeInBytes = 0;
        void* GetPayloadPtr() const noexcept
        {
            return (void*)(this + 1);
        }
    };
    
    struct F_RHIBindGroupSlotData;
    
    struct F_RHIBindGroupSlotInfo
    {
        E_RHIBindGroupSlotType Type = E_RHIBindGroupSlotType::NONE;
        U16 BinaryOffset = 0;
        
        F_RHIBindGroupSlotResourceView& ResolveResourceView(const F_RHIBindGroupSlotData& Data) const;
        F_RHIBindGroupSlotResourceViewSet& ResolveResourceViewSet(const F_RHIBindGroupSlotData& Data) const;
        
        F_RHIBindGroupSlotSampler& ResolveSampler(const F_RHIBindGroupSlotData& Data) const;
        F_RHIBindGroupSlotSamplerSet& ResolveSamplerSet(const F_RHIBindGroupSlotData& Data) const;
        
        F_RHIBindGroupSlotRTV& ResolveRTV(const F_RHIBindGroupSlotData& Data) const;
        F_RHIBindGroupSlotDSV& ResolveDSV(const F_RHIBindGroupSlotData& Data) const;
        
        F_RHIBindGroupSlotVertexBuffer& ResolveVertexBuffer(const F_RHIBindGroupSlotData& Data) const;
        F_RHIBindGroupSlotIndexBuffer& ResolveIndexBuffer(const F_RHIBindGroupSlotData& Data) const;
        
        F_RHIBindGroupSlotUniformData& ResolveUniformData(const F_RHIBindGroupSlotData& Data) const;
    };
    struct F_RHIBindGroupSlotData
    { 
        TF_SmallVector<F_RHIBindGroupSlotInfo, 8> SlotInfos;
        TF_SmallVector<U8, 256> Binary;
        
        F_RHIBindGroupSlotData() noexcept = default;
        ~F_RHIBindGroupSlotData() noexcept
        {
            Destruct();
        }
        
        F_RHIBindGroupSlotData(const F_RHIBindGroupSlotData& X) noexcept
        {
            CopyFrom(X);
        }
        F_RHIBindGroupSlotData& operator = (const F_RHIBindGroupSlotData& X) noexcept 
        {
            if (this != &X)
            {
                CopyFrom(X);
            }
            return *this;
        }
        
        F_RHIBindGroupSlotData(F_RHIBindGroupSlotData&& X) noexcept
        {
            MoveFrom(ABYTEK_MOVE(X));
        }
        F_RHIBindGroupSlotData& operator = (F_RHIBindGroupSlotData&& X) noexcept
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
            
            Binary.resize(X.Binary.size());
            
            U32 NumSlots = X.SlotInfos.size();
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                const auto& SlotInfo = X.SlotInfos[SlotIndex];
                switch (SlotInfo.Type)
                {
                case E_RHIBindGroupSlotType::RESOURCE_VIEW:
                    new (&SlotInfo.ResolveResourceView(*this)) F_RHIBindGroupSlotResourceView(
                        X.GetResourceView(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    new (&SlotInfo.ResolveResourceViewSet(*this)) F_RHIBindGroupSlotResourceViewSet(
                        X.GetResourceViewSet(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::SAMPLER:
                    new (&SlotInfo.ResolveSampler(*this)) F_RHIBindGroupSlotSampler(
                        X.GetSampler(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::SAMPLER_SET:
                    new (&SlotInfo.ResolveSamplerSet(*this)) F_RHIBindGroupSlotSamplerSet(
                        X.GetSamplerSet(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::RTV:
                    new (&SlotInfo.ResolveRTV(*this)) F_RHIBindGroupSlotRTV(
                        X.GetRTV(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::DSV:
                    new (&SlotInfo.ResolveDSV(*this)) F_RHIBindGroupSlotDSV(
                        X.GetDSV(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                    new (&SlotInfo.ResolveVertexBuffer(*this)) F_RHIBindGroupSlotVertexBuffer(
                        X.GetVertexBuffer(SlotIndex)
                    );
                    break;
                case E_RHIBindGroupSlotType::INDEX_BUFFER:
                    new (&SlotInfo.ResolveIndexBuffer(*this)) F_RHIBindGroupSlotIndexBuffer(
                        X.GetIndexBuffer(SlotIndex)
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::UNIFORM_DATA:
                    {
                        auto& SrcUniformData = X.GetUniformData(SlotIndex);
                        auto& UniformData = *(
                            new (&SlotInfo.ResolveUniformData(*this)) F_RHIBindGroupSlotUniformData(SrcUniformData)
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
        void MoveFrom(F_RHIBindGroupSlotData&& X)
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
                    new (&SlotInfo.ResolveResourceView(*this)) F_RHIBindGroupSlotResourceView(
                        ABYTEK_MOVE(X.GetResourceView(SlotIndex))
                    );
                    break;
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    new (&SlotInfo.ResolveResourceViewSet(*this)) F_RHIBindGroupSlotResourceViewSet(
                        ABYTEK_MOVE(X.GetResourceViewSet(SlotIndex))
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::SAMPLER:
                    new (&SlotInfo.ResolveSampler(*this)) F_RHIBindGroupSlotSampler(
                        ABYTEK_MOVE(X.GetSampler(SlotIndex))
                    );
                    break;
                case E_RHIBindGroupSlotType::SAMPLER_SET:
                    new (&SlotInfo.ResolveSamplerSet(*this)) F_RHIBindGroupSlotSamplerSet(
                        ABYTEK_MOVE(X.GetSamplerSet(SlotIndex))
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::RTV:
                    new (&SlotInfo.ResolveRTV(*this)) F_RHIBindGroupSlotRTV(
                        ABYTEK_MOVE(X.GetRTV(SlotIndex))
                    );
                    break;
                case E_RHIBindGroupSlotType::DSV:
                    new (&SlotInfo.ResolveDSV(*this)) F_RHIBindGroupSlotDSV(
                        ABYTEK_MOVE(X.GetDSV(SlotIndex))
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                    new (&SlotInfo.ResolveVertexBuffer(*this)) F_RHIBindGroupSlotVertexBuffer(
                        ABYTEK_MOVE(X.GetVertexBuffer(SlotIndex))
                    );
                    break;
                case E_RHIBindGroupSlotType::INDEX_BUFFER:
                    new (&SlotInfo.ResolveIndexBuffer(*this)) F_RHIBindGroupSlotIndexBuffer(
                        ABYTEK_MOVE(X.GetIndexBuffer(SlotIndex))
                    );
                    break;
                    
                case E_RHIBindGroupSlotType::UNIFORM_DATA:
                    {
                        auto& SrcUniformData = X.GetUniformData(SlotIndex);
                        auto& UniformData = *(
                            new (&SlotInfo.ResolveUniformData(*this)) F_RHIBindGroupSlotUniformData(SrcUniformData)
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
                    GetResourceView(SlotIndex).~F_RHIBindGroupSlotResourceView();
                    break;
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    GetResourceViewSet(SlotIndex).~F_RHIBindGroupSlotResourceViewSet();
                    break;
                
                case E_RHIBindGroupSlotType::SAMPLER:
                    GetSampler(SlotIndex).~F_RHIBindGroupSlotSampler();
                    break;
                case E_RHIBindGroupSlotType::SAMPLER_SET:
                    GetSamplerSet(SlotIndex).~F_RHIBindGroupSlotSamplerSet();
                    break;
                
                case E_RHIBindGroupSlotType::RTV:
                    GetRTV(SlotIndex).~F_RHIBindGroupSlotRTV();
                    break;
                case E_RHIBindGroupSlotType::DSV:
                    GetDSV(SlotIndex).~F_RHIBindGroupSlotDSV();
                    break;
                
                case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                    GetVertexBuffer(SlotIndex).~F_RHIBindGroupSlotVertexBuffer();
                    break;
                case E_RHIBindGroupSlotType::INDEX_BUFFER:
                    GetIndexBuffer(SlotIndex).~F_RHIBindGroupSlotIndexBuffer();
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
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceView& AddResourceView()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotResourceView)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotResourceView));
            auto Data = (F_RHIBindGroupSlotResourceView*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotResourceView();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::RESOURCE_VIEW, BinaryOffset });
            return *Data;
        }
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceViewSet& AddResourceViewSet()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotResourceViewSet)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotResourceViewSet));
            auto Data = (F_RHIBindGroupSlotResourceViewSet*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotResourceViewSet();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::RESOURCE_VIEW_SET, BinaryOffset });
            return *Data;
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSampler& AddSampler()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotSampler)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotSampler));
            auto Data = (F_RHIBindGroupSlotSampler*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotSampler();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::SAMPLER, BinaryOffset });
            return *Data;
        }
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSamplerSet& AddSamplerSet()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotSamplerSet)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotSamplerSet));
            auto Data = (F_RHIBindGroupSlotSamplerSet*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotSamplerSet();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::SAMPLER_SET, BinaryOffset });
            return *Data;
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotRTV& AddRTV()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotRTV)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotRTV));
            auto Data = (F_RHIBindGroupSlotRTV*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotRTV();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::RTV, BinaryOffset });
            return *Data;
        }
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotDSV& AddDSV()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotDSV)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotDSV));
            auto Data = (F_RHIBindGroupSlotDSV*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotDSV();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::DSV, BinaryOffset });
            return *Data;
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotVertexBuffer& AddVertexBuffer()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotVertexBuffer)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotVertexBuffer));
            auto Data = (F_RHIBindGroupSlotVertexBuffer*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotVertexBuffer();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::VERTEX_BUFFER, BinaryOffset });
            return *Data;
        }
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotIndexBuffer& AddIndexBuffer()
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotIndexBuffer)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotIndexBuffer));
            auto Data = (F_RHIBindGroupSlotIndexBuffer*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotIndexBuffer();
            SlotInfos.push_back({ E_RHIBindGroupSlotType::INDEX_BUFFER, BinaryOffset });
            return *Data;
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotUniformData& AddUniformData(U16 SizeInBytes)
        {
            U16 BinaryOffset = Binary.size();
            BinaryOffset = static_cast<U16>(AlignAddress_PO2(BinaryOffset, ABYTEK_ALIGNOF(F_RHIBindGroupSlotUniformData)));
            Binary.resize(BinaryOffset + sizeof(F_RHIBindGroupSlotUniformData) + SizeInBytes);
            auto Data = (F_RHIBindGroupSlotUniformData*)(Binary.data() + BinaryOffset);
            new (Data) F_RHIBindGroupSlotUniformData { SizeInBytes };
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
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceView& GetResourceView(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveResourceView(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotResourceView& GetResourceView(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveResourceView(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceViewSet& GetResourceViewSet(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveResourceViewSet(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotResourceViewSet& GetResourceViewSet(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveResourceViewSet(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSampler& GetSampler(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveSampler(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotSampler& GetSampler(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveSampler(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSamplerSet& GetSamplerSet(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveSamplerSet(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotSamplerSet& GetSamplerSet(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveSamplerSet(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotRTV& GetRTV(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveRTV(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotRTV& GetRTV(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveRTV(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotDSV& GetDSV(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveDSV(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotDSV& GetDSV(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveDSV(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotVertexBuffer& GetVertexBuffer(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveVertexBuffer(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotVertexBuffer& GetVertexBuffer(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveVertexBuffer(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotIndexBuffer& GetIndexBuffer(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveIndexBuffer(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotIndexBuffer& GetIndexBuffer(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveIndexBuffer(*this);
        }
        
        ABYTEK_FORCE_INLINE F_RHIBindGroupSlotUniformData& GetUniformData(U32 SlotIndex) noexcept
        {
            return SlotInfos[SlotIndex].ResolveUniformData(*this);
        }
        ABYTEK_FORCE_INLINE const F_RHIBindGroupSlotUniformData& GetUniformData(U32 SlotIndex) const noexcept
        {
            return SlotInfos[SlotIndex].ResolveUniformData(*this);
        }
    };
    
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceView& F_RHIBindGroupSlotInfo::ResolveResourceView(const F_RHIBindGroupSlotData& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::RESOURCE_VIEW) << "Invalid type";
        return *(F_RHIBindGroupSlotResourceView*)&Data.Binary[BinaryOffset];
    }
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotResourceViewSet& F_RHIBindGroupSlotInfo::ResolveResourceViewSet(const F_RHIBindGroupSlotData& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::RESOURCE_VIEW_SET) << "Invalid type";
        return *(F_RHIBindGroupSlotResourceViewSet*)&Data.Binary[BinaryOffset];
    }
        
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSampler& F_RHIBindGroupSlotInfo::ResolveSampler(const F_RHIBindGroupSlotData& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::SAMPLER) << "Invalid type";
        return *(F_RHIBindGroupSlotSampler*)&Data.Binary[BinaryOffset];
    }
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotSamplerSet& F_RHIBindGroupSlotInfo::ResolveSamplerSet(const F_RHIBindGroupSlotData& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::SAMPLER_SET) << "Invalid type";
        return *(F_RHIBindGroupSlotSamplerSet*)&Data.Binary[BinaryOffset];
    }
        
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotRTV& F_RHIBindGroupSlotInfo::ResolveRTV(const F_RHIBindGroupSlotData& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::RTV) << "Invalid type";
        return *(F_RHIBindGroupSlotRTV*)&Data.Binary[BinaryOffset];
    }
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotDSV& F_RHIBindGroupSlotInfo::ResolveDSV(const F_RHIBindGroupSlotData& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::DSV) << "Invalid type";
        return *(F_RHIBindGroupSlotDSV*)&Data.Binary[BinaryOffset];
    }
        
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotVertexBuffer& F_RHIBindGroupSlotInfo::ResolveVertexBuffer(const F_RHIBindGroupSlotData& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::VERTEX_BUFFER) << "Invalid type";
        return *(F_RHIBindGroupSlotVertexBuffer*)&Data.Binary[BinaryOffset];
    }
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotIndexBuffer& F_RHIBindGroupSlotInfo::ResolveIndexBuffer(const F_RHIBindGroupSlotData& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::INDEX_BUFFER) << "Invalid type";
        return *(F_RHIBindGroupSlotIndexBuffer*)&Data.Binary[BinaryOffset];
    }
        
    ABYTEK_FORCE_INLINE F_RHIBindGroupSlotUniformData& F_RHIBindGroupSlotInfo::ResolveUniformData(const F_RHIBindGroupSlotData& Data) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(Type == E_RHIBindGroupSlotType::UNIFORM_DATA) << "Invalid type";
        return *(F_RHIBindGroupSlotUniformData*)&Data.Binary[BinaryOffset];
    }
    
    using F_RHIBindGroupEncodedData = TF_SmallVector<U8, 256>;
    using F_RHIBindGroupEncodedDataView = TF_Span<U8>;
    
    enum class E_RHIBindGroupCommitFrequency : U8
    {
        NONE,
        STATIC,
        DYNAMIC,
        
        DEFAULT = STATIC
    };
    
    struct F_RHIBindGroupBuildParams : F_RHIContextChildBuildParams
    {
        TS<A_RHIBindGroupTemplateRuntime> TemplateRuntime;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIBindGroup : public A_RHIContextChild
    {
    private:
        TS<A_RHIBindGroupTemplateRuntime> _TemplateRuntime;
        F_RHIBindGroupSlotData _SlotData;
        
        U32 _EncodedDataSizeInBytes = ~U32(0);
        U32 _EncodedDataAlignmentInBytes = ~U32(0);
        
        F_AtomicFlag _EverCommitted;

    public:
        ABYTEK_FORCE_INLINE const auto& GetTemplateRuntime() const noexcept
        {
            return _TemplateRuntime;
        }
        ABYTEK_FORCE_INLINE auto& InjectTemplateRuntime() noexcept
        {
            return _TemplateRuntime;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetSlotData() const noexcept
        {
            return _SlotData;
        }
        ABYTEK_FORCE_INLINE auto& InjectSlotData() noexcept
        {
            return _SlotData;
        }

        ABYTEK_FORCE_INLINE const auto& GetEncodedDataSizeInBytes() const noexcept
        {
            return _EncodedDataSizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto& InjectEncodedDataSizeInBytes() noexcept
        {
            return _EncodedDataSizeInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetEncodedDataAlignmentInBytes() const noexcept
        {
            return _EncodedDataAlignmentInBytes;
        }
        ABYTEK_FORCE_INLINE auto& InjectEncodedDataAlignmentInBytes() noexcept
        {
            return _EncodedDataAlignmentInBytes;
        }
        
        ABYTEK_FORCE_INLINE auto EverCommitted() const noexcept
        {
            return _EverCommitted.test(boost::memory_order_acquire);
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIBindGroup);
        virtual void Build(const F_RHIBindGroupBuildParams& BuildParams);
        virtual void Release();
        
    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;

    public:
        virtual void BindResourceView(
            U32 Index,
            const TS_Valid<A_RHIResource>& Resource,
            const F_RHIResourceAccess& Access
        );
        virtual void BindResourceView(
            U32 Index,
            const TS_Valid<A_RHIResource>& Resource
        );
        virtual void BindResourceView(
            U32 Index,
            const TS_Valid<A_RHIResourceView>& ResourceView
        );
        virtual void BindResourceViewSet(
            U32 Index,
            const F_RHIResourceViewSet& ResourceViewSet
        );
        virtual void BindSampler(
            U32 Index,
            const TS_Valid<A_RHISampler>& Sampler
        );
        virtual void BindSamplerSet(
            U32 Index,
            const F_RHISamplerSet& SamplerSet
        );
        virtual void BindRTV(
            U32 Index,
            const TS_Valid<A_RHIResource>& Resource
        );
        virtual void BindRTV(
            U32 Index,
            const TS_Valid<A_RHIResourceView>& RTV
        );
        virtual void BindDSV(
            U32 Index,
            const TS_Valid<A_RHIResource>& Resource
        );
        virtual void BindDSV(
            U32 Index,
            const TS_Valid<A_RHIResourceView>& DSV
        );
        virtual void BindVertexBuffer(
            U32 Index,
            const TS_Valid<A_RHIResource>& Resource
        );
        virtual void BindVertexBuffer(
            U32 Index,
            const F_RHIVertexBufferBinding& VertexBufferBinding
        );
        virtual void BindIndexBuffer(
            U32 Index,
            const TS_Valid<A_RHIResource>& Resource
        );
        virtual void BindIndexBuffer(
            U32 Index,
            const F_RHIIndexBufferBinding& IndexBufferBinding
        );
        virtual void BindUniformData(
            U32 Index,
            const F_RHIUniformDataView& UniformDataView
        );
        template<typename __F_UniformData>
        void BindUniformData(
            U32 Index,
            const __F_UniformData& UniformData
        )
        {
            BindUniformData(Index, { (U8*)&UniformData, sizeof(__F_UniformData) });
        }

    public:
        void BindResourceView(
            const F_Name& Name,
            const TS_Valid<A_RHIResource>& Resource,
            const F_RHIResourceAccess& Access
        );
        void BindResourceView(
            const F_Name& Name,
            const TS_Valid<A_RHIResource>& Resource
        );
        void BindResourceView(
            const F_Name& Name,
            const TS_Valid<A_RHIResourceView>& ResourceView
        );
        void BindResourceViewSet(
            const F_Name& Name,
            const F_RHIResourceViewSet& ResourceViewSet
        );
        void BindSampler(
            const F_Name& Name,
            const TS_Valid<A_RHISampler>& Sampler
        );
        void BindSamplerSet(
            const F_Name& Name,
            const F_RHISamplerSet& SamplerSet
        );
        void BindRTV(
            const F_Name& Name,
            const TS_Valid<A_RHIResource>& Resource
        );
        void BindRTV(
            const F_Name& Name,
            const TS_Valid<A_RHIResourceView>& RTV
        );
        void BindDSV(
            const F_Name& Name,
            const TS_Valid<A_RHIResource>& Resource
        );
        void BindDSV(
            const F_Name& Name,
            const TS_Valid<A_RHIResourceView>& DSV
        );
        void BindVertexBuffer(
            const F_Name& Name,
            const TS_Valid<A_RHIResource>& Resource
        );
        void BindVertexBuffer(
            const F_Name& Name,
            const F_RHIVertexBufferBinding& VertexBufferBinding
        );
        void BindIndexBuffer(
            const F_Name& Name,
            const TS_Valid<A_RHIResource>& Resource
        );
        void BindIndexBuffer(
            const F_Name& Name,
            const F_RHIIndexBufferBinding& IndexBufferBinding
        );
        void BindUniformData(
            const F_Name& Name,
            const F_RHIUniformDataView& UniformDataView
        );
        template<typename __F_UniformData>
        void BindUniformData(
            const F_Name& Name,
            const __F_UniformData& UniformData
        )
        {
            BindUniformData(Name, { (U8*)&UniformData, sizeof(__F_UniformData) });
        }
        
    public:
        B8 HasSlot(const F_Name& Name) const;
        
    public:
        virtual void Commit();
        
    public:
        virtual void EncodeData(void* OutData); 
        F_RHIBindGroupEncodedData EncodeData();
        
    public:
        virtual F_RHIResourceViewBindlessRange GetResourceViewBindlessRange(U32 Index) const;
        virtual F_RHISamplerBindlessRange GetSamplerBindlessRange(U32 Index) const;
        F_RHIResourceViewBindlessRange GetResourceViewBindlessRange(const F_Name& Name) const;
        F_RHISamplerBindlessRange GetSamplerBindlessRange(const F_Name& Name) const;
        
    public:
        
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_RHIVertexInputElement>
    {
        size_t operator ()(const Abytek::F_RHIVertexInputElement& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.Name);
            hash_combine(Result, X.Format);
            hash_combine(Result, X.OffsetInBytes);
            hash_combine(Result, X.Class);
            return Result;
        }
    };
    template<>
    struct hash<Abytek::F_RHIVertexInputLayout>
    {
        size_t operator ()(const Abytek::F_RHIVertexInputLayout& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.Elements);
            return Result;
        }
    };
}
