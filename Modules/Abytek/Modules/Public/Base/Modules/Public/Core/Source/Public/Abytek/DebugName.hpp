#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/Name.hpp"
#include "Abytek/CrtUseAllocatorTypedefs.hpp"


namespace Abytek
{
    /**
     * @brief Mode enumeration for F_DebugName
     * 
     * Determines whether a debug name uses static or dynamic storage
     */
    enum class E_DebugNameMode : U8
    {
        NONE,  ///< Invalid
        STATIC,  ///< Name is stored as a static string pointer
        DYNAMIC_NAME,  ///< Name is stored as a dynamic F_Name object
        DYNAMIC_TEXT  ///< Name is stored as a dynamic F_Text object. Used to reduce mem leak
    };

    /**
     * @brief Utility class for debug naming of objects
     * 
     * Provides a flexible way to assign debug names to objects,
     * supporting both static string literals and dynamic name objects.
     */
    struct F_DebugName
    {
    private:
        E_DebugNameMode _Mode = E_DebugNameMode::NONE;
        const F_TextChar* _Static = 0;
        F_Name _DynamicName;
        F_Text _DynamicText;

    public:
        /**
         * @brief Get the current storage mode of the debug name
         * @return The mode (STATIC or DYNAMIC)
         */
        ABYTEK_FORCE_INLINE auto GetMode() const noexcept
        {
            return _Mode;
        }
        
        /**
         * @brief Get the static string pointer
         * @return Pointer to the static string or null if not in static mode
         */
        ABYTEK_FORCE_INLINE const F_TextChar* GetStatic() const noexcept
        {
            return _Static;
        }
        
        /**
         * @brief Get the dynamic name object
         * @return Reference to the dynamic name
         */
        ABYTEK_FORCE_INLINE const F_Name& GetDynamicName() const noexcept
        {
            return _DynamicName;
        }
        
        /**
         * @brief Get the dynamic text object
         * @return Reference to the dynamic text
         */
        ABYTEK_FORCE_INLINE const F_Text& GetDynamicText() const noexcept
        {
            return _DynamicText;
        }
        
        /**
         * @brief Get the actual name string pointer regardless of storage mode
         * @return Pointer to the name string or null if not set
         */
        ABYTEK_FORCE_INLINE const F_TextChar* GetDataP() const noexcept
        {
            switch (_Mode)
            {
            case E_DebugNameMode::STATIC:
                return _Static;
                break;
            case E_DebugNameMode::DYNAMIC_NAME:
                if(_DynamicName)
                {
                    return _DynamicName->data();
                }
                break;
            case E_DebugNameMode::DYNAMIC_TEXT:
                return _DynamicText.c_str();
            default:
                break;
            }
            return nullptr;
        }

    private:
        /**
         * @brief Private constructor for static names
         * @param Static Pointer to a static string
         */
        ABYTEK_FORCE_INLINE F_DebugName(const F_TextChar* Static) noexcept :
            _Mode(E_DebugNameMode::STATIC),
            _Static(Static)
        {
        }
        
    public:
        /**
         * @brief Default constructor
         * Creates an empty debug name
         */
        ABYTEK_FORCE_INLINE F_DebugName() noexcept = default;
        
        /**
         * @brief Constructor for dynamic name
         * @param DynamicName The dynamic name to use
        */
        ABYTEK_FORCE_INLINE F_DebugName(const F_Name& DynamicName) noexcept :
            _Mode(E_DebugNameMode::DYNAMIC_NAME),
            _DynamicName(DynamicName)
        {
        }
        
        /**
         * @brief Constructor for dynamic text
         * @param DynamicText The dynamic text to use
        */
        ABYTEK_FORCE_INLINE F_DebugName(const F_Text& DynamicText) noexcept :
            _Mode(E_DebugNameMode::DYNAMIC_TEXT),
            _DynamicText(DynamicText)
        {
        }
        
        /**
         * @brief Copy constructor
         * @param X The debug name to copy from
         */
        ABYTEK_FORCE_INLINE F_DebugName(const F_DebugName& X) noexcept = default;
        
        /**
         * @brief Copy assignment operator
         * @param X The debug name to copy from
         * @return Reference to this object
         */
        ABYTEK_FORCE_INLINE F_DebugName& operator = (const F_DebugName& X) noexcept = default;
        
        /**
         * @brief Move constructor
         * @param X The debug name to move from
         */
        ABYTEK_FORCE_INLINE F_DebugName(F_DebugName&& X) noexcept = default;
        
        /**
         * @brief Move assignment operator
         * @param X The debug name to move from
         * @return Reference to this object
         */
        ABYTEK_FORCE_INLINE F_DebugName& operator = (F_DebugName&& X) noexcept = default;

    public:
        /**
         * @brief Boolean conversion operator
         * @return True if the debug name has been set, false otherwise
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return _Mode != E_DebugNameMode::NONE;
        }
        
        /**
         * @brief Dereference operator
         * @return Pointer to the name string
         */
        ABYTEK_FORCE_INLINE const F_TextChar* operator * () const noexcept
        {
            if (auto Result = GetDataP())
            {
                return Result;
            }
            return ABYTEK_TEXT("None");
        }

    public:
        /**
         * @brief Factory method to create a debug name with static storage
         * @param Static Pointer to a static string
         * @return A new debug name instance with static storage
         */
        static ABYTEK_FORCE_INLINE F_DebugName MakeStatic(const F_TextChar* Static)
        {
            return F_DebugName { Static };
        }
		
        /**
         * @brief Equality comparison operator
         * @param X The string ID to compare with
         * @return true if the hash codes are equal
         */
        ABYTEK_FORCE_INLINE B8 operator == (const F_DebugName& X) const noexcept
        {
            return boost::hash_value(GetDataP()) == boost::hash_value(X.GetDataP());
        }
		
        /**
         * @brief Inequality comparison operator
         * @param X The string ID to compare with
         * @return true if the hash codes are not equal
         */
        ABYTEK_FORCE_INLINE B8 operator != (const F_DebugName& X) const noexcept
        {
            return boost::hash_value(GetDataP()) != boost::hash_value(X.GetDataP());
        }
		
        /**
         * @brief Greater than comparison operator
         * @param X The string ID to compare with
         * @return true if this hash code is greater than X's hash code
         */
        ABYTEK_FORCE_INLINE B8 operator > (const F_DebugName& X) const noexcept
        {
            return boost::hash_value(GetDataP()) > boost::hash_value(X.GetDataP());
        }
		
        /**
         * @brief Less than comparison operator
         * @param X The string ID to compare with
         * @return true if this hash code is less than X's hash code
         */
        ABYTEK_FORCE_INLINE B8 operator < (const F_DebugName& X) const noexcept
        {
            return boost::hash_value(GetDataP()) < boost::hash_value(X.GetDataP());
        }
		
        /**
         * @brief Greater than or equal comparison operator
         * @param X The string ID to compare with
         * @return true if this hash code is greater than or equal to X's hash code
         */
        ABYTEK_FORCE_INLINE B8 operator >= (const F_DebugName& X) const noexcept
        {
            return boost::hash_value(GetDataP()) >= boost::hash_value(X.GetDataP());
        }
		
        /**
         * @brief Less than or equal comparison operator
         * @param X The string ID to compare with
         * @return true if this hash code is less than or equal to X's hash code
         */
        ABYTEK_FORCE_INLINE B8 operator <= (const F_DebugName& X) const noexcept
        {
            return boost::hash_value(GetDataP()) <= boost::hash_value(X.GetDataP());
        }
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_DebugName>
    {
        size_t operator() (const Abytek::F_DebugName& X) const
        {
            return boost::hash_value(X.GetDataP());
        }
    };
}


/**
 * @brief Macro to create a static debug name from a string literal
 * @param ... The string literal to use as the debug name
 */
#define ABYTEK_DEBUG_NAME(...) Abytek::F_DebugName::MakeStatic(ABYTEK_TEXT(__VA_ARGS__))