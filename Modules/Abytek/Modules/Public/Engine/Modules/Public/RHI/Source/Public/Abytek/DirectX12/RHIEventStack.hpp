#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/DirectX12/RHIProfilerEventState.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
    struct ABYTEK_ENGINE_RHI_API F_DirectX12RHIEventStack
    {
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> D3D12CommandList;
        U32 Size = 0;
#ifdef ABYTEK_ENGINE_RHI_ENABLE_NVTX
        TF_SmallVector<nvtx3::range_handle, 4> NVTXRanges;
#endif
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return D3D12CommandList;
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        void Push(const F_DebugName& Name, const F_Vector3_F32& Color = F_Vector3_F32::One())
        {
            ABYTEK_ENGINE_RHI_ASSERT(IsValid()) << "Invalid event stack";
            F_String StrProfilerEventState;
#ifdef ABYTEK_DEBUG_INFO
            StrProfilerEventState = ToString(*Name);
#endif
            F_Vector3_U32 ColorU32x3(
                Min<U32>(
                    static_cast<U32>(Color.X * 255.0f), 
                    255
                ),
                Min<U32>(
                    static_cast<U32>(Color.Y * 255.0f), 
                    255
                ),
                Min<U32>(
                    static_cast<U32>(Color.Z * 255.0f), 
                    255
                )
            );
            UINT32 ColorU32 = (0xff000000u | ((UINT32)ColorU32x3.X << 16) | ((UINT32)ColorU32x3.Y << 8) | (UINT32)ColorU32x3.Z);
            
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
            PIXBeginEvent(
                D3D12CommandList.Get(), 
                ColorU32, 
                StrProfilerEventState.c_str()
            );
#endif
#ifdef ABYTEK_ENGINE_RHI_ENABLE_NVTX
            nvtx3::event_attributes NVTXEventAttributes(
                nvtx3::color { ColorU32 },
                nvtx3::message { StrProfilerEventState.c_str() }
            );
            NVTXRanges.push_back(nvtx3::start_range(NVTXEventAttributes));
#endif
            ++Size;
        }
        void Pop(U32 Num = 1)
        {
            ABYTEK_ENGINE_RHI_ASSERT(IsValid()) << "Invalid event stack";
            ABYTEK_ENGINE_RHI_ASSERT(Num <= Size) << "Num too large";
            for (U32 Idx = 0; Idx < Num; ++Idx)
            {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_NVTX
                nvtx3::end_range(NVTXRanges.back());
                NVTXRanges.resize(NVTXRanges.size() - 1);
#endif
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
                PIXEndEvent(D3D12CommandList.Get());
#endif
            }
            Size -= Num;
        }
        
        void Push(const F_DirectX12RHIProfilerEventState& State)
        {
            F_DebugName StateName;
#ifdef ABYTEK_DEBUG_INFO
            StateName = State.Name;
#endif
            Push(StateName, State.Color);
        }
        template<typename __F_Iterator>
        void Push(const __F_Iterator& BeginIt, const __F_Iterator& EndIt)
        {
            for (auto It = BeginIt; It != EndIt; ++It)
            {
                Push(*It);
            }
        }
        void Push(const TF_SmallVector<F_DirectX12RHIProfilerEventState, 1>& States)
        {
            Push(States.begin(), States.end());
        }
        template<typename __F_Iterator>
        void Pop(const __F_Iterator& BeginIt, const __F_Iterator& EndIt)
        {
            for (auto It = BeginIt; It != EndIt; ++It)
            {
                Pop();
            }
        }
        void Pop(const TF_SmallVector<F_DirectX12RHIProfilerEventState, 1>& States)
        {
            Pop(States.size());
        }
    };
#endif
}
#endif