/**
 * @file CrtUseAllocatorTypedefs.hpp
 * @brief Type definitions for containers using C runtime allocator
 * 
 * This file defines container types that use the C runtime allocator.
 * It includes ContainerTypedefs.inl with the namespace Abytek::WithCrtAllocator
 * to create type definitions for all container types using the CRT allocator.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/DefaultAllocators.hpp"

#define ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE Abytek::WithCrtAllocator
#include "Abytek/ContainerTypedefs.inl"