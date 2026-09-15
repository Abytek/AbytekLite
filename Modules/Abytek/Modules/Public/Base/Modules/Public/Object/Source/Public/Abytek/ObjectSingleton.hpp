/**
 * @file ObjectSingleton.hpp
 * @brief Provides macros for creating and managing singleton objects
 * 
 * This file contains macros that simplify the implementation of the singleton pattern
 * for various object types with different allocator strategies. The macros handle
 * the declaration, definition, and binding of singleton instances.
 */

#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"

/**
 * @brief Declares a singleton object with the specified namespace and parameters
 * 
 * This macro declares a singleton pattern implementation with a protected static instance
 * and a public getter method.
 * 
 * @param Namespace The namespace containing the smart pointer implementation
 * @param ... The class type to be declared as a singleton
 */
#define ABYTEK_DECLARE_OBJECT_SINGLETON_ADVANCED(Namespace, ...) \
            ABYTEK_PROTECTED_KEYWORD \
                static Namespace TW<__VA_ARGS__> _Instance; \
            ABYTEK_PUBLIC_KEYWORD \
                static ABYTEK_FORCE_INLINE Namespace TW<__VA_ARGS__>& GetInstance() noexcept \
                { \
                    return _Instance; \
                }

/**
 * @brief Defines the static singleton instance declared by ABYTEK_DECLARE_OBJECT_SINGLETON_ADVANCED
 * 
 * @param Namespace The namespace containing the smart pointer implementation
 * @param ... The class type for which to define the singleton instance
 */
#define ABYTEK_DEFINE_OBJECT_SINGLETON_ADVANCED(Namespace, ...) Namespace TW<__VA_ARGS__> __VA_ARGS__::_Instance;

/**
 * @brief Binds a singleton instance to the current object instance
 * 
 * This macro initializes the singleton instance if not already initialized and
 * binds it to the current instance of the class. It asserts if multiple instances attempt
 * to bind to the singleton.
 * 
 * @param Namespace The namespace containing the smart pointer implementation
 */
#define ABYTEK_BIND_OBJECT_SINGLETON_ADVANCED(Namespace) \
            if (!_Instance) _Instance = {}; \
            ABYTEK_BASE_CORE_ASSERT((!_Instance) || (_Instance == ABYTEK_WTHIS(Namespace))) << "only one instance is allowed"; \
            _Instance = ABYTEK_WTHIS_NO_VALIDATION(Namespace);

/**
 * @brief Declares a singleton using CRT allocator
 * 
 * Shorthand for ABYTEK_DECLARE_OBJECT_SINGLETON_ADVANCED with CRT allocator namespace
 * 
 * @param ... The class type to be declared as a singleton
 */
#define ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(...) ABYTEK_DECLARE_OBJECT_SINGLETON_ADVANCED(Abytek::WithCrtAllocator::, __VA_ARGS__)

/**
 * @brief Defines a singleton using CRT allocator
 * 
 * Shorthand for ABYTEK_DEFINE_OBJECT_SINGLETON_ADVANCED with CRT allocator namespace
 * 
 * @param ... The class type for which to define the singleton instance
 */
#define ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(...) ABYTEK_DEFINE_OBJECT_SINGLETON_ADVANCED(Abytek::WithCrtAllocator::, __VA_ARGS__)

/**
 * @brief Binds a singleton using CRT allocator
 * 
 * Shorthand for ABYTEK_BIND_OBJECT_SINGLETON_ADVANCED with CRT allocator namespace
 */
#define ABYTEK_BIND_OBJECT_SINGLETON_CRT() ABYTEK_BIND_OBJECT_SINGLETON_ADVANCED(Abytek::WithCrtAllocator::)

/**
 * @brief Declares a singleton using RAW allocator
 * 
 * Shorthand for ABYTEK_DECLARE_OBJECT_SINGLETON_ADVANCED with RAW allocator namespace
 * 
 * @param ... The class type to be declared as a singleton
 */
#define ABYTEK_DECLARE_OBJECT_SINGLETON_RAW(...) ABYTEK_DECLARE_OBJECT_SINGLETON_ADVANCED(Abytek::WithRawAllocator::, __VA_ARGS__)

/**
 * @brief Defines a singleton using RAW allocator
 * 
 * Shorthand for ABYTEK_DEFINE_OBJECT_SINGLETON_ADVANCED with RAW allocator namespace
 * 
 * @param ... The class type for which to define the singleton instance
 */
#define ABYTEK_DEFINE_OBJECT_SINGLETON_RAW(...) ABYTEK_DEFINE_OBJECT_SINGLETON_ADVANCED(Abytek::WithRawAllocator::, __VA_ARGS__)

/**
 * @brief Binds a singleton using RAW allocator
 * 
 * Shorthand for ABYTEK_BIND_OBJECT_SINGLETON_ADVANCED with RAW allocator namespace
 */
#define ABYTEK_BIND_OBJECT_SINGLETON_RAW() ABYTEK_BIND_OBJECT_SINGLETON_ADVANCED(Abytek::WithRawAllocator::)