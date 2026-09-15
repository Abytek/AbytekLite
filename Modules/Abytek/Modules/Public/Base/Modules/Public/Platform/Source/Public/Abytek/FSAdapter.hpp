#pragma once

#include "Abytek/FSCommon.hpp"


namespace Abytek
{
    class A_FSFile;
    
    class ABYTEK_BASE_PLATFORM_API A_FSAdapter : public A_Object
    {
    protected:
        A_FSAdapter();
        
    public:
        virtual ~A_FSAdapter() override;
        
    public:
        virtual F_FeedbackStatus GetEntryStat(const F_Text& Path, F_FSEntryStat& OutStat) const;
        virtual B8 Exists(const F_Text& Path, E_FSEntryType Type = E_FSEntryType::ALL) const;
        
    public:
        virtual F_FeedbackStatus OpenFile(const F_FSFileOpenParams& OpenParams, TS<A_FSFile>& OutFile) const;
        virtual F_FeedbackStatus CreateFile_(const F_Text& Path) const;
        virtual F_FeedbackStatus DeleteFile_(const F_Text& Path) const;
        virtual F_FeedbackStatus MoveFile_(const F_Text& NewPath, const F_Text& CurrentPath) const;
        virtual F_FeedbackStatus CopyFile_(const F_Text& DstPath, const F_Text& SrcPath) const;
        
    public:
        virtual F_FeedbackStatus CreateDirectory_(const F_Text& Path) const;
        virtual F_FeedbackStatus DeleteDirectory_(const F_Text& Path) const;
        virtual F_FeedbackStatus MoveDirectory_(const F_Text& NewPath, const F_Text& CurrentPath) const;
        virtual F_FeedbackStatus CopyDirectory_(const F_Text& DstPath, const F_Text& SrcPath) const;
        virtual F_FeedbackStatus ReadDirectory(const F_Text& Path, TF_Vector<F_Text>& OutEntries) const;
        virtual F_FeedbackStatus EnsureDirectory(const F_Text& Path) const;
        virtual F_FeedbackStatus ClearDirectory(const F_Text& Path) const;
    };
}