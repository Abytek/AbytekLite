#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"
#include "Abytek/IHIMouseCommon.hpp"


namespace Abytek
{
    class A_IHIMouse;
    class A_IHIButtonCoder;
    
    class ABYTEK_ENGINE_IHI_API A_IHIMouseManager : public A_RAObject
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_IHIMouseManager);
        
    public:
        static F_Name GetUpdateUpdateFunctionName();
        static F_Name GetPostUpdateUpdateFunctionName();
        static void GlobalInit();
        static void GlobalRelease();
        
    private:
        TF_Set<TS<A_IHIMouse>> _Mouses;

    public:
        ABYTEK_FORCE_INLINE const auto& GetMouses() const noexcept
        {
            return _Mouses;
        }
        ABYTEK_FORCE_INLINE auto& InjectMouses() noexcept
        {
            return _Mouses;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_IHIMouseManager);
        virtual void Build();
        virtual void Release() override;
        
    public:
        virtual void Startup();
        virtual void Shutdown();

    protected:
        TW_Valid<A_IHIMouse> CreateMouse();
        void DestroyMouse(const TW_Valid<A_IHIMouse>& Mouse);
        
    protected:
        virtual void OnUpdate();
        virtual void OnPostUpdate();
        
    public:
        TW<A_IHIMouse> GetMainMouse();
    };
}
