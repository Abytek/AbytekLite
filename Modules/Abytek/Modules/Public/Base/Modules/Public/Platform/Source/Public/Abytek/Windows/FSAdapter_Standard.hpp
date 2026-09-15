#pragma once

#include "Abytek/FSAdapter_Standard.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_WindowsFSAdapter_Standard final : public A_FSAdapter_Standard
    {
    public:
        F_WindowsFSAdapter_Standard();  
        ~F_WindowsFSAdapter_Standard() override;
        
    public:
        F_FeedbackStatus GetEntryStat(const F_Text& Path, F_FSEntryStat& OutStat) const override;

    public:
        F_FeedbackStatus OpenFile(const F_FSFileOpenParams& OpenParams, TS<A_FSFile>& OutFile) const override;
        F_FeedbackStatus CreateFile_(const F_Text& Path) const override;
        F_FeedbackStatus DeleteFile_(const F_Text& Path) const override;
        F_FeedbackStatus MoveFile_(const F_Text& NewPath, const F_Text& CurrentPath) const override;
        F_FeedbackStatus CopyFile_(const F_Text& DstPath, const F_Text& SrcPath) const override;
        
    public:
        F_FeedbackStatus CreateDirectory_(const F_Text& Path) const override;
        F_FeedbackStatus DeleteDirectory_(const F_Text& Path) const override;
        F_FeedbackStatus MoveDirectory_(const F_Text& NewPath, const F_Text& CurrentPath) const override;
        F_FeedbackStatus CopyDirectory_(const F_Text& DstPath, const F_Text& SrcPath) const override;
        F_FeedbackStatus ReadDirectory(const F_Text& Path, TF_Vector<F_Text>& OutEntries) const override;
    };
}
#endif