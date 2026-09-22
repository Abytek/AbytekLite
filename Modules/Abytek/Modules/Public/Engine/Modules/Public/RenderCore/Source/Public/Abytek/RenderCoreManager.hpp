#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/World/WorldSubsystem.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    class A_RHITemplateSerializer;
    class F_RenderRegistry;
    class F_RenderRegistryRuntime;
    class F_RenderCorePack;
    class F_GlobalRenderPack;
    class F_RenderPackProxy;
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderCoreManager final : public A_WorldSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldSubsystem)
        ABYTEK_END_REFLECTOR(F_RenderCoreManager);

    public:
        ABYTEK_DECLARE_STATIC_SUBSYSTEM(F_RenderCoreManager);

    private:
        F_RHIFeatureSupports _MainFeatureSupports;
        TS<F_RenderRegistry> _MainRegistry;
        TS<F_GlobalRenderPack> _GlobalRenderPack;
        F_AtomicFlag _AllowCreateRenderPacks;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetMainFeatureSupports() const noexcept
        {
            return _MainFeatureSupports;
        }
        ABYTEK_FORCE_INLINE const auto& GetMainRegistry() const noexcept
        {
            return _MainRegistry;
        }
        ABYTEK_FORCE_INLINE const auto& GetGlobalRenderPack() const noexcept
        {
            return _GlobalRenderPack;
        }
        ABYTEK_FORCE_INLINE auto GetAllowCreateRenderPacks() const noexcept
        {
            return _AllowCreateRenderPacks.test(boost::memory_order_acquire);
        }

    public:
        F_RenderCoreManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RenderCoreManager() override;

    protected:
        void OnInit() override;
        void OnStartup() override;
        void OnShutdown() override;
        void OnRelease() override;
    };
}
