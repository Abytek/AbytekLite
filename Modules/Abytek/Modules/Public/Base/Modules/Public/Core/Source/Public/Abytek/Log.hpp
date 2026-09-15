#pragma once

/**
 * @file Log.hpp
 * @brief Logging system header that provides various logging streams and utilities
 * 
 * This header includes all the necessary logging infrastructure components:
 * - Base logging streams
 * - Standard output streams
 * - Console streams
 * - Transfer streams for network/remote logging
 * - Array-based transfer streams
 */
#include <Abytek/Base.Core.prerequisites.pch.hpp>
#include "Abytek/LogStream.hpp"
#include "Abytek/ConsoleLogStream.hpp"
#include "Abytek/TransferLogStream.hpp"
#include "Abytek/SectionLogStream.hpp"


namespace Abytek
{
    U32 GetLogHiddenCounter();
    U32 IncreaseLogHiddenCounter(U32 Num = 1);
    U32 DecreaseLogHiddenCounter(U32 Num = 1);

    struct F_LogSection final
    {
        std::function<void(F_LogSection& LogStream)> Callback;
        mutable TF_SectionLogStream<1> LogStream;
        
        F_LogSection(std::function<void(F_LogSection& LogStream)>&& InCallback) :
            Callback(ABYTEK_MOVE(InCallback)),
            LogStream(MakeSectionLogStream(ConsoleLogStream))
        {
        }
        ~F_LogSection()
        {
            LogStream.Flush();
            Callback(*this);
        }
        
        auto& GetLogStreamLValue() const
        {
            return (TF_SectionLogStream<1>&)LogStream;
        }
    };
}