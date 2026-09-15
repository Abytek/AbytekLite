/**
 * @file ObjectMemory.hpp
 * @brief Defines memory layouts for uniquely and shared owned objects.
 * 
 * This file contains the memory structures that combine object headers with 
 * the actual object data storage, providing a complete memory representation
 * for managed objects.
 */
#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectBase.hpp"


namespace Abytek::ObjectSmartPointerTemplates
{
    /**
     * @brief Memory layout for uniquely owned objects.
     * 
     * Combines the unique object header with storage for the actual object data.
     * This is allocated as a single memory block when creating unique objects.
     * 
     * @tparam __F_Object The object type
     * @tparam __F_ObjectManagement The object management strategy
     */
    template<class __F_Object, class __F_ObjectManagement>
    struct ABYTEK_ALIGN(ABYTEK_ALIGNOF(__F_Object)) TF_ManagedObjectMemory final
    {
    public:
        using F_Object = __F_Object;
        using F_ObjectManagement = __F_ObjectManagement;

    public:
        using F_ObjectHeader = TF_ManagedObjectHeader<__F_Object, __F_ObjectManagement>;

    public:
        /** The object header containing management metadata */
        F_ObjectHeader ObjectHeader;
        
        /** Storage for the actual object data */
        U8 ObjectData[sizeof(F_Object)];

        /**
         * @brief Gets a pointer to the object data.
         * @return Pointer to the object
         */
        ABYTEK_FORCE_INLINE F_Object* GetObjectRawP() const noexcept
        {
            return (F_Object*)ObjectData;
        }
    };
    
    template<class __F_Object>
    struct ABYTEK_ALIGN(ABYTEK_ALIGNOF(__F_Object)) TF_UniqueUnmanagedObjectMemory final
    {
    public:
        using F_Object = __F_Object;

    public:
        using F_ObjectHeader = TF_UniqueUnmanagedObjectHeader<__F_Object>;

    public:
        /** The object header containing management metadata */
        F_ObjectHeader ObjectHeader;
        
        /** Storage for the actual object data */
        U8 ObjectData[sizeof(F_Object)];

        /**
         * @brief Gets a pointer to the object data.
         * @return Pointer to the object
         */
        ABYTEK_FORCE_INLINE F_Object* GetObjectRawP() const noexcept
        {
            return (F_Object*)ObjectData;
        }
    };
    
    template<class __F_Object>
    struct ABYTEK_ALIGN(ABYTEK_ALIGNOF(__F_Object)) TF_SharedUnmanagedObjectMemory final
    {
    public:
        using F_Object = __F_Object;

    public:
        using F_ObjectHeader = TF_SharedUnmanagedObjectHeader<__F_Object>;

    public:
        /** The object header containing management metadata */
        F_ObjectHeader ObjectHeader;
        
        /** Storage for the actual object data */
        U8 ObjectData[sizeof(F_Object)];

        /**
         * @brief Gets a pointer to the object data.
         * @return Pointer to the object
         */
        ABYTEK_FORCE_INLINE F_Object* GetObjectRawP() const noexcept
        {
            return (F_Object*)ObjectData;
        }
    };
}