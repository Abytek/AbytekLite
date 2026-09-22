#include "Abytek/RenderResource.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RenderRegistryRuntime.hpp"
#include "Abytek/RHISubmissionQueue.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    A_RenderResource::A_RenderResource()
    {
    }
    A_RenderResource::~A_RenderResource()
    {
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(
            !_EnqueuedToInit
            || _EnqueuedToRelease
        ) << "Requires releasing render resource before its destruction";
    }

    void A_RenderResource::Init(const TS<F_RenderRegistry>& RenderRegistry)
    {
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(!_EnqueuedToInit) << "Render resource was already enqueued to initialize";
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [SThis = ABYTEK_STHIS(), RenderRegistry]
            {
                SThis->_RenderRegistryRuntime = RenderRegistry->GetOrActiveRuntime(H_RHI::GetMainContext());
                SThis->OnInit_RenderTask(H_RHI::GetMainSubmissionQueue());
            }
        );
#ifdef ABYTEK_ENGINE_RENDER_CORE_ENABLE_ASSERTIONS
        _EnqueuedToInit = true;
#endif
    }
    void A_RenderResource::Release()
    {
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(_EnqueuedToInit) << "Render resource was not enqueued to initialize, cannot release";
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [SThis = ABYTEK_STHIS()]
            {
                SThis->OnRelease_RenderTask(H_RHI::GetMainSubmissionQueue());
                SThis->_RenderRegistryRuntime = {};
            }
        );
#ifdef ABYTEK_ENGINE_RENDER_CORE_ENABLE_ASSERTIONS
        _EnqueuedToRelease = true;
#endif
    }

    void A_RenderResource::OnInit_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderResource::OnRelease_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }

    const F_RenderCoreRHIConfig& A_RenderResource::GetRHIConfig() const noexcept
    {
        return _RenderRegistryRuntime->GetRHIConfig();
    }
    const F_RHIFeatureSupports& A_RenderResource::GetRHIFeatureSupports() const noexcept
    {
        return GetRHIConfig().FeatureSupports;
    }
}
