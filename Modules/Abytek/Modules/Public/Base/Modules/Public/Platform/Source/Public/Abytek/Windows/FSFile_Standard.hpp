#pragma once

#include "Abytek/FSFile_Standard.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_WindowsFSFile_Standard final : public A_FSFile_Standard
    {
    public:
        friend class F_WindowsFSAdapter_Standard;

    private:
        HANDLE _PlatformHandle = INVALID_HANDLE_VALUE;

    public:
        ABYTEK_FORCE_INLINE HANDLE GetPlatformHandle() const noexcept { return _PlatformHandle; }
        
    public:
        F_WindowsFSFile_Standard(const F_FSFileOpenParams& OpenParams, HANDLE PlatformHandle);
        ~F_WindowsFSFile_Standard() override;

    protected:
        F_FeedbackStatus SetSize(Sz NewSize) const override;
        
    public:
        F_FeedbackStatus GetSize(Sz& OutSize) const override;
        F_FeedbackStatus Seek(PDiff NewOffset) override;
        F_FeedbackStatus ReadBinary(const TF_Span<U8>& OutData, Sz& OutSize) override;
        F_FeedbackStatus WriteBinary(const TF_Span<const U8>& Data, Sz& OutSize) override;
    };
}
#endif