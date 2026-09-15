#include "Abytek/RHISampleModule_UploadBuffer.hpp"

#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RHISampleModule_UploadBuffer)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RHISampleModule_UploadBuffer"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_RHISampleModule_UploadBuffer)
    
    F_RHISampleModule_UploadBuffer::F_RHISampleModule_UploadBuffer(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_RHISampleModule_UploadBuffer::~F_RHISampleModule_UploadBuffer()
    {
    }

    void F_RHISampleModule_UploadBuffer::OnReflect()
    {
    }

    void F_RHISampleModule_UploadBuffer::OnStartup()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            []
            {
                auto RHIContext = H_RHI::GetMainContext();
                auto RHIProcess = H_RHI::GetMainProcess();
                
                U32 NumElements = 16;
                
                F_RHIBufferBuildParams BufferBuildParams;
                BufferBuildParams.Context = RHIContext.Weak();
                BufferBuildParams.Format = E_RHIFormat::R32_UINT;
                BufferBuildParams.BufferAspect.SizeInBytes = sizeof(U32) * NumElements;
                BufferBuildParams.BufferAspect.StrideInBytes = sizeof(U32);
                BufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeSRVCapabilities();
                auto Buffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
                
                auto SubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(RHIProcess);
                SubmissionList->SetDebugName(ABYTEK_DEBUG_NAME("Main"));
                
                U32* DataPtr = H_Frame::GetArena(E_FrameParamType::RENDER)->AllocateData<U32>(NumElements);
                for (U32 Idx = 0; Idx < NumElements; ++Idx)
                {
                    DataPtr[Idx] = Idx;
                }
                
                H_RHIPassUtilities::UploadBuffer(
                    SubmissionList,
                    { (const U8*)(DataPtr), (const U8*)(DataPtr + NumElements) },
                    Buffer,
                    0,
                    ABYTEK_DEBUG_NAME("UploadBuffer")
                );
                
                RHIProcess->AddSubmissionItem(SubmissionList);
                
                H_RHI::SignalCapture();
            }
        );
    }
}
