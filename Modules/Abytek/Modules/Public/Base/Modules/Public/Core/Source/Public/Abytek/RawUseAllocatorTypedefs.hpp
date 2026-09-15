/**
 * @file RawUseAllocatorTypedefs.hpp
 * @brief Type definitions for containers using raw memory allocator
 * 
 * This file defines container types that use the raw memory allocator.
 * It includes ContainerTypedefs.inl with the namespace Abytek::WithRawAllocator
 * to create type definitions for all container types using the raw allocator.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/DefaultAllocators.hpp"

#define ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE Abytek::WithRawAllocator
#include "Abytek/ContainerTypedefs.inl"