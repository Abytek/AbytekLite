#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_AndroidJNIManager : public A_Object
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_AndroidJNIManager);

    private:
        JavaVM* _VM = nullptr;

    public:
        ABYTEK_FORCE_INLINE auto GetVM() const noexcept
        {
            return _VM;
        }

    public:
        F_AndroidJNIManager();
        virtual ~F_AndroidJNIManager() override;

    public:
        void AttachToCurrentThread();
        void DeattachFromCurrentThread();
    };
}
#endif