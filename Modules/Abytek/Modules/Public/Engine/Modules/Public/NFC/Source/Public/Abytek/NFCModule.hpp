#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"


namespace Abytek
{
    struct F_RenderSceneUpdateRange;
    struct F_RenderScenePostUpdateRange;
    struct F_RenderableComponentUpdateRange;

    class ABYTEK_ENGINE_NFC_API F_NFCModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_NFCModule)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_NFCModule);

    private:
        TU<F_RenderSceneUpdateRange> _RenderSceneUpdateRange;
        TU<F_RenderScenePostUpdateRange> _RenderScenePostUpdateRange;
        TU<F_RenderableComponentUpdateRange> _RenderableComponentUpdateRange;
        
    public:
        
    public:
        F_NFCModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_NFCModule() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnInit() override;
        void OnRelease() override;
    };
}
