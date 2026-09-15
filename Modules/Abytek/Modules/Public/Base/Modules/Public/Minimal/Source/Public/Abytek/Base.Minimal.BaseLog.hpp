#pragma once

#define ABYTEK_LOG_BASE(PREFIX, SUFFIX, ...) \
                if (Abytek::GetLogHiddenCounter() == 0) \
                    Abytek::F_LogSection( \
                        [](Abytek::F_LogSection& LogSection) \
                        { \
                            if (__VA_ARGS__ == Abytek::E_LogType::LT_FATAL) throw;  \
                        } \
                    ) \
                    .GetLogStreamLValue() \
                    << __VA_ARGS__ \
                    << Abytek::F_LogHeader \
                    { \
                        ABYTEK_TEXT(ABYTEK_FILE),\
                        ABYTEK_LINE,\
                        ABYTEK_TEXT(#PREFIX),\
                        ABYTEK_TEXT(#SUFFIX)\
                    }

#define ABYTEK_LOG_BASE_DISABLED(PREFIX, SUFFIX, ...) \
                if constexpr (false) \
                    Abytek::F_LogSection( \
                        [](Abytek::F_LogSection& LogSection) \
                        { \
                            if (__VA_ARGS__ == Abytek::E_LogType::LT_FATAL) throw;  \
                        } \
                    ) \
                    .GetLogStreamLValue() \
                    << __VA_ARGS__ \
                    << Abytek::F_LogHeader \
                    { \
                        ABYTEK_TEXT(ABYTEK_FILE),\
                        ABYTEK_LINE,\
                        ABYTEK_TEXT(#PREFIX),\
                        ABYTEK_TEXT(#SUFFIX)\
                    }