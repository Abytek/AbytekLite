#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Namespace containing internal implementation details for type hash code generation
     */
    // \cond INTERNAL
    namespace Internal::TypeHashCode
    {
        /**
         * @brief Computes a hash code for a type at compile-time
         * @tparam __F The type to generate a hash code for
         * @return A 64-bit hash value representing the type
         */
        template<typename __F>
        constexpr U64 TypeHashCode()
        {
            U64 Result = 0xcbf29ce484222325LL;
            
#ifdef _MSC_VER
#define CURRENT_FUNCTION_SIGNATURE __FUNCSIG__
#else
#define CURRENT_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif

            for(const auto C : CURRENT_FUNCTION_SIGNATURE)
            {
                Result = (Result ^ U64(C)) * 0x100000001b3LL;
            }

            return Result;
#undef CURRENT_FUNCTION_SIGNATURE
        }
    }
    // \endcond

    /**
     * @brief A compile-time hash code for a given type
     * @tparam __F The type to get a hash code for
     */
    template<typename __F>
    constexpr U64 TypeHashCode = Internal::TypeHashCode::TypeHashCode<__F>();


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /**
     * @brief Namespace containing internal implementation details for type name retrieval
     */
    // \cond INTERNAL
    namespace Internal
    {
        /**
         * @brief Converts a string_view to a std::array at compile time
         * @param str The string view to convert
         * @param Sequence of indices for array construction
         * @return Array containing the characters of the string
         */
        template <std::size_t...Idxs>
        constexpr auto SubStringAsArray(std::string_view str, std::index_sequence<Idxs...>)
        {
            return std::array{str[Idxs]...};
        }

        /**
         * @brief Extracts the type name from compiler-specific function name information
         * @tparam T The type to get the name for
         * @return Array containing the type name
         */
        template <typename T>
        constexpr auto TypeNameArray()
        {
#if defined(__clang__)
            constexpr auto prefix   = std::string_view{"[T = "};
            constexpr auto suffix   = std::string_view{"]"};
            constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(__GNUC__)
            constexpr auto prefix   = std::string_view{"with T = "};
            constexpr auto suffix   = std::string_view{"]"};
            constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(_MSC_VER)
            constexpr auto prefix   = std::string_view{"TypeNameArray<"};
            constexpr auto suffix   = std::string_view{">(void)"};
            constexpr auto function = std::string_view{__FUNCSIG__};
#else
            # error Unsupported compiler
#endif

            constexpr auto start = function.find(prefix) + prefix.size();
            constexpr auto end = function.rfind(suffix);

            constexpr auto name = function.substr(start, (end - start));
            return SubStringAsArray(name, std::make_index_sequence<name.size()>{});
        }

        /**
         * @brief Holder for the type name array value
         * @tparam T The type to hold the name for
         */
        template <typename T>
        struct TypeNameHolder {
            static inline constexpr auto value = TypeNameArray<T>();
        };

        /**
         * @brief Gets the name of a type as a string_view
         * @tparam T The type to get the name for
         * @return String view containing the type name
         */
        template <typename T>
        constexpr auto type_name() -> std::string_view
        {
            constexpr auto& value = TypeNameHolder<T>::value;
            return std::string_view{value.data(), value.size()};
        }

        /**
         * @brief Type flag for character conversion
         * @tparam __F_Char Character type
         */
        template<typename __F_Char>
        struct F_TypeNameCharFlag {};

        /**
         * @brief Converts a string to a string (identity function)
         * @param str The string to convert
         * @return The same string
         */
        ABYTEK_FORCE_INLINE std::string TypeNameCharConvert(std::string&& str, F_TypeNameCharFlag<char>){

            return str;
        }

        /**
         * @brief Converts a wide string to a narrow string
         * @param str The wide string to convert
         * @return Narrow string version
         */
        ABYTEK_FORCE_INLINE std::string TypeNameCharConvert(std::wstring&& str, F_TypeNameCharFlag<char>){

            std::string value;
            value.resize(str.size());

            for(Sz i = 0; i < str.length(); ++i)
                value[i] = static_cast<char>(str[i]);

            return value;
        }

        /**
         * @brief Returns a wide string (identity function)
         * @param str The wide string
         * @return The same wide string
         */
        ABYTEK_FORCE_INLINE std::wstring TypeNameCharConvert(std::wstring&& str, F_TypeNameCharFlag<wchar_t>){

            return str;
        }

        /**
         * @brief Converts a narrow string to a wide string
         * @param str The narrow string to convert
         * @return Wide string version
         */
        ABYTEK_FORCE_INLINE std::wstring TypeNameCharConvert(std::string&& str, F_TypeNameCharFlag<wchar_t>){

            std::wstring value;
            value.resize(str.size());

            for(Sz i = 0; i < str.length(); ++i)
                value[i] = static_cast<wchar_t>(str[i]);

            return value;
        }
    }
    // \endcond

    /**
     * @brief Gets the full type name as a C-string
     * @tparam __F The type to get the name for
     * @tparam __F_Char The character type for the returned string (default: char)
     * @return C-string containing the type name
     */
    template<typename __F, typename __F_Char = char>
    const __F_Char* TypeFullName() {

        static std::basic_string<__F_Char> static_str = (std::basic_stringstream<__F_Char>() << Internal::type_name<__F>()).str();

        return static_str.c_str();
    }
}