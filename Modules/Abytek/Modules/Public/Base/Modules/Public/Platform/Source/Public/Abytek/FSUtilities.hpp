#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/Path.hpp"
#include "Abytek/FSCOmmon.hpp"
#include "Abytek/FSFile.hpp"
#include "Abytek/FSFile_Standard.hpp"
#include "Abytek/FSAdapter.hpp"
#include "Abytek/FSAdapter_Standard.hpp"


namespace Abytek
{
    class A_FSAdapter;

    struct ABYTEK_BASE_PLATFORM_API H_FSUtilities
    {
        static TW<A_FSAdapter> GetDefaultAdapter();
        
        static B8 Exists(const F_Text& Path, E_FSEntryType EntryTypes = E_FSEntryType::ALL, const TW<A_FSAdapter>& Adapter = {});
        
        static F_FeedbackStatus OpenFile(const F_FSFileOpenParams& OpenParams, TS<A_FSFile>& OutFile, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus CreateFile_(const F_Text& Path, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus DeleteFile_(const F_Text& Path, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus MoveFile_(const F_Text& NewPath, const F_Text& CurrentPath, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus CopyFile_(const F_Text& DstPath, const F_Text& SrcPath, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus ReadFileBinary(const F_Text& Path, TF_Vector<U8>& OutData, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus WriteFileBinary(const F_Text& Path, const TF_Span<const U8>& Data, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus ReadFileString(const F_Text& Path, F_String& OutString, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus WriteFileString(const F_Text& Path, const F_String& String, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus ReadFileText(const F_Text& Path, F_Text& OutText, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus WriteFileText(const F_Text& Path, const F_Text& Text, const TW<A_FSAdapter>& Adapter = {});
        
        static F_FeedbackStatus CreateDirectory_(const F_Text& Path, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus DeleteDirectory_(const F_Text& Path, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus MoveDirectory_(const F_Text& NewPath, const F_Text& CurrentPath, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus CopyDirectory_(const F_Text& DstPath, const F_Text& SrcPath, const TW<A_FSAdapter>& Adapter = {});
        static F_FeedbackStatus ListEntries(const F_Text& DirPath, TF_Vector<F_Text>& OutEntries, E_FSEntryType EntryTypes, B8 Recursive = false, const TW<A_FSAdapter>& Adapter = {});
        template<E_FSEntryType __EntryTypes>
        static F_FeedbackStatus ListEntries(const F_Text& DirPath, TF_Vector<F_Text>& OutEntries, B8 Recursive = false, const TW<A_FSAdapter>& Adapter = {})
        {
            return ListEntries(DirPath, OutEntries, __EntryTypes, Recursive, Adapter);
        }
        static F_FeedbackStatus EnsureDirectory(const F_Text& Path, const TW<A_FSAdapter>& Adapter = {});
    };
}
