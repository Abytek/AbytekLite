#pragma once

#include "Abytek/BuildSharedDefines.hpp"
#include "Abytek/Base.Minimal.config.hpp"
#include "Abytek/Base.Minimal.Platforms.hpp"
#include "Abytek/Base.Minimal.LogDefines.hpp"
#include "Abytek/Base.Minimal.AssertionDefines.hpp"
#include "Abytek/Base.Minimal.UnitTestDefines.hpp"

#pragma region Boost Headers
#include "boost/any.hpp"
#include "boost/core/default_allocator.hpp"
#include "boost/utility.hpp"
#include "boost/function.hpp"
#include "boost/atomic.hpp"
#include "boost/variant2/variant.hpp"
#include "boost/optional.hpp"
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"
#include "boost/array.hpp"
#include "boost/random.hpp"
#include "boost/core/span.hpp"
#include "boost/context/fiber.hpp"
#if !defined(BOOST_DISABLE_THREADS)
#include "boost/thread.hpp"
#endif
#include <boost/integer.hpp>
#include <boost/algorithm/string.hpp>
#include "boost/iostreams/stream.hpp"
#include "boost/chrono.hpp"
#include "boost/container/flat_map.hpp"
#include "boost/container/flat_set.hpp"
#include "boost/container/list.hpp"
#include "boost/container/map.hpp"
#include "boost/container/vector.hpp"
#include "boost/container/deque.hpp" 
#include "boost/container/string.hpp"
#include "boost/container/static_vector.hpp"
#include "boost/container/stable_vector.hpp"
#include "boost/container/small_vector.hpp"
#include "boost/container/slist.hpp"
#include "boost/container/set.hpp"
#include "boost/container/scoped_allocator.hpp"
#include "boost/container/scoped_allocator_fwd.hpp"
#include "boost/container/node_allocator.hpp"
#include "boost/container/new_allocator.hpp"
#include "boost/container/allocator.hpp"
#include "boost/container/adaptive_pool.hpp"
#include "boost/range/algorithm.hpp"
#include "boost/json.hpp"
#include "boost/locale.hpp"
#pragma endregion

#pragma region C++ STL Headers
#include <charconv>
#include <iostream>
#include <algorithm>
#include <array>
#include <cassert>
#include <vector>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <thread>
#include <atomic>
#include <mutex>
#include <sstream>
#include <functional>
#include <random>
#include <cmath>
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <condition_variable>
#include <chrono>
#include <limits>
#include <string.h>

#if _HAS_CXX20
#include <coroutine>
#include <string_view>
#endif
#pragma endregion


#pragma region Platform Specific Instruction Extension Headers
#ifdef ABYTEK_ENABLE_SSE
    #ifdef ABYTEK_PROCESSOR_X86_64
    #include <xmmintrin.h>
    #endif
    #ifndef ABYTEK_ENABLE_SIMD
        #define ABYTEK_ENABLE_SIMD
    #endif
#endif

#ifdef ABYTEK_ENABLE_SSE2
    #ifdef ABYTEK_PROCESSOR_X86_64
    #include <emmintrin.h>
    #endif
    #ifndef ABYTEK_ENABLE_SIMD
        #define ABYTEK_ENABLE_SIMD
    #endif
#endif

#ifdef ABYTEK_ENABLE_SSE3
    #ifdef ABYTEK_PROCESSOR_X86_64
    #include <pmmintrin.h>
    #endif
    #ifndef ABYTEK_ENABLE_SIMD
        #define ABYTEK_ENABLE_SIMD
    #endif
#endif

#ifdef ABYTEK_ENABLE_SSE4
    #ifdef ABYTEK_PROCESSOR_X86_64
    #include <smmintrin.h>
    #endif
    #ifndef ABYTEK_ENABLE_SIMD
        #define ABYTEK_ENABLE_SIMD
    #endif
#endif

#if defined(ABYTEK_ENABLE_AVX) || defined(ABYTEK_ENABLE_AVX2) || defined(ABYTEK_ENABLE_FMA3)
    #ifdef ABYTEK_PROCESSOR_X86_64
    #include <immintrin.h>
    #endif
    #ifndef ABYTEK_ENABLE_SIMD
        #define ABYTEK_ENABLE_SIMD
    #endif
#endif

#ifdef ABYTEK_ENABLE_ARM_NEON
    #ifdef ABYTEK_PROCESSOR_ARM
    #include <arm_neon.h>
    #endif
    #ifndef ABYTEK_ENABLE_SIMD
        #define ABYTEK_ENABLE_SIMD
    #endif
    #endif
#pragma endregion


#define ABYTEK_PRIVATE_KEYWORD private:
#define ABYTEK_PROTECTED_KEYWORD protected:
#define ABYTEK_PUBLIC_KEYWORD public:
#define ABYTEK_EXPAND(...) __VA_ARGS__
#define ABYTEK_EXPAND_PARAMS(...) ABYTEK_EXPAND(ABYTEK_EXPAND __VA_ARGS__)
#define ABYTEK_GLUE_HELPER(x, y) x ## y
#define ABYTEK_GLUE(x, y) ABYTEK_GLUE_HELPER(x, y)

#define ABYTEK_CSTR(...) __VA_ARGS__
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
#define ABYTEK_WCSTR(...) ABYTEK_GLUE(L, __VA_ARGS__)
#else
#define ABYTEK_WCSTR(...) __VA_ARGS__
#endif
#define ABYTEK_CSTR_ML(...) #__VA_ARGS__
#define ABYTEK_WCSTR_ML(...) ABYTEK_WCSTR(#__VA_ARGS__)

#define ABYTEK_WRAPPED_ARGS_TO_CSTR_INTERNAL(Args) ABYTEK_CSTR_ML(Args)
#define ABYTEK_WRAPPED_ARGS_TO_CSTR(Args) ABYTEK_WRAPPED_ARGS_TO_CSTR_INTERNAL(ABYTEK_EXPAND Args)
#define ABYTEK_MA(...) __VA_ARGS__
#define ABYTEK_MA_IGNORE(...)
/* Should not use for future features */
#define ABYTEK_MP_GROUP(...) __VA_ARGS__


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_BASE_THIS(), ABYTEK_BASE_THIS_CONST(),...
////////////////////////////////////////////////////////////////////////////////////
#define ABYTEK_BASE_THIS() ((F_Base*)this)
#define ABYTEK_BASE_THIS_CONST() ((const F_Base*)this)
#define ABYTEK_BASE_P(x) ((F_Base*)x)
#define ABYTEK_BASE_P_CONST(x) ((const F_Base*)x)
#define ABYTEK_BASE_R(x) ((F_Base&)x)
#define ABYTEK_BASE_R_CONST(x) ((const F_Base&)x)


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_TEXT(CStr) macro
////////////////////////////////////////////////////////////////////////////////////
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
#define ABYTEK_TEXT(...) ABYTEK_GLUE(L, __VA_ARGS__)
#else
#define ABYTEK_TEXT(...) __VA_ARGS__
#endif
#define ABYTEK_TEXT_ML(...) ABYTEK_TEXT(#__VA_ARGS__)


////////////////////////////////////////////////////////////////////////////////////
//  Abytek module related macros for non-abytek targets
////////////////////////////////////////////////////////////////////////////////////
#ifndef ABYTEK_MODULE
#define ABYTEK_MODULE ""
#endif
#ifndef ABYTEK_MODULE_DIRECTORY
#define ABYTEK_MODULE_DIRECTORY ""
#endif


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_MODULE_TEXT macro
////////////////////////////////////////////////////////////////////////////////////
#define ABYTEK_MODULE_TEXT ABYTEK_TEXT(ABYTEK_MODULE)

////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_MODULE_DIRECTORY_TEXT macro
////////////////////////////////////////////////////////////////////////////////////
#define ABYTEK_MODULE_DIRECTORY_TEXT ABYTEK_TEXT(ABYTEK_MODULE_DIRECTORY)


////////////////////////////////////////////////////////////////////////////////////
//  Debug/release enable/disable utility macros
////////////////////////////////////////////////////////////////////////////////////
#ifndef NDEBUG
#define ABYTEK_DEBUG
#define ABYTEK_ENABLE_IF_DEBUG(...) __VA_ARGS__
#define ABYTEK_ENABLE_IF_RELEASE(...)
#else
#define ABYTEK_RELEASE
#define ABYTEK_ENABLE_IF_DEBUG(...)
#define ABYTEK_ENABLE_IF_RELEASE(...) __VA_ARGS__
#endif

#if defined(ABYTEK_INTERNAL_CONFIG_DEBUG) || defined(ABYTEK_INTERNAL_CONFIG_REL_WITH_DEB_INFO)
#if !defined(ABYTEK_FORCE_DISABLE_DEBUG_INFO)
#define ABYTEK_DEBUG_INFO
#endif
#endif


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_LINE, ABYTEK_FILE, ABYTEK_FUNCTION macros
////////////////////////////////////////////////////////////////////////////////////
#define ABYTEK_LINE __LINE__
#define ABYTEK_FILE __FILE__
#if defined(_MSC_VER)
#define ABYTEK_FUNCTION __FUNCTION__
#elif defined(__GNUC__) || defined(__clang__)
#define ABYTEK_FUNCTION __PRETTY_FUNCTION__
#else
#error "Unknown compiler; can't define ABYTEK_FUNCTION"
#endif


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_DISABLE_COPY(...) macros
////////////////////////////////////////////////////////////////////////////////////
#define ABYTEK_DISABLE_COPY(...) \
            __VA_ARGS__(const __VA_ARGS__&) = delete;\
            __VA_ARGS__& operator = (const __VA_ARGS__&) = delete;


////////////////////////////////////////////////////////////////////////////////////
//  Call convention macros
////////////////////////////////////////////////////////////////////////////////////
#ifdef ABYTEK_ENABLE_SIMD
#if defined(_MSC_VER) || (defined(__clang__) && defined(ABYTEK_PLATFORM_WINDOWS))
#define ABYTEK_VECTOR_CALL __vectorcall
#else
#define ABYTEK_VECTOR_CALL
#endif
#else
#define ABYTEK_VECTOR_CALL
#endif

/*#if defined(__clang__)
#define ABYTEK_FAST_CALL
#elif defined(__GNUC__)
#define ABYTEK_FAST_CALL __attribute__((fastcall))
#elif defined(_MSC_VER)
#define ABYTEK_FAST_CALL __fastcall
#else
#error "Unknown compiler; can't define ABYTEK_FAST_CALL"
#endif*/

#define ABYTEK_FAST_CALL


////////////////////////////////////////////////////////////////////////////////////
//  Warning disable macros
////////////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
// For Microsoft Visual Studio
#define ABYTEK_DISABLE_ALL_WARNINGS_PUSH __pragma(warning( push, 0 ))
#define ABYTEK_DISABLE_ALL_WARNINGS_POP  __pragma(warning( pop ))
#elif defined(__clang__)
// For Clang
#define ABYTEK_DISABLE_ALL_WARNINGS_PUSH _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Weverything\"")
#define ABYTEK_DISABLE_ALL_WARNINGS_POP  _Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
// For GCC
#define ABYTEK_DISABLE_ALL_WARNINGS_PUSH //_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wall\"") _Pragma("GCC diagnostic ignored \"-Wextra\"")
#define ABYTEK_DISABLE_ALL_WARNINGS_POP  //_Pragma("GCC diagnostic pop")
#else
#error "Unknown compiler; can't define ABYTEK_DISABLE_ALL_WARNINGS_PUSH and ABYTEK_DISABLE_ALL_WARNINGS_POP"
#define ABYTEK_DISABLE_ALL_WARNINGS_PUSH
#define ABYTEK_DISABLE_ALL_WARNINGS_POP
#endif


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_FORCE_INLINE macro
////////////////////////////////////////////////////////////////////////////////////
#if defined(__GNUC__) || defined(__clang__)
// #define ABYTEK_FORCE_INLINE __attribute__((always_inline)) inline
#define ABYTEK_FORCE_INLINE inline
#elif defined(_MSC_VER)
#define ABYTEK_FORCE_INLINE __forceinline
#else
#error "Unknown compiler; can't define ABYTEK_FORCE_INLINE"
#endif


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_FORWARD(...), ABYTEK_MOVE(...) macro
////////////////////////////////////////////////////////////////////////////////////
#define ABYTEK_FORWARD(...) (\
                boost::forward<\
                    decltype(__VA_ARGS__)\
                >(__VA_ARGS__)\
            )
#define ABYTEK_MOVE(...) boost::move(__VA_ARGS__)


////////////////////////////////////////////////////////////////////////////////////
//  Alignment macros
////////////////////////////////////////////////////////////////////////////////////
namespace Abytek
{
    namespace Internal::IsOneOf
    {
        template<typename T, typename... Ts>
        struct TH : std::false_type {};

        template<typename T, typename U, typename... Ts>
        struct TH<T, U, Ts...>
            : std::conditional_t<
                std::is_same_v<T, U>,
                std::true_type,
                TH<T, Ts...>>
        {};
    }
    
    template<typename T, typename... Ts>
    constexpr bool IsOneOf()
    {
        return Internal::IsOneOf::TH<T, Ts...>::value;
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  Alignment macros
////////////////////////////////////////////////////////////////////////////////////
#if defined(__GNUC__) || defined(__clang__)
#define ABYTEK_ALIGN(...) __attribute__ ((aligned(__VA_ARGS__)))
#elif defined(_MSC_VER)
#define ABYTEK_ALIGN(...) __declspec(align(__VA_ARGS__))
#else
#error "Unknown compiler; can't define ABYTEK_ALIGN(N)"
#endif

namespace Abytek
{
    namespace Internal
    {
        template<bool __IsVoid, typename __F>
        struct TH_Alignof;
        template<typename __F>
        struct TH_Alignof<false, __F>
        {
#if defined(__GNUC__) || defined(__clang__)
            static constexpr size_t Value = __alignof__(__F);
#elif defined(_MSC_VER)
            static constexpr size_t Value = __alignof(__F);
#else
#error "Unknown compiler; can't define ABYTEK_ALIGNOF(T)"
#endif
        };
        template<>
        struct TH_Alignof<true, void>
        {
            static constexpr size_t Value = ABYTEK_PLATFORM_MIN_MALLOC_ALIGNMENT;
        };
    }
}

#define ABYTEK_ALIGNOF(X) Abytek::Internal::TH_Alignof<std::is_same_v<X, void>, X>::Value


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_DEPRECATE(foo, ...) macro
////////////////////////////////////////////////////////////////////////////////////
#if defined(__GNUC__) || defined(__clang__)
#define ABYTEK_DEPRECATE(foo, ...) foo __attribute__((deprecated(__VA_ARGS__)))
#elif defined(_MSC_VER)
#define ABYTEK_DEPRECATE(foo, ...) __declspec(deprecated(__VA_ARGS__)) foo
#else
#error "Unknown compiler; can't define ABYTEK_DEPRECATE(foo, ...)"
#endif


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_DEPRECATE(foo, ...) macro
////////////////////////////////////////////////////////////////////////////////////
#define ABYTEK_REQUIRES(...) , std::enable_if_t<(__VA_ARGS__), int> = 0
#define ABYTEK_REQUIRES_FRIEND(...) , std::enable_if_t<(__VA_ARGS__), int>
#define ABYTEK_REQUIRES_DEFINITION(...) , std::enable_if_t<(__VA_ARGS__), int>
#define ABYTEK_REQUIRES_EXPR(...) (!std::is_same_v<decltype(__VA_ARGS__), long long************>)


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_DISABLE_COPY(...) macros
////////////////////////////////////////////////////////////////////////////////////

#define ABYTEK_DISABLE_COPY(...) \
			__VA_ARGS__(const __VA_ARGS__&) = delete;\
			__VA_ARGS__& operator = (const __VA_ARGS__&) = delete;


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_OFFSETOF(...) macro
////////////////////////////////////////////////////////////////////////////////////
#define ABYTEK_OFFSETOF(...) offsetof(__VA_ARGS__)


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_DEFER(...) macro
////////////////////////////////////////////////////////////////////////////////////
// \cond INTERNAL
namespace Abytek::Internal
{
    struct F_Defer
    {
        boost::container::small_vector<uint8_t, 64> Binary;
        void (*CallerP)(void*);

        template<typename __F>
        ABYTEK_FORCE_INLINE F_Defer(__F&& Functor)
        {
            using F = std::remove_const_t<std::remove_reference_t<__F>>;
            Binary.resize(sizeof(F));
            new (Binary.data()) F(ABYTEK_FORWARD(Functor));
            
            CallerP = [](void* P)
            {
                (*(F*)P)();  
                std::destroy_at((F*)P);
            };
        }
        ABYTEK_FORCE_INLINE ~F_Defer()
        {
            CallerP(Binary.data());
        }
    };
}
// \endcond
#define ABYTEK_DEFER_OBJECT_NAME(...) ABYTEK_GLUE(___Abytek_Defer_##__VA_ARGS__, ABYTEK_GLUE(__at_line_, __LINE__))
#define ABYTEK_DEFER_ADVANCED(Suffix, ...) Abytek::Internal::F_Defer ABYTEK_DEFER_OBJECT_NAME(Suffix) = [__VA_ARGS__]()
#define ABYTEK_DEFER(...) ABYTEK_DEFER_ADVANCED(, __VA_ARGS__)

#pragma region Primitive type limits
#define ABYTEK_U64_MAX 0xFFFFFFFFFFFFFFFF
#define ABYTEK_U64_MIN 0
#define ABYTEK_U32_MAX 0xFFFFFFFF
#define ABYTEK_U32_MIN 0
#define ABYTEK_U16_MAX 0xFFFF
#define ABYTEK_U16_MIN 0
#define ABYTEK_U8_MAX 0xFF
#define ABYTEK_U8_MIN 0

#define ABYTEK_I64_MAX 0x7FFFFFFFFFFFFFFF
#define ABYTEK_I64_MIN (-0x8000000000000000)
#define ABYTEK_I32_MAX 0x7FFFFFFF
#define ABYTEK_I32_MIN (-0x80000000)
#define ABYTEK_I16_MAX 0x7FFF
#define ABYTEK_I16_MIN (-0x8000)
#define ABYTEK_I8_MAX 0x7F
#define ABYTEK_I8_MIN (-0x80)

#define ABYTEK_F32_MAX boost::numeric_limits<float>::max();
#define ABYTEK_F32_MIN boost::numeric_limits<float>::min();
#define ABYTEK_F64_MAX boost::numeric_limits<double>::max();
#define ABYTEK_F64_MIN boost::numeric_limits<float>::max();
#pragma endregion

#define ABYTEK_MIN_MEMORY_ALIGNMENT ABYTEK_PLATFORM_MIN_MALLOC_ALIGNMENT


/**
*    The main namespace
*/
namespace Abytek
{
    /** 8bit unsigned integer. */
    using U8 = uint8_t;
    /** 16bit unsigned integer. */
    using U16 = uint16_t;
    /** 32bit unsigned integer. */
    using U32 = uint32_t;
    /** 64bit unsigned integer. */
    using U64 = uint64_t;

    /** 8bit signed integer. */
    using I8 = int8_t;
    /** 16bit signed integer. */
    using I16 = int16_t;
    /** 32bit signed integer. */
    using I32 = int32_t;
    /** 64bit signed integer. */
    using I64 = int64_t;

    /** 32bit floating-point type. */
    using F32 = float_t;
    /** 64bit floating-point type. */
    using F64 = double_t;

    /** 8bit boolean. */
    using B8 = bool;

    /** char. */
    using F_Char = char;
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
    using F_TextChar = wchar_t;
#else
    using F_TextChar = char;
#endif

    /** the same as size_t. */
    using Sz = size_t;

    /** the same as ptrdiff_t. */
    using PDiff = ptrdiff_t;

    /** 8bit atomic unsigned integer. */
    using AU8 = boost::atomic<U8>;
    /** 16bit atomic unsigned integer. */
    using AU16 = boost::atomic<U16>;
    /** 32bit atomic unsigned integer. */
    using AU32 = boost::atomic<U32>;
    /** 64bit atomic unsigned integer. */
    using AU64 = boost::atomic<U64>;

    /** 8bit atomic signed integer. */
    using AI8 = boost::atomic<I8>;
    /** 16bit atomic signed integer. */
    using AI16 = boost::atomic<I16>;
    /** 32bit atomic signed integer. */
    using AI32 = boost::atomic<I32>;
    /** 64bit atomic signed integer. */
    using AI64 = boost::atomic<I64>;

    /** 32bit atomic floating-point type. */
    using AF32 = boost::atomic<F32>;
    /** 64bit atomic floating-point type. */
    using AF64 = boost::atomic<F64>;

    /** 8bit atomic boolean. */
    using AB8 = boost::atomic<B8>;

    /** atomic char. */
    using F_AtomicChar = boost::atomic<F_Char>;

    /** atomic text char. */
    using F_AtomicTextChar = boost::atomic<F_TextChar>;

    /** 8bit atomic flag. */
    using F_AtomicFlag = boost::atomic_flag;

    /** the same as size_t but is atomic. */
    using ASz = boost::atomic<Sz>;

    /** the same as ptrdiff_t but is atomic. */
    using APDiff = boost::atomic<PDiff>;
    
    // \cond INTERNAL
    namespace Internal {
        template<Sz __Bytes>
        struct TF_UInt {
            using F = void;
        };
        template<>
        struct TF_UInt<1> {
            using F = U8;
        };
        template<>
        struct TF_UInt<2> {
            using F = U16;
        };
        template<>
        struct TF_UInt<4> {
            using F = U32;
        };
        template<>
        struct TF_UInt<8> {
            using F = U64;
        };
    }
    // \endcond
    template<Sz __Bytes>
    using TF_UInt = typename Internal::TF_UInt<__Bytes>::F;
    template<typename __F>
    using TF_TypeToUInt = TF_UInt<sizeof(__F)>;

    // \cond INTERNAL
    namespace Internal {
        template<Sz __Bytes>
        struct TF_Int {
            using F = void;
        };
        template<>
        struct TF_Int<1> {
            using F = I8;
        };
        template<>
        struct TF_Int<2> {
            using F = I16;
        };
        template<>
        struct TF_Int<4> {
            using F = I32;
        };
        template<>
        struct TF_Int<8> {
            using F = I64;
        };
    }
    // \endcond
    template<Sz __Bytes>
    using TF_Int = typename Internal::TF_Int<__Bytes>::F;
    template<typename __F>
    using TF_TypeToInt = TF_Int<sizeof(__F)>;

    // \cond INTERNAL
    namespace Internal {
        template<Sz __Bytes>
        struct TF_AtomicUInt {
            using F = void;
        };
        template<>
        struct TF_AtomicUInt<1> {
            using F = AU8;
        };
        template<>
        struct TF_AtomicUInt<2> {
            using F = AU16;
        };
        template<>
        struct TF_AtomicUInt<4> {
            using F = AU32;
        };
        template<>
        struct TF_AtomicUInt<8> {
            using F = AU64;
        };
    }
    // \endcond
    template<Sz __Bytes>
    using TF_AtomicUInt = typename Internal::TF_AtomicUInt<__Bytes>::F;
    template<typename __F>
    using TF_TypeToAtomicUInt = TF_AtomicUInt<sizeof(__F)>;

    // \cond INTERNAL
    namespace Internal {
        template<Sz __Bytes>
        struct TF_AtomicInt {
            using F = void;
        };
        template<>
        struct TF_AtomicInt<1> {
            using F = AI8;
        };
        template<>
        struct TF_AtomicInt<2> {
            using F = AI16;
        };
        template<>
        struct TF_AtomicInt<4> {
            using F = AI32;
        };
        template<>
        struct TF_AtomicInt<8> {
            using F = AI64;
        };
    }
    // \endcond
    template<Sz __Bytes>
    using TF_AtomicInt = typename Internal::TF_AtomicInt<__Bytes>::F;
    template<typename __F>
    using TF_TypeToAtomicInt = TF_AtomicInt<sizeof(__F)>;

    template<typename __F>
    ABYTEK_FORCE_INLINE static auto ToUInt(__F Value) noexcept
    {
        return static_cast<TF_TypeToUInt<__F>>(Value);
    }
    template<typename __F>
    ABYTEK_FORCE_INLINE static auto ToInt(__F Value) noexcept
    {
        return static_cast<TF_TypeToInt<__F>>(Value);
    }
#pragma endregion

    static constexpr I32 INVALID_INDEX_32 = ~I32(0);
    
    template<typename F__>
    ABYTEK_FORCE_INLINE F__& MakeNullReference() noexcept
    {
        return *((F__*)0);
    }

    template<typename F__>
    ABYTEK_FORCE_INLINE B8 IsValid(F__ const* ptr) noexcept
    {
        return (ptr != 0);
    }
    template<typename F__>
    ABYTEK_FORCE_INLINE B8 IsValid(F__&& ptr) noexcept
    {
        return (&ptr != 0);
    }

    template<typename F__>
    ABYTEK_FORCE_INLINE B8 IsNull(F__ const* ptr) noexcept
    {
        return (ptr == 0);
    }
    template<typename F__>
    ABYTEK_FORCE_INLINE B8 IsNull(F__&& ptr) noexcept
    {
        return (&ptr == 0);
    }

    ABYTEK_FORCE_INLINE void PauseConsole()
    {
        std::cout << "Press any key to continue..." << std::endl;
        std::cin.get();
    }

    template<typename __F>
    ABYTEK_FORCE_INLINE constexpr __F Max(__F a, __F b) noexcept
    {
        return (a > b) ? a : b;
    }
    template<typename __F>
    ABYTEK_FORCE_INLINE constexpr __F Min(__F a, __F b) noexcept
    {
        return (a < b) ? a : b;
    }
    
    template<typename __F, auto __N>
    ABYTEK_FORCE_INLINE constexpr auto GetStaticArraySize(__F (&A)[__N]) noexcept
    {
        return __N;
    }
    template<typename __F, auto __N>
    ABYTEK_FORCE_INLINE void CopyStaticArray(__F (&Dest)[__N], __F* Src) noexcept
    {
        memcpy(
            Dest,
            Src,
            sizeof(__F) * __N
        );
    }

    template<typename __F>
    using TF_RawForward = __F;
}



#define ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(KEYWORD_BEFORE, ...) \
            KEYWORD_BEFORE ABYTEK_FORCE_INLINE __VA_ARGS__ operator ~ (__VA_ARGS__ A) noexcept \
            { \
                using F_UInt = TF_TypeToUInt<__VA_ARGS__>;\
                return static_cast<__VA_ARGS__>( \
                    ~static_cast<F_UInt>(A) \
                ); \
            } \
            KEYWORD_BEFORE ABYTEK_FORCE_INLINE __VA_ARGS__ operator & (__VA_ARGS__ A, __VA_ARGS__ B) noexcept \
            { \
                using F_UInt = TF_TypeToUInt<__VA_ARGS__>;\
                return static_cast<__VA_ARGS__>( \
                    static_cast<F_UInt>(A) & static_cast<F_UInt>(B) \
                ); \
            } \
            KEYWORD_BEFORE ABYTEK_FORCE_INLINE __VA_ARGS__ operator | (__VA_ARGS__ A, __VA_ARGS__ B) noexcept \
            { \
                using F_UInt = TF_TypeToUInt<__VA_ARGS__>;\
                return static_cast<__VA_ARGS__>( \
                    static_cast<F_UInt>(A) | static_cast<F_UInt>(B) \
                ); \
            } \
            KEYWORD_BEFORE ABYTEK_FORCE_INLINE __VA_ARGS__ operator << (__VA_ARGS__ A, __VA_ARGS__ B) noexcept \
            { \
                using F_UInt = TF_TypeToUInt<__VA_ARGS__>;\
                return static_cast<__VA_ARGS__>( \
                    static_cast<F_UInt>(A) << static_cast<F_UInt>(B) \
                ); \
            } \
            KEYWORD_BEFORE ABYTEK_FORCE_INLINE __VA_ARGS__ operator >> (__VA_ARGS__ A, __VA_ARGS__ B) noexcept \
            { \
                using F_UInt = TF_TypeToUInt<__VA_ARGS__>;\
                return static_cast<__VA_ARGS__>( \
                    static_cast<F_UInt>(A) >> static_cast<F_UInt>(B) \
                ); \
            } \
            KEYWORD_BEFORE ABYTEK_FORCE_INLINE __VA_ARGS__& operator &= (__VA_ARGS__& A, __VA_ARGS__ B) noexcept \
            { \
                using F_UInt = TF_TypeToUInt<__VA_ARGS__>;\
                A = static_cast<__VA_ARGS__>( \
                    static_cast<F_UInt>(A) & static_cast<F_UInt>(B) \
                ); \
                return A; \
            } \
            KEYWORD_BEFORE ABYTEK_FORCE_INLINE __VA_ARGS__& operator |= (__VA_ARGS__& A, __VA_ARGS__ B) noexcept \
            { \
                using F_UInt = TF_TypeToUInt<__VA_ARGS__>;\
                A = static_cast<__VA_ARGS__>( \
                    static_cast<F_UInt>(A) | static_cast<F_UInt>(B) \
                ); \
                return A; \
            } \
            KEYWORD_BEFORE ABYTEK_FORCE_INLINE __VA_ARGS__& operator <<= (__VA_ARGS__& A, __VA_ARGS__ B) noexcept \
            { \
                using F_UInt = TF_TypeToUInt<__VA_ARGS__>;\
                A = static_cast<__VA_ARGS__>( \
                    static_cast<F_UInt>(A) << static_cast<F_UInt>(B) \
                ); \
                return A; \
            } \
            KEYWORD_BEFORE ABYTEK_FORCE_INLINE __VA_ARGS__& operator >>= (__VA_ARGS__& A, __VA_ARGS__ B) noexcept \
            { \
                using F_UInt = TF_TypeToUInt<__VA_ARGS__>;\
                A = static_cast<__VA_ARGS__>( \
                    static_cast<F_UInt>(A) >> static_cast<F_UInt>(B) \
                ); \
                return A; \
            }

#define ABYTEK_DEFINE_FLAG_OPERATORS(...) ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(constexpr, __VA_ARGS__)
#define ABYTEK_DEFINE_FLAG_OPERATORS_NON_CONSTEXPR(...) ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(, __VA_ARGS__)

#define ABYTEK_INTERNAL_FORWARD_WITH_DUMMY(Name, ...) Name


#define ABYTEK_MEMBER_OFFSET(T, Member) offsetof(T, Member)