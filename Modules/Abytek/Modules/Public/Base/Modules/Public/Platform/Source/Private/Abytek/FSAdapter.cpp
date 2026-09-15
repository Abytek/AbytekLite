#include "Abytek/FSAdapter.hpp"


namespace Abytek
{
    A_FSAdapter::A_FSAdapter()
    {
    }
    A_FSAdapter::~A_FSAdapter()
    {
    }

    F_FeedbackStatus A_FSAdapter::GetEntryStat(const F_Text& Path, F_FSEntryStat& OutStat) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    B8 A_FSAdapter::Exists(const F_Text& Path, E_FSEntryType Type) const
    {
        F_FSEntryStat Stat;
        if (!GetEntryStat(Path, Stat))
        {
            return false;
        }
        return FlagHasAny(Stat.Type, Type);
    }

    F_FeedbackStatus A_FSAdapter::OpenFile(const F_FSFileOpenParams& OpenParams, TS<A_FSFile>& OutFile) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSAdapter::CreateFile_(const F_Text& Path) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSAdapter::DeleteFile_(const F_Text& Path) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSAdapter::MoveFile_(const F_Text& NewPath, const F_Text& CurrentPath) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSAdapter::CopyFile_(const F_Text& DstPath, const F_Text& SrcPath) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }

    F_FeedbackStatus A_FSAdapter::CreateDirectory_(const F_Text& Path) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSAdapter::DeleteDirectory_(const F_Text& Path) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSAdapter::MoveDirectory_(const F_Text& NewPath, const F_Text& CurrentPath) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSAdapter::CopyDirectory_(const F_Text& DstPath, const F_Text& SrcPath) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSAdapter::ReadDirectory(const F_Text& Path, TF_Vector<F_Text>& OutEntries) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSAdapter::EnsureDirectory(const F_Text& Path) const
    {
        if (!Exists(Path, E_FSEntryType::DIRECTORY))
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(CreateDirectory_(Path));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSAdapter::ClearDirectory(const F_Text& Path) const
    {
        if (!Exists(Path, E_FSEntryType::DIRECTORY))
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not found directory: ") + Path);
        }
        ABYTEK_FEEDBACK_STATUS_CHECK(DeleteDirectory_(Path));
        ABYTEK_FEEDBACK_STATUS_CHECK(CreateDirectory_(Path));
        return F_FeedbackStatus::MakeSucceeded();
    }
}
