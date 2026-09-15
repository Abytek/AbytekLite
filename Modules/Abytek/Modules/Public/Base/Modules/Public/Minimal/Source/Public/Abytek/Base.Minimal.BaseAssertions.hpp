#pragma once

#define ABYTEK_ASSERT_BASE(PREFIX, SUFFIX, ...) \
            if((Abytek::GetLogHiddenCounter() == 0) && !(__VA_ARGS__)) \
                Abytek::F_LogSection( \
                    [](Abytek::F_LogSection& LogSection) \
                    { \
                        throw;  \
                    } \
                ) \
                .GetLogStreamLValue() \
                << Abytek::E_LogType::LT_FATAL \
                << Abytek::F_LogHeader \
                { \
                    ABYTEK_TEXT(ABYTEK_FILE), \
                    ABYTEK_LINE, \
                    ABYTEK_TEXT(#PREFIX), \
                    ABYTEK_TEXT(#SUFFIX), \
                    ABYTEK_TEXT("ASSERT") \
                } \
                << Abytek::F_UpdateLogColor::Field() \
                << "Condition" \
                << Abytek::F_UpdateLogColor::LowLight() \
                << ": " \
                << ABYTEK_TEXT(#__VA_ARGS__) \
                << Abytek::F_NewLine {} \
                << Abytek::F_ResetLogColor {}

#define ABYTEK_ASSERT_BASE_DISABLED(PREFIX, SUFFIX, ...) \
            if constexpr (false) \
                Abytek::F_LogSection( \
                    [](Abytek::F_LogSection& LogSection) \
                    { \
                        throw;  \
                    } \
                ) \
                .GetLogStreamLValue() \
                << Abytek::E_LogType::LT_FATAL \
                << Abytek::F_LogHeader \
                { \
                    ABYTEK_TEXT(ABYTEK_FILE), \
                    ABYTEK_LINE, \
                    ABYTEK_TEXT(#PREFIX), \
                    ABYTEK_TEXT(#SUFFIX), \
                    ABYTEK_TEXT("ASSERT") \
                } \
                << Abytek::F_UpdateLogColor::Field() \
                << "Condition" \
                << Abytek::F_UpdateLogColor::LowLight() \
                << ": " \
                << ABYTEK_TEXT(#__VA_ARGS__) \
                << Abytek::F_NewLine {} \
                << Abytek::F_ResetLogColor {}
