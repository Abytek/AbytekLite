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

    void A_RenderResource::Init(const TS<A_RenderRegistryPort>& RenderRegistryPort)
    {
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(!_EnqueuedToInit) << "Render resource was already enqueued to initialize";
        RenderRegistryPort->EnqueueCommand(
            [SThis = ABYTEK_STHIS(), RenderRegistryPortData = RenderRegistryPort->GetData()]
            {
                SThis->_RenderRegistryRuntime = RenderRegistryPortData->GetRegistryRuntime();
                SThis->OnInit_RenderTask(
                    RenderRegistryPortData->GetSubmissionItemContainer()
                );
            }
        );
#ifdef ABYTEK_ENGINE_RENDER_CORE_ENABLE_ASSERTIONS
        _EnqueuedToInit = true;
        _LastRenderRegistryPort = RenderRegistryPort;
#endif
    }
    void A_RenderResource::Release(const TS<A_RenderRegistryPort>& RenderRegistryPort)
    {
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(_LastRenderRegistryPort == RenderRegistryPort) << "Render registry port mismatch";
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(_EnqueuedToInit) << "Render resource was not enqueued to initialize, cannot release";
        RenderRegistryPort->EnqueueCommand(
            [SThis = ABYTEK_STHIS(), RenderRegistryPortData = RenderRegistryPort->GetData()]
            {
                SThis->OnRelease_RenderTask(
                    RenderRegistryPortData->GetSubmissionItemContainer()
                );
                SThis->_RenderRegistryRuntime = {};
            }
        );
#ifdef ABYTEK_ENGINE_RENDER_CORE_ENABLE_ASSERTIONS
        _EnqueuedToRelease = true;
        _LastRenderRegistryPort = {};
#endif
    }

    void A_RenderResource::OnInit_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderResource::OnRelease_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }

    const F_RHIFeatureSupports& A_RenderResource::GetRHIFeatureSupports() const noexcept
    {
        return _RenderRegistryRuntime->GetRHIFeatureSupports();
    }
}
