/**
 * @file VectorVector.hpp
 * @brief Includes all vector-vector operation headers for different numeric types.
 * 
 * This file serves as a single include point for all vector-vector operations,
 * bringing in the specialized implementations for different numeric types:
 * - F32 (32-bit floating point)
 * - I32 (32-bit signed integer)
 * - U32 (32-bit unsigned integer)
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Operators/VectorVector_F32.hpp"
#include "Abytek/Operators/VectorVector_I64.hpp"
#include "Abytek/Operators/VectorVector_I32.hpp"
#include "Abytek/Operators/VectorVector_I16.hpp"
#include "Abytek/Operators/VectorVector_I8.hpp"
#include "Abytek/Operators/VectorVector_U64.hpp"
#include "Abytek/Operators/VectorVector_U32.hpp"
#include "Abytek/Operators/VectorVector_U16.hpp"
#include "Abytek/Operators/VectorVector_U8.hpp"