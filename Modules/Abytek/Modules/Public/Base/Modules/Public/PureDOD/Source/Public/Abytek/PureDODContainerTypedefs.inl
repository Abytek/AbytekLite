/**
 * @file PureDODContainerTypedefs.inl
 * 
 * @brief Defines convenient type aliases for the Pure DOD containers using default allocators.
 * 
 * This file provides common typedefs for the Pure DOD container templates with default allocator
 * configurations. The namespace for the typedefs can be customized by defining
 * ABYTEK_PURE_DOD_CONTAINER_TYPEDEFS_NAMESPACE before including this file.
 */

#include "Abytek/Base.PureDOD.prerequisites.pch.hpp"
#include "Abytek/IdedDataTable.hpp"
#include "Abytek/PayloadDataTable.hpp"
#include "Abytek/RuntimeIdedDataTable.hpp"
#include "Abytek/RuntimePayloadDataTable.hpp"

#ifndef ABYTEK_PURE_DOD_CONTAINER_TYPEDEFS_NAMESPACE
#define ABYTEK_PURE_DOD_CONTAINER_TYPEDEFS_NAMESPACE Abytek
#endif

namespace ABYTEK_PURE_DOD_CONTAINER_TYPEDEFS_NAMESPACE
{
    /**
     * @brief Type alias for a PayloadDataTable with the default allocator.
     * 
     * @tparam __F_Values The types stored in each column of the table
     */
    template<typename... __F_Values>
    using TF_PayloadDataTable = ContainerTemplates::TF_PayloadDataTable<TF_DefaultAllocator<U8>, __F_Values...>;
    
    /**
     * @brief Type alias for an IdedDataTable with the default allocator and U32 as the payload index type.
     * 
     * @tparam __F_Values The types stored in each column of the table
     */
    template<typename... __F_Values>
    using TF_IdedDataTable = ContainerTemplates::TF_IdedDataTable<TF_DefaultAllocator<U8>, U32, __F_Values...>;
    
    /**
     * @brief Type alias for a RuntimePayloadDataTable with the default allocator.
     */
    using F_RuntimePayloadDataTable = ContainerTemplates::TF_RuntimePayloadDataTable<TF_DefaultAllocator<U8>>;
    
    /**
     * @brief Type alias for a RuntimeIdedDataTable with the default allocator and U32 as the payload index type.
     */
    using F_RuntimeIdedDataTable = ContainerTemplates::TF_RuntimeIdedDataTable<TF_DefaultAllocator<U8>, U32>;
}

#undef ABYTEK_PURE_DOD_CONTAINER_TYPEDEFS_NAMESPACE