/**
 * @file MatrixMatrix.hpp
 * @brief Includes all matrix-matrix operation headers for different numeric types.
 * 
 * This file serves as a single include point for all matrix-matrix operations,
 * bringing in the specialized implementations for different numeric types:
 * - F32 (32-bit floating point)
 * - I32 (32-bit signed integer)
 * - U32 (32-bit unsigned integer)
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Operators/MatrixMatrix_F32.hpp"
#include "Abytek/Operators/MatrixMatrix_I64.hpp"
#include "Abytek/Operators/MatrixMatrix_I32.hpp"
#include "Abytek/Operators/MatrixMatrix_I16.hpp"
#include "Abytek/Operators/MatrixMatrix_I8.hpp"
#include "Abytek/Operators/MatrixMatrix_U64.hpp"
#include "Abytek/Operators/MatrixMatrix_U32.hpp"
#include "Abytek/Operators/MatrixMatrix_U16.hpp"
#include "Abytek/Operators/MatrixMatrix_U8.hpp"