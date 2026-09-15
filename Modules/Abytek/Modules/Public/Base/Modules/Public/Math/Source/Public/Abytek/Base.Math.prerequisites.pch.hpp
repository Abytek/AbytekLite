#pragma once

#include "Abytek/Base.Math.config.hpp"
#include "Abytek/Base.Minimal.hpp"
#include "Abytek/Base.Core.hpp"
#include "Abytek/Base.Reflection.hpp"
#include "Abytek/Base.Serializable.hpp"


namespace Abytek
{
    ABYTEK_BASE_MATH_API void SetupReflectionSession_Base_Math(const TW_Valid<F_ReflectionSession>& Session);
    
    namespace Internal::MathOperators
    {
    }
    using namespace Internal::MathOperators;
    
    ABYTEK_FORCE_INLINE F32 I32AsF32(I32 a) noexcept {

        union {
            F32 f;
            I32 i;
        } temp;

        temp.i = a;

        return temp.f;
    }
    ABYTEK_FORCE_INLINE F32 U32AsF32(U32 a) noexcept {

        union {
            F32 f;
            U32 u;
        } temp;

        temp.u = a;

        return temp.f;
    }

    namespace Internal
    {
        template<typename __F>
        struct TH_DefaultTolerance
        {
            static constexpr __F Value = ABYTEK_DEFAULT_FP_TOLERANCE;
        };
    }
    template<typename __F = ABYTEK_DEFAULT_FP_TYPE>
    static constexpr __F DEFAULT_TOLERANCE = Internal::TH_DefaultTolerance<__F>::Value;
}

using namespace Abytek::Internal::MathOperators;

#define ABYTEK_F32_ONE 1.0f
#define ABYTEK_F32_ONE_NEGATIVE -1.0f
#define ABYTEK_F32_ZERO 0.0f
#define ABYTEK_F32_FULL ::Abytek::U32AsF32(ABYTEK_U32_MAX)

#define ABYTEK_I32_ONE 1
#define ABYTEK_I32_ONE_NEGATIVE -1
#define ABYTEK_I32_ZERO 0
#define ABYTEK_I32_FULL ABYTEK_U32_MAX

#define ABYTEK_U32_ONE 1
#define ABYTEK_U32_ZERO 0
#define ABYTEK_U32_FULL ABYTEK_U32_MAX

#define ABYTEK_F32_INFINITY (std::numeric_limits<Abytek::F32>::infinity())
#define ABYTEK_F64_INFINITY (std::numeric_limits<Abytek::F64>::infinity())

#define ABYTEK_F32_NEGATIVE_INFINITY (-std::numeric_limits<Abytek::F32>::infinity())
#define ABYTEK_F64_NEGATIVE_INFINITY (-std::numeric_limits<Abytek::F64>::infinity())

#define ABYTEK_F32_PI (3.14159265358979323846f)
#define ABYTEK_F64_PI (3.141592653589793238462643383279502884)

#define ABYTEK_F32_DEGREES_TO_RADS (ABYTEK_F32_PI / 180.0f)
#define ABYTEK_F32_RADS_TO_DEGREES (180.0f / ABYTEK_F32_PI)

#define ABYTEK_F32_HALF_PI (ABYTEK_F32_PI * 0.5f)
#define ABYTEK_F64_HALF_PI (ABYTEK_F64_PI * 0.5)

#define ABYTEK_F32_TWO_PI (ABYTEK_F32_PI * 2.0f)
#define ABYTEK_F64_TWO_PI (ABYTEK_F64_PI * 2.0)

#define ABYTEK_F32_HALF (0.5f)
#define ABYTEK_F64_HALF (0.5)

#define ABYTEK_F32_INV_PI (1.0f / 3.14159265358979323846f)
#define ABYTEK_F64_INV_PI (1.0 / 3.141592653589793238462643383279502884)

#define ABYTEK_F32_INV_HALF_PI (ABYTEK_F32_INV_PI / 0.5f)
#define ABYTEK_F64_INV_HALF_PI (ABYTEK_F64_INV_PI / 0.5)

#define ABYTEK_F32_INV_TWO_PI (ABYTEK_F32_INV_PI / 2.0f)
#define ABYTEK_F64_INV_TWO_PI (ABYTEK_F64_INV_PI / 2.0)

#define ABYTEK_F32_INV_HALF (1.0f / 0.5f)
#define ABYTEK_F64_INV_HALF (1.0 / 0.5)

#define ABYTEK_F32_NEGATIVE_PI (-3.14159265358979323846f)
#define ABYTEK_F64_NEGATIVE_PI (-3.141592653589793238462643383279502884)

#define ABYTEK_F32_NEGATIVE_HALF_PI (-ABYTEK_F32_PI * 0.5f)
#define ABYTEK_F64_NEGATIVE_HALF_PI (-ABYTEK_F64_PI * 0.5)

#define ABYTEK_F32_NEGATIVE_TWO_PI (-ABYTEK_F32_PI * 2.0f)
#define ABYTEK_F64_NEGATIVE_TWO_PI (-ABYTEK_F64_PI * 2.0)

#define ABYTEK_F32_NEGATIVE_HALF (-0.5f)
#define ABYTEK_F64_NEGATIVE_HALF (-0.5)

#define ABYTEK_F32_INV_NEGATIVE_PI (-1.0f / 3.14159265358979323846f)
#define ABYTEK_F64_INV_NEGATIVE_PI (-1.0 / 3.141592653589793238462643383279502884)

#define ABYTEK_F32_INV_NEGATIVE_HALF_PI (-ABYTEK_F32_INV_PI / 0.5f)
#define ABYTEK_F64_INV_NEGATIVE_HALF_PI (-ABYTEK_F64_INV_PI / 0.5)

#define ABYTEK_F32_INV_NEGATIVE_TWO_PI (-ABYTEK_F32_INV_PI / 2.0f)
#define ABYTEK_F64_INV_NEGATIVE_TWO_PI (-ABYTEK_F64_INV_PI / 2.0)

#define ABYTEK_F32_INV_NEGATIVE_HALF (-1.0f / 0.5f)
#define ABYTEK_F64_INV_NEGATIVE_HALF (-1.0 / 0.5)