#include "Abytek/FSUtilities.hpp"
#include "Abytek/FSAdapter.hpp"
#include "Abytek/FSAdapter_Standard.hpp"


namespace Abytek
{
    TW<A_FSAdapter> H_FSUtilities::GetDefaultAdapter()
    {
        return A_FSAdapter_Standard::GetInstance();
    }
    
    B8 H_FSUtilities::Exists(const F_Text& Path, E_FSEntryType EntryTypes, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->Exists(Path, EntryTypes);
    }

    F_FeedbackStatus H_FSUtilities::OpenFile(const F_FSFileOpenParams& OpenParams, TS<A_FSFile>& OutFile, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->OpenFile(OpenParams, OutFile);
    }
    F_FeedbackStatus H_FSUtilities::CreateFile_(const F_Text& Path, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->CreateFile_(Path);
    }
    F_FeedbackStatus H_FSUtilities::DeleteFile_(const F_Text& Path, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->DeleteFile_(Path);
    }
    F_FeedbackStatus H_FSUtilities::MoveFile_(const F_Text& NewPath, const F_Text& CurrentPath, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->MoveFile_(NewPath, CurrentPath);
    }
    F_FeedbackStatus H_FSUtilities::CopyFile_(const F_Text& DstPath, const F_Text& SrcPath, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->MoveFile_(DstPath, SrcPath);
    }
    F_FeedbackStatus H_FSUtilities::ReadFileBinary(const F_Text& Path, TF_Vector<U8>& OutData, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        
        F_FSFileOpenParams FileOpenParams;
        FileOpenParams.Path = Path;
        FileOpenParams.AccessFlags = E_FSFileAccessFlag::READ;
        TS<A_FSFile> File; 
        ABYTEK_FEEDBACK_STATUS_CHECK(ActualAdapter->OpenFile(FileOpenParams, File))
        ABYTEK_FEEDBACK_STATUS_CHECK(File->ReadBinaryExclusive(OutData));
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_FSUtilities::WriteFileBinary(const F_Text& Path, const TF_Span<const U8>& Data, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        
        F_FSFileOpenParams FileOpenParams;
        FileOpenParams.Path = Path;
        FileOpenParams.AccessFlags = E_FSFileAccessFlag::WRITE;
        TS<A_FSFile> File; 
        ABYTEK_FEEDBACK_STATUS_CHECK(ActualAdapter->OpenFile(FileOpenParams, File))
        ABYTEK_FEEDBACK_STATUS_CHECK(File->WriteBinaryExclusive(Data));
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_FSUtilities::ReadFileString(const F_Text& Path, F_String& OutString, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        
        F_FSFileOpenParams FileOpenParams;
        FileOpenParams.Path = Path;
        FileOpenParams.AccessFlags = E_FSFileAccessFlag::READ;
        TS<A_FSFile> File; 
        ABYTEK_FEEDBACK_STATUS_CHECK(ActualAdapter->OpenFile(FileOpenParams, File))
        ABYTEK_FEEDBACK_STATUS_CHECK(File->ReadStringExclusive(OutString));
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_FSUtilities::WriteFileString(const F_Text& Path, const F_String& String, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        
        F_FSFileOpenParams FileOpenParams;
        FileOpenParams.Path = Path;
        FileOpenParams.AccessFlags = E_FSFileAccessFlag::WRITE;
        TS<A_FSFile> File; 
        ABYTEK_FEEDBACK_STATUS_CHECK(ActualAdapter->OpenFile(FileOpenParams, File))
        ABYTEK_FEEDBACK_STATUS_CHECK(File->WriteStringExclusive(String));
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_FSUtilities::ReadFileText(const F_Text& Path, F_Text& OutText, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        
        F_FSFileOpenParams FileOpenParams;
        FileOpenParams.Path = Path;
        FileOpenParams.AccessFlags = E_FSFileAccessFlag::READ;
        TS<A_FSFile> File; 
        ABYTEK_FEEDBACK_STATUS_CHECK(ActualAdapter->OpenFile(FileOpenParams, File))
        ABYTEK_FEEDBACK_STATUS_CHECK(File->ReadTextExclusive(OutText));
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_FSUtilities::WriteFileText(const F_Text& Path, const F_Text& Text, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        
        F_FSFileOpenParams FileOpenParams;
        FileOpenParams.Path = Path;
        FileOpenParams.AccessFlags = E_FSFileAccessFlag::WRITE;
        TS<A_FSFile> File; 
        ABYTEK_FEEDBACK_STATUS_CHECK(ActualAdapter->OpenFile(FileOpenParams, File))
        ABYTEK_FEEDBACK_STATUS_CHECK(File->WriteTextExclusive(Text));
        return F_FeedbackStatus::MakeSucceeded();
    }

    F_FeedbackStatus H_FSUtilities::CreateDirectory_(const F_Text& Path, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->CreateDirectory_(Path);
    }
    F_FeedbackStatus H_FSUtilities::DeleteDirectory_(const F_Text& Path, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->DeleteDirectory_(Path);
    }
    F_FeedbackStatus H_FSUtilities::MoveDirectory_(const F_Text& NewPath, const F_Text& CurrentPath, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->MoveDirectory_(NewPath, CurrentPath);
    }
    F_FeedbackStatus H_FSUtilities::CopyDirectory_(const F_Text& DstPath, const F_Text& SrcPath, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->CopyDirectory_(DstPath, SrcPath);
    }
    F_FeedbackStatus H_FSUtilities::ListEntries(const F_Text& DirPath, TF_Vector<F_Text>& OutEntries, E_FSEntryType EntryTypes, B8 Recursive, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        
        TF_Vector<F_Text> ItemNames;
        ABYTEK_FEEDBACK_STATUS_CHECK(ActualAdapter->ReadDirectory(DirPath, ItemNames));
        
        TF_Vector<F_Text> Result;
        Result.reserve(ItemNames.size());
        for (const auto& ItemName : ItemNames)
        {
            F_Text ItemPath = DirPath + ABYTEK_TEXT("/") + ItemName;
            F_FSEntryStat ItemStat;
            ABYTEK_FEEDBACK_STATUS_CHECK(ActualAdapter->GetEntryStat(ItemPath, ItemStat));
            if (FlagHasAny(ItemStat.Type, EntryTypes))
            {
                Result.push_back(ItemName);
            }
            if (Recursive && (ItemStat.Type == E_FSEntryType::DIRECTORY))
            {
                TF_Vector<F_Text> ChildItemNames;
                ABYTEK_FEEDBACK_STATUS_CHECK(ListEntries(ItemPath, ChildItemNames, EntryTypes, Recursive, ActualAdapter));
                for (auto& ChildItemName : ChildItemNames)
                {
                    ChildItemName = ItemName + ABYTEK_TEXT("/") + ChildItemName;
                }
                Result.insert(
                    Result.end(),
                    ChildItemNames.begin(),
                    ChildItemNames.end()
                );
            }
        }
        OutEntries = ABYTEK_MOVE(Result);
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_FSUtilities::EnsureDirectory(const F_Text& Path, const TW<A_FSAdapter>& Adapter)
    {
        TW<A_FSAdapter> ActualAdapter = Adapter;
        if (!ActualAdapter)
        {
            ActualAdapter = GetDefaultAdapter();
        }
        return ActualAdapter->EnsureDirectory(Path);
    }
}
