/**
 * @file ObjectHeader.hpp
 * @brief Defines memory header structures for unique and shared objects.
 * 
 * This file contains the header structures that precede the actual object data
 * in memory, storing metadata such as object keys, releasers, and reference counters.
 */
#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectBase.hpp"


namespace Abytek
{
    template<typename __F_Object>
    static constexpr Sz ObjectMemoryAlignment = (
        (ABYTEK_ALIGNOF(__F_Object) > ABYTEK_ALIGNOF(void*))
        ? ABYTEK_ALIGNOF(__F_Object)
        : ABYTEK_ALIGNOF(void*)
    );
}

namespace Abytek::ObjectSmartPointerTemplates
{
    /**
     * @brief Memory header structure for managed objects.
     * 
     * Contains the memory layout preceding a managed object,
     * storing the object key, releaser function, and atomic reference counter.
     * 
     * @tparam __F_Object The object type
     * @tparam __F_ObjectManagement The object management strategy
     */
    template<class __F_Object, class __F_ObjectManagement>
    struct ABYTEK_ALIGN(ObjectMemoryAlignment<__F_Object>) TF_ManagedObjectHeader
    {
    public:
        using F_Object = __F_Object;
        using F_ObjectManagement = __F_ObjectManagement;
        
    private:
        using F_ObjectKey = typename F_ObjectManagement::F_ObjectKey;
        using F_ObjectKeyPool = typename F_ObjectManagement::F_ObjectKeyPool;

    public:
        /** Size of the header, properly aligned for the object type */
        static constexpr Sz Size = AlignSize_PO2(
            sizeof(AU32) + sizeof(F_ObjectReleaser) + sizeof(F_ObjectKey),
            ObjectMemoryAlignment<__F_Object>
        );
        
        /** Raw data storage for the header */
        U8 Data[Size];

        /**
         * @brief Gets the object key stored in the header.
         * @return Reference to the object key
         */
        ABYTEK_FORCE_INLINE F_ObjectKey& GetKey() noexcept
        {
            return ((F_ObjectKey*)(
                Data
                + Size
            ))[-1];
        }
        
        /**
         * @brief Gets the object releaser function stored in the header.
         * @return Reference to the releaser function
         */
        ABYTEK_FORCE_INLINE F_ObjectReleaser& GetReleaser() noexcept
        {
            return ((F_ObjectReleaser*)(
                Data
                + Size
                - sizeof(F_ObjectKey)
            ))[-1];
        }
        
        /**
         * @brief Gets the atomic reference counter stored in the header.
         * @return Reference to the atomic counter
         */
        ABYTEK_FORCE_INLINE AU32& GetCounter() noexcept
        {
            return ((AU32*)(
                Data
                + Size
                - sizeof(F_ObjectKey)
                - sizeof(F_ObjectReleaser)
            ))[-1];
        }
    };
}

namespace Abytek
{
    template<class __F_Object>
    struct ABYTEK_ALIGN(ObjectMemoryAlignment<__F_Object>) TF_UniqueUnmanagedObjectHeader
    {
    public:
        using F_Object = __F_Object;

    public:
        /** Size of the header, properly aligned for the object type */
        static constexpr Sz Size = AlignSize_PO2(
            sizeof(F_ObjectReleaser),
            ObjectMemoryAlignment<__F_Object>
        );
        
        /** Raw data storage for the header */
        U8 Data[Size];

        /**
         * @brief Gets the object releaser function stored in the header.
         * @return Reference to the releaser function
         */
        ABYTEK_FORCE_INLINE F_ObjectReleaser& GetReleaser() noexcept
        {
            return ((F_ObjectReleaser*)(
                Data
                + Size
            ))[-1];
        }
    };
    
    template<class __F_Object>
    struct ABYTEK_ALIGN(ObjectMemoryAlignment<__F_Object>) TF_SharedUnmanagedObjectHeader
    {
    public:
        using F_Object = __F_Object;

    public:
        /** Size of the header, properly aligned for the object type */
        static constexpr Sz Size = AlignSize_PO2(
            sizeof(AU32) + sizeof(F_ObjectReleaser),
            ObjectMemoryAlignment<__F_Object>
        );
        
        /** Raw data storage for the header */
        U8 Data[Size];
        
        /**
         * @brief Gets the object releaser function stored in the header.
         * @return Reference to the releaser function
         */
        ABYTEK_FORCE_INLINE F_ObjectReleaser& GetReleaser() noexcept
        {
            return ((F_ObjectReleaser*)(
                Data
                + Size
            ))[-1];
        }
        
        /**
         * @brief Gets the atomic reference counter stored in the header.
         * @return Reference to the atomic counter
         */
        ABYTEK_FORCE_INLINE AU32& GetCounter() noexcept
        {
            return ((AU32*)(
                Data
                + Size
                - sizeof(F_ObjectReleaser)
            ))[-1];
        }
    };
}