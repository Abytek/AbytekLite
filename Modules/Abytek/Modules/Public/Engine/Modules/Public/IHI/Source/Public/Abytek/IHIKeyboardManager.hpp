#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"
#include "Abytek/IHIKeyboardCommon.hpp"


namespace Abytek
{
    class A_IHIKeyboard;
    class A_IHIButtonCoder;
    
    class ABYTEK_ENGINE_IHI_API A_IHIKeyboardManager : public A_RAObject
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_IHIKeyboardManager);
        
    public:
        static F_Name GetUpdateUpdateFunctionName();
        static F_Name GetPostUpdateUpdateFunctionName();
        static void GlobalInit();
        static void GlobalRelease();
        
    private:
        TF_Set<TS<A_IHIKeyboard>> _Keyboards;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetKeyboards() const noexcept
        {
            return _Keyboards;
        }
        ABYTEK_FORCE_INLINE auto& InjectKeyboards() noexcept
        {
            return _Keyboards;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_IHIKeyboardManager);
        virtual void Build();
        virtual void Release() override;
        
    public:
        virtual void Startup();
        virtual void Shutdown();

    protected:
        TW_Valid<A_IHIKeyboard> CreateKeyboard();
        void DestroyKeyboard(const TW_Valid<A_IHIKeyboard>& Keyboard);
        
    protected:
        virtual void OnUpdate();
        virtual void OnPostUpdate();
        
    public:
        TW<A_IHIKeyboard> GetMainKeyboard();
    };
}
