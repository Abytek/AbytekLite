/**
 * @file RuntimeTools.hpp
 * @brief Runtime type manipulation tools
 * 
 * This file provides utility structures for runtime type operations such as
 * construction, destruction, copying, and moving of objects whose types may
 * not be known at compile time. These tools use type-erased function pointers
 * to perform type-specific operations.
 */

#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"


namespace Abytek
{
    namespace Internal
    {
        namespace RuntimeTools
        {
            /**
             * @brief Placeholder type used when the real type doesn't support an operation
             */
            struct F_FakeData {};
        }
    }
    
    /**
     * @brief Tool for default-constructing objects at runtime
     * 
     * Provides a type-erased way to default-construct objects whose type
     * may not be known at compile time.
     */
    struct F_RuntimeConstructDefaultTool
    {
        /** Function pointer to the type-specific constructor implementation */
        void (*Address)(void* DataP) = 0;

        /**
         * @brief Construct an object at the given memory location
         * @param DataP Pointer to the memory where the object should be constructed
         */
        ABYTEK_FORCE_INLINE void operator () (void* DataP) const noexcept
        {
            Address(DataP);
        }
        
        /**
         * @brief Check if the tool is valid
         * @return True if the tool can construct objects
         */
        ABYTEK_FORCE_INLINE operator B8 () const noexcept
        {
            return (Address != 0);
        }

        /**
         * @brief Create a constructor tool for a specific type
         * @tparam __F Type to create the constructor for
         * @return A constructor tool for the specified type
         */
        template<typename __F>
        static F_RuntimeConstructDefaultTool Make()
        {
            using F_Parsed = std::conditional_t<
                std::is_default_constructible_v<__F>,
                __F,
                Internal::RuntimeTools::F_FakeData
            >;
            if constexpr (!std::is_same_v<F_Parsed, __F>)
            {
                return { 0 };
            }
            
            return {
                [](void* DataP)
                {
                    new((F_Parsed*)DataP) F_Parsed {};
                }
            };
        }
    };
    
    /**
     * @brief Tool for destructing objects at runtime
     * 
     * Provides a type-erased way to destruct objects whose type
     * may not be known at compile time.
     */
    struct F_RuntimeDestructTool
    {
        /** Function pointer to the type-specific destructor implementation */
        void (*Address)(void* DataP) = 0;

        /**
         * @brief Destruct an object at the given memory location
         * @param DataP Pointer to the object to be destructed
         */
        ABYTEK_FORCE_INLINE void operator () (void* DataP) const noexcept
        {
            Address(DataP);
        }
        
        /**
         * @brief Check if the tool is valid
         * @return True if the tool can destruct objects
         */
        ABYTEK_FORCE_INLINE operator B8 () const noexcept
        {
            return (Address != 0);
        }

        /**
         * @brief Create a destructor tool for a specific type
         * @tparam __F Type to create the destructor for
         * @return A destructor tool for the specified type
         */
        template<typename __F>
        static F_RuntimeDestructTool Make()
        {
            using F_Parsed = std::conditional_t<
                std::is_destructible_v<__F>,
                __F,
                Internal::RuntimeTools::F_FakeData
            >;
            if constexpr (!std::is_same_v<F_Parsed, __F>)
            {
                return { 0 };
            }
            
            return {
                [](void* DataP)
                {
                    ((F_Parsed*)DataP)->~F_Parsed();
                }
            };
        }
    };
    
    /**
     * @brief Tool for copy-constructing objects at runtime
     * 
     * Provides a type-erased way to copy-construct objects whose type
     * may not be known at compile time.
     */
    struct F_RuntimeCopyConstructTool
    {
        /** Function pointer to the type-specific copy constructor implementation */
        void (*Address)(void* DstP, void* SrcP) = 0;

        /**
         * @brief Copy-construct an object at the destination from a source object
         * @param DstP Pointer to the memory where the object should be constructed
         * @param SrcP Pointer to the source object to copy from
         */
        ABYTEK_FORCE_INLINE void operator () (void* DstP, void* SrcP) const noexcept
        {
            Address(DstP, SrcP);
        }
        
        /**
         * @brief Check if the tool is valid
         * @return True if the tool can copy-construct objects
         */
        ABYTEK_FORCE_INLINE operator B8 () const noexcept
        {
            return (Address != 0);
        }

        /**
         * @brief Create a copy constructor tool for a specific type
         * @tparam __F Type to create the copy constructor for
         * @return A copy constructor tool for the specified type
         */
        template<typename __F>
        static F_RuntimeCopyConstructTool Make()
        {
            using F_Parsed = std::conditional_t<
                std::is_copy_constructible_v<__F>,
                __F,
                Internal::RuntimeTools::F_FakeData
            >;
            if constexpr (!std::is_same_v<F_Parsed, __F>)
            {
                return { 0 };
            }
            
            return {
                [](void* DstP, void* SrcP)
                {
                    new((F_Parsed*)DstP) F_Parsed(
                        *(const F_Parsed*)SrcP
                    );      
                }
            };
        }
    };
    
    /**
     * @brief Tool for copy-assigning objects at runtime
     * 
     * Provides a type-erased way to copy-assign objects whose type
     * may not be known at compile time.
     */
    struct F_RuntimeCopyAssignTool
    {
        /** Function pointer to the type-specific copy assignment implementation */
        void (*Address)(void* DstP, void* SrcP) = 0;

        /**
         * @brief Copy-assign a source object to a destination object
         * @param DstP Pointer to the destination object
         * @param SrcP Pointer to the source object to copy from
         */
        ABYTEK_FORCE_INLINE void operator () (void* DstP, void* SrcP) const noexcept
        {
            Address(DstP, SrcP);
        }
        
        /**
         * @brief Check if the tool is valid
         * @return True if the tool can copy-assign objects
         */
        ABYTEK_FORCE_INLINE operator B8 () const noexcept
        {
            return (Address != 0);
        }

        /**
         * @brief Create a copy assignment tool for a specific type
         * @tparam __F Type to create the copy assignment for
         * @return A copy assignment tool for the specified type
         */
        template<typename __F>
        static F_RuntimeCopyAssignTool Make()
        {
            using F_Parsed = std::conditional_t<
                std::is_copy_assignable_v<__F>,
                __F,
                Internal::RuntimeTools::F_FakeData
            >;
            if constexpr (!std::is_same_v<F_Parsed, __F>)
            {
                return { 0 };
            }
            
            return {
                [](void* DstP, void* SrcP)
                {
                    *(F_Parsed*)DstP = *(const F_Parsed*)SrcP;      
                }
            };
        }
    };
    
    /**
     * @brief Tool for move-constructing objects at runtime
     * 
     * Provides a type-erased way to move-construct objects whose type
     * may not be known at compile time.
     */
    struct F_RuntimeMoveConstructTool
    {
        /** Function pointer to the type-specific move constructor implementation */
        void (*Address)(void* DstP, void* SrcP) = 0;

        /**
         * @brief Move-construct an object at the destination from a source object
         * @param DstP Pointer to the memory where the object should be constructed
         * @param SrcP Pointer to the source object to move from
         */
        ABYTEK_FORCE_INLINE void operator () (void* DstP, void* SrcP) const noexcept
        {
            Address(DstP, SrcP);
        }
        
        /**
         * @brief Check if the tool is valid
         * @return True if the tool can move-construct objects
         */
        ABYTEK_FORCE_INLINE operator B8 () const noexcept
        {
            return (Address != 0);
        }

        /**
         * @brief Create a move constructor tool for a specific type
         * @tparam __F Type to create the move constructor for
         * @return A move constructor tool for the specified type
         */
        template<typename __F>
        static F_RuntimeMoveConstructTool Make()
        {
            using F_Parsed = std::conditional_t<
                std::is_move_constructible_v<__F>,
                __F,
                Internal::RuntimeTools::F_FakeData
            >;
            if constexpr (!std::is_same_v<F_Parsed, __F>)
            {
                return { 0 };
            }
            
            return {
                [](void* DstP, void* SrcP)
                {
                    new((F_Parsed*)DstP) __F(
                        boost::move(*(F_Parsed*)SrcP)
                    );      
                }
            };
        }
    };
    
    /**
     * @brief Tool for move-assigning objects at runtime
     * 
     * Provides a type-erased way to move-assign objects whose type
     * may not be known at compile time.
     */
    struct F_RuntimeMoveAssignTool
    {
        /** Function pointer to the type-specific move assignment implementation */
        void (*Address)(void* DstP, void* SrcP) = 0;

        /**
         * @brief Move-assign a source object to a destination object
         * @param DstP Pointer to the destination object
         * @param SrcP Pointer to the source object to move from
         */
        ABYTEK_FORCE_INLINE void operator () (void* DstP, void* SrcP) const noexcept
        {
            Address(DstP, SrcP);
        }
        
        /**
         * @brief Check if the tool is valid
         * @return True if the tool can move-assign objects
         */
        ABYTEK_FORCE_INLINE operator B8 () const noexcept
        {
            return (Address != 0);
        }

        /**
         * @brief Create a move assignment tool for a specific type
         * @tparam __F Type to create the move assignment for
         * @return A move assignment tool for the specified type
         */
        template<typename __F>
        static F_RuntimeMoveAssignTool Make()
        {
            using F_Parsed = std::conditional_t<
                std::is_move_assignable_v<__F>,
                __F,
                Internal::RuntimeTools::F_FakeData
            >;
            if constexpr (!std::is_same_v<F_Parsed, __F>)
            {
                return { 0 };
            }
            
            return {
                [](void* DstP, void* SrcP)
                {
                    *(F_Parsed*)DstP = boost::move(*(F_Parsed*)SrcP);      
                }
            };
        }
    };
}