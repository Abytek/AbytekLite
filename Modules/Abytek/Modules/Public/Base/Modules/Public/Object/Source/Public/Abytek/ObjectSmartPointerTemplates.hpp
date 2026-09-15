/**
 * @file ObjectSmartPointerTemplates.hpp
 * @brief Main header file for Abytek's smart pointer system.
 * 
 * This is the primary include file that brings together all components of the
 * Abytek smart pointer system. It includes definitions for weak, unique, and
 * shared pointer templates, as well as their supporting infrastructure.
 * 
 * The smart pointer system provides memory-safe object management with:
 * - Weak pointers (TW) - Non-owning references to objects
 * - Unique pointers (TU) - Exclusive ownership of objects
 * - Shared pointers (TS) - Shared ownership with reference counting
 * 
 * Object Creation:
 * The recommended way to create objects with smart pointers is using the function call operator:
 * 
 * SmartPointerType<ObjectType>()(args...)
 * 
 * Examples:
 * - TU<F_Foo>()()           // Create unique pointer with default constructor
 * - TU<F_Foo>()(42)         // Create unique pointer with parameters
 * - TS<F_Foo>()(100, "Bar") // Create shared pointer with multiple parameters
 * 
 * This syntax creates a temporary default-constructed smart pointer and
 * immediately calls its function call operator to create and return a
 * new smart pointer that owns the newly created object.
 * 
 * Note: Do NOT use the F_ObjectQuickCreateFlag constructor directly.
 * Always prefer the function call operator syntax for object creation.
 */
#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectBase.hpp"
#include "Abytek/ObjectHeader.hpp"
#include "Abytek/ObjectMemory.hpp"
#include "Abytek/ObjectWeakSmartPointerTemplate.hpp"
#include "Abytek/ObjectUniqueSmartPointerTemplate.hpp"
#include "Abytek/ObjectSharedSmartPointerTemplate.hpp"
#include "Abytek/UnmanagedObjectWeakSmartPointerTemplate.hpp"
#include "Abytek/UnmanagedObjectUniqueSmartPointerTemplate.hpp"
#include "Abytek/UnmanagedObjectSharedSmartPointerTemplate.hpp"
#include "Abytek/ObjectSmartPointerHash.hpp"