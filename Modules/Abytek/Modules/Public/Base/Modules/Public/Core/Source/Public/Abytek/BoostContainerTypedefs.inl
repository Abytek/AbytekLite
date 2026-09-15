/**
 * @file BoostContainerTypedefs.inl
 * @brief Type definitions for Boost containers with default allocator
 * 
 * This file provides concrete type definitions using the templates defined in BoostContainerTemplates.hpp.
 * It includes string conversion functions and container type definitions that use the default allocator.
 */

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BoostContainerTemplates.hpp"
#include "Abytek/HashBase.hpp"
#include "Abytek/UTF8.hpp"

#ifndef ABYTEK_BOOST_CONTAINER_TYPEDEFS_NAMESPACE
#define ABYTEK_BOOST_CONTAINER_TYPEDEFS_NAMESPACE Abytek
#endif


namespace ABYTEK_BOOST_CONTAINER_TYPEDEFS_NAMESPACE
{
#pragma region Boost Container Typedefs
    /**
     * @brief String type definitions using the default allocator
     * @tparam __F_Char Character type
     */
    template<typename __F_Char>
    using TF_String = ContainerTemplates::TF_String<__F_Char, TF_DefaultAllocator<__F_Char>>;
    /// @brief Default string type using F_Char
    using F_String = TF_String<F_Char>;
    /// @brief Text type using F_TextChar
    using F_Text = TF_String<F_TextChar>;
    
    /**
     * @brief Vector template using default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_Vector = ContainerTemplates::TF_Vector<__F_Item, TF_DefaultAllocator<__F_Item>>;
    /**
     * @brief Devector template using default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_DeVector = ContainerTemplates::TF_DeVector<__F_Item, TF_DefaultAllocator<__F_Item>>;
    /**
     * @brief Small vector template using default allocator
     * @tparam __F_Item Item type
     * @tparam __StaticSize Static buffer size
     */
    template<typename __F_Item, Sz __StaticSize>
    using TF_SmallVector = ContainerTemplates::TF_SmallVector<__F_Item, __StaticSize, TF_DefaultAllocator<__F_Item>>;
    /**
     * @brief Stable vector template using default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_StableVector = ContainerTemplates::TF_StableVector<__F_Item, TF_DefaultAllocator<__F_Item>>;
    /**
     * @brief Static vector template
     * @tparam __F_Item Item type
     * @tparam __StaticSize Static buffer size
     * @tparam __F_Options Options type
     */
    template<typename __F_Item, Sz __StaticSize, typename __F_Options = void>
    using TF_StaticVector = ContainerTemplates::TF_StaticVector<__F_Item, __StaticSize, __F_Options>;
    /**
     * @brief Deque template using default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_Deque = ContainerTemplates::TF_Deque<__F_Item, TF_DefaultAllocator<__F_Item>>;
    /**
     * @brief List template using default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_List = ContainerTemplates::TF_List<__F_Item, TF_DefaultAllocator<__F_Item>>;
    /**
     * @brief Singly linked list template using default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_SinglyList = ContainerTemplates::TF_SinglyList<__F_Item, TF_DefaultAllocator<__F_Item>>;
    /**
     * @brief Set template using default allocator
     * @tparam __F_Item Item type
     * @tparam __F_Compare Comparison function type
     */
    template<
        typename __F_Item,
        typename __F_Compare = std::less<__F_Item>
    >
    using TF_Set = ContainerTemplates::TF_Set<
        __F_Item,
        __F_Compare,
        TF_DefaultAllocator<__F_Item>
    >;
    /**
     * @brief Map template using default allocator
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Compare Key comparison function type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare = std::less<__F_Key>
    >
    using TF_Map = ContainerTemplates::TF_Map<
        __F_Key,
        __F_Value,
        __F_Compare,
        TF_DefaultAllocator<std::pair<const __F_Key, __F_Value>>
    >;
    /**
     * @brief MultiMap template using default allocator
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Compare Key comparison function type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare = std::less<__F_Key>
    >
    using TF_MultiMap = ContainerTemplates::TF_MultiMap<
        __F_Key,
        __F_Value,
        __F_Compare,
        TF_DefaultAllocator<std::pair<const __F_Key, __F_Value>>
    >;
    /**
     * @brief Unordered set template using default allocator
     * @tparam __F_Item Item type
     * @tparam __F_Hash Hash function type
     * @tparam __F_Predicate Equality predicate type
     */
    template<
        typename __F_Item,
        typename __F_Hash = Abytek::TF_Hash<__F_Item>,
        typename __F_Predicate = std::equal_to<__F_Item>
    >
    using TF_UnorderedSet = ContainerTemplates::TF_UnorderedSet<
        __F_Item,
        __F_Hash,
        __F_Predicate,
        TF_DefaultAllocator<__F_Item>
    >;
    /**
     * @brief Unordered map template using default allocator
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Hash Hash function type
     * @tparam __F_Predicate Equality predicate type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Hash = Abytek::TF_Hash<__F_Key>,
        typename __F_Predicate = std::equal_to<__F_Key>
    >
    using TF_UnorderedMap = ContainerTemplates::TF_UnorderedMap<
        __F_Key,
        __F_Value,
        __F_Hash,
        __F_Predicate,
        TF_DefaultAllocator<std::pair<const __F_Key, __F_Value>>
    >;
    /**
     * @brief Unordered multimap template using default allocator
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Hash Hash function type
     * @tparam __F_Predicate Equality predicate type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Hash = Abytek::TF_Hash<__F_Key>,
        typename __F_Predicate = std::equal_to<__F_Key>
    >
    using TF_UnorderedMultiMap = ContainerTemplates::TF_UnorderedMultiMap<
        __F_Key,
        __F_Value,
        __F_Hash,
        __F_Predicate,
        TF_DefaultAllocator<std::pair<const __F_Key, __F_Value>>
    >;
    /**
     * @brief Flat set template using default allocator
     * @tparam __F_Item Item type
     * @tparam __F_Compare Comparison function type
     */
    template<
        typename __F_Item,
        typename __F_Compare = std::less<__F_Item>
    >
    using TF_FlatSet = ContainerTemplates::TF_FlatSet<
        __F_Item,
        __F_Compare,
        TF_DefaultAllocator<__F_Item>
    >;
    /**
     * @brief Flat map template using default allocator
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Compare Key comparison function type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare = std::less<__F_Key>
    >
    using TF_FlatMap = ContainerTemplates::TF_FlatMap<
        __F_Key,
        __F_Value,
        __F_Compare,
        TF_DefaultAllocator<std::pair<const __F_Key, __F_Value>>
    >;
    /**
     * @brief Flat multimap template using default allocator
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Compare Key comparison function type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare = std::less<__F_Key>
    >
    using TF_FlatMultiMap = ContainerTemplates::TF_FlatMultiMap<
        __F_Key,
        __F_Value,
        __F_Compare,
        TF_DefaultAllocator<std::pair<const __F_Key, __F_Value>>
    >;
#pragma endregion

#pragma region ToString & ToText
    /**
     * @brief InternalWithCrtAllocator implementation details for string conversion
     */
    namespace InternalWithCrtAllocator 
    {
        /**
         * @brief Helper for string conversion
         * @tparam __F_Char Output character type
         * @tparam __F_Value Input value type
         */
        template<typename __F_Char, typename __F_Value>
        struct TF_ToStringHelper
        {};

        /**
         * @brief Specialization for converting F_TextChar* to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, const F_TextChar*>
        {
            /**
             * @brief Converts F_TextChar* to F_String
             * @param value Input string
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(const F_TextChar* value)
            {
                if (value == nullptr)
                {
                    return {};
                }
                F_Text Text = value;
                auto Utf8Buffer = H_UTF8::From(
                    boost::container::vector<F_TextChar>(
                        Text.begin(),
                        Text.end()
                    )
                );
                return F_String(Utf8Buffer.begin(), Utf8Buffer.end());
            }
        };
        /**
         * @brief Specialization for converting F_Text to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, F_Text>
        {
            /**
             * @brief Converts F_Text to F_String
             * @param value Input text
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(const F_Text& value)
            {
                if (value.length() == 0)
                {
                    return {};
                }
                auto Utf8Buffer = H_UTF8::From(
                    boost::container::vector<F_TextChar>(
                        value.begin(),
                        value.end()
                    )
                );
                return F_String(Utf8Buffer.begin(), Utf8Buffer.end());
            }
        };
        /**
         * @brief Specialization for converting int to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, int>
        {
            /**
             * @brief Converts int to F_String
             * @param value Input integer
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(int value)
            {
                return std::to_string(value).c_str();
            }
        };
        /**
         * @brief Specialization for converting long to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, long>
        {
            /**
             * @brief Converts long to F_String
             * @param value Input long integer
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(long value)
            {
                return std::to_string(value).c_str();
            }
        };
        /**
         * @brief Specialization for converting long long to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, long long>
        {
            /**
             * @brief Converts long long to F_String
             * @param value Input long long integer
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(long long value)
            {
                return std::to_string(value).c_str();
            }
        };
        /**
         * @brief Specialization for converting unsigned to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, unsigned>
        {
            /**
             * @brief Converts unsigned to F_String
             * @param value Input unsigned integer
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(unsigned value)
            {
                return std::to_string(value).c_str();
            }
        };
        /**
         * @brief Specialization for converting unsigned long to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, unsigned long>
        {
            /**
             * @brief Converts unsigned long to F_String
             * @param value Input unsigned long integer
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(unsigned long value)
            {
                return std::to_string(value).c_str();
            }
        };
        /**
         * @brief Specialization for converting unsigned long long to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, unsigned long long>
        {
            /**
             * @brief Converts unsigned long long to F_String
             * @param value Input unsigned long long integer
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(unsigned long long value)
            {
                return std::to_string(value).c_str();
            }
        };
        /**
         * @brief Specialization for converting float to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, float>
        {
            /**
             * @brief Converts float to F_String
             * @param value Input float
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(float value)
            {
                return std::to_string(value).c_str();
            }
        };
        /**
         * @brief Specialization for converting double to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, double>
        {
            /**
             * @brief Converts double to F_String
             * @param value Input double
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(double value)
            {
                return std::to_string(value).c_str();
            }
        };
        /**
         * @brief Specialization for converting long double to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, long double>
        {
            /**
             * @brief Converts long double to F_String
             * @param value Input long double
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(long double value)
            {
                return std::to_string(value).c_str();
            }
        };
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Specialization for converting F_Char* to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, const F_Char*>
        {
            /**
             * @brief Converts F_Char* to F_String
             * @param value Input string
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(const F_Char* value)
            {
                if (value == nullptr)
                {
                    return {};
                }
                return value;
            }
        };
        /**
         * @brief Specialization for converting F_String to F_String
         */
        template<>
        struct TF_ToStringHelper<F_Char, F_String>
        {
            /**
             * @brief Converts F_String to F_String
             * @param value Input string
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(const F_String& value)
            {
                return value;
            }
        };
#endif
        /**
         * @brief Specialization for converting boost::container::basic_string to F_String
         */
        template<typename __F_CharTraits, typename __F_Allocator>
        struct TF_ToStringHelper<F_Char, boost::container::basic_string<F_Char, __F_CharTraits, __F_Allocator>>
        {
            /**
             * @brief Converts boost::container::basic_string to F_String
             * @param value Input string
             * @return Converted string
             */
            static ABYTEK_FORCE_INLINE F_String ToString(const boost::container::basic_string<F_Char, __F_CharTraits, __F_Allocator>& value)
            {
                return value.c_str();
            }
        };

#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Specialization for converting F_Char* to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, const F_Char*>
        {
            /**
             * @brief Converts F_Char* to F_Text
             * @param value Input string
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(const F_Char* value)
            {
                if (value == nullptr)
                {
                    return {};
                }
                F_String String = value;
                auto TextBuffer = H_UTF8::ToText(
                    boost::container::vector<F_Char>(
                        String.begin(),
                        String.end()
                    )
                );
                return F_Text(TextBuffer.begin(), TextBuffer.end());
            }
        };
        /**
         * @brief Specialization for converting F_String to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, F_String>
        {
            /**
             * @brief Converts F_String to F_Text
             * @param value Input string
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(const F_String& value)
            {
                if (value.length() == 0)
                {
                    return {};
                }
                auto TextBuffer = H_UTF8::ToText(
                    boost::container::vector<F_Char>(
                        value.begin(),
                        value.end()
                    )
                );
                return F_Text(TextBuffer.begin(), TextBuffer.end());
            }
        };
        /**
         * @brief Specialization for converting int to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, int>
        {
            /**
             * @brief Converts int to F_Text
             * @param value Input integer
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(int value)
            {
                return TF_ToStringHelper<F_TextChar, F_String>::ToString(
                    TF_ToStringHelper<F_Char, int>::ToString(value)
                );
            }
        };
        /**
         * @brief Specialization for converting long to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, long>
        {
            /**
             * @brief Converts long to F_Text
             * @param value Input long integer
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(long value)
            {
                return TF_ToStringHelper<F_TextChar, F_String>::ToString(
                    TF_ToStringHelper<F_Char, long>::ToString(value)
                );
            }
        };
        /**
         * @brief Specialization for converting long long to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, long long>
        {
            /**
             * @brief Converts long long to F_Text
             * @param value Input long long integer
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(long long value)
            {
                return TF_ToStringHelper<F_TextChar, F_String>::ToString(
                    TF_ToStringHelper<F_Char, long long>::ToString(value)
                );
            }
        };
        /**
         * @brief Specialization for converting unsigned to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, unsigned>
        {
            /**
             * @brief Converts unsigned to F_Text
             * @param value Input unsigned integer
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(unsigned value)
            {
                return TF_ToStringHelper<F_TextChar, F_String>::ToString(
                    TF_ToStringHelper<F_Char, unsigned>::ToString(value)
                );
            }
        };
        /**
         * @brief Specialization for converting unsigned long to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, unsigned long>
        {
            /**
             * @brief Converts unsigned long to F_Text
             * @param value Input unsigned long integer
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(unsigned long value)
            {
                return TF_ToStringHelper<F_TextChar, F_String>::ToString(
                    TF_ToStringHelper<F_Char, unsigned long>::ToString(value)
                );
            }
        };
        /**
         * @brief Specialization for converting unsigned long long to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, unsigned long long>
        {
            /**
             * @brief Converts unsigned long long to F_Text
             * @param value Input unsigned long long integer
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(unsigned long long value)
            {
                return TF_ToStringHelper<F_TextChar, F_String>::ToString(
                    TF_ToStringHelper<F_Char, unsigned long long>::ToString(value)
                );
            }
        };
        /**
         * @brief Specialization for converting float to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, float>
        {
            /**
             * @brief Converts float to F_Text
             * @param value Input float
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(float value)
            {
                return TF_ToStringHelper<F_TextChar, F_String>::ToString(
                    TF_ToStringHelper<F_Char, float>::ToString(value)
                );
            }
        };
        /**
         * @brief Specialization for converting double to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, double>
        {
            /**
             * @brief Converts double to F_Text
             * @param value Input double
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(double value)
            {
                return TF_ToStringHelper<F_TextChar, F_String>::ToString(
                    TF_ToStringHelper<F_Char, double>::ToString(value)
                );
            }
        };
        /**
         * @brief Specialization for converting long double to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, long double>
        {
            /**
             * @brief Converts long double to F_Text
             * @param value Input long double
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(long double value)
            {
                return TF_ToStringHelper<F_TextChar, F_String>::ToString(
                    TF_ToStringHelper<F_Char, long double>::ToString(value)
                );
            }
        };
        /**
         * @brief Specialization for converting F_TextChar* to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, const F_TextChar*>
        {
            /**
             * @brief Converts F_TextChar* to F_Text
             * @param value Input text
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(const F_TextChar* value)
            {
                if (value == nullptr)
                {
                    return {};
                }
                return value;
            }
        };
        /**
         * @brief Specialization for converting F_Text to F_Text
         */
        template<>
        struct TF_ToStringHelper<F_TextChar, F_Text>
        {
            /**
             * @brief Converts F_Text to F_Text
             * @param value Input text
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(const F_Text& value)
            {
                return value;
            }
        };
#endif
        /**
         * @brief Specialization for converting boost::container::basic_string to F_Text
         */
        template<typename __F_CharTraits, typename __F_Allocator>
        struct TF_ToStringHelper<F_Text, boost::container::basic_string<F_TextChar, __F_CharTraits, __F_Allocator>>
        {
            /**
             * @brief Converts boost::container::basic_string to F_Text
             * @param value Input text
             * @return Converted text
             */
            static ABYTEK_FORCE_INLINE F_Text ToString(const boost::container::basic_string<F_TextChar, __F_CharTraits, __F_Allocator>& value)
            {
                return value.c_str();
            }
        };
    }
    /**
     * @brief Converts a value to a string
     * @tparam __F_Char Output character type
     * @tparam __F_Value Input value type
     * @param value Input value
     * @return Converted string
     */
    template<typename __F_Char, typename __F_Value>
    TF_String<__F_Char> ToString(const __F_Value& value)
    {
        return InternalWithCrtAllocator::TF_ToStringHelper<__F_Char, __F_Value>::ToString(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(int value)
    {
        return ToString<F_Char, int>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(long value)
    {
        return ToString<F_Char, long>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(long long value)
    {
        return ToString<F_Char, long long>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(unsigned value)
    {
        return ToString<F_Char, unsigned>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(unsigned long value)
    {
        return ToString<F_Char, unsigned long>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(unsigned long long value)
    {
        return ToString<F_Char, unsigned long long>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(float value)
    {
        return ToString<F_Char, float>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(double value)
    {
        return ToString<F_Char, double>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(long double value)
    {
        return ToString<F_Char, long double>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(const F_Text& value)
    {
        return ToString<F_Char, F_Text>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(const F_TextChar* value)
    {
        return ToString<F_Char, const F_TextChar*>(value);
    }
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
    ABYTEK_FORCE_INLINE F_String ToString(const F_String& value)
    {
        return ToString<F_Char, F_String>(value);
    }
    ABYTEK_FORCE_INLINE F_String ToString(const F_Char* value)
    {
        return ToString<F_Char, const F_Char*>(value);
    }
#endif
    ABYTEK_FORCE_INLINE F_Text ToText(int value)
    {
        return ToString<F_TextChar, int>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(long value)
    {
        return ToString<F_TextChar, long>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(long long value)
    {
        return ToString<F_TextChar, long long>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(unsigned value)
    {
        return ToString<F_TextChar, unsigned>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(unsigned long value)
    {
        return ToString<F_TextChar, unsigned long>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(unsigned long long value)
    {
        return ToString<F_TextChar, unsigned long long>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(float value)
    {
        return ToString<F_TextChar, float>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(double value)
    {
        return ToString<F_TextChar, double>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(long double value)
    {
        return ToString<F_TextChar, long double>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(const F_Text& value)
    {
        return ToString<F_TextChar, F_Text>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(const F_TextChar* value)
    {
        return ToString<F_TextChar, const F_TextChar*>(value);
    }
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
    ABYTEK_FORCE_INLINE F_Text ToText(const F_String& value)
    {
        return ToString<F_TextChar, F_String>(value);
    }
    ABYTEK_FORCE_INLINE F_Text ToText(const F_Char* value)
    {
        return ToString<F_TextChar, const F_Char*>(value);
    }
#endif
    ABYTEK_FORCE_INLINE F_Text ToText(const F_NewLine&)
    {
        return ABYTEK_TEXT("\n");
    }
#pragma endregion
}


#undef ABYTEK_BOOST_CONTAINER_TYPEDEFS_NAMESPACE