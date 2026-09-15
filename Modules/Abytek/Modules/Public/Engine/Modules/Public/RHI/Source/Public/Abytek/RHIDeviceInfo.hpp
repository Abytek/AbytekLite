#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    struct F_RHIDeviceInfo
    {
        /** @brief Unique identifier for the adapter */
        WithCrtAllocator::F_Text Id;
        
        /** @brief Human-readable description of the adapter */
        WithCrtAllocator::F_Text Description;
        
        /** @brief Vendor-specific identifier (e.g., 0x10DE for NVIDIA, 0x1002 for AMD) */
        U32 VendorId = 0;
        
        /** @brief Device-specific identifier assigned by the vendor */
        U32 DeviceId = 0;
        
        /** @brief Subsystem identifier for additional device classification */
        U32 SubsystemId = 0;
        
        /** @brief Revision number of the adapter hardware */
        U32 Revision = 0;
        
        /** @brief Size of dedicated video memory in bytes */
        Sz DedicatedVideoMemory = 0;
        
        /** @brief Size of dedicated system memory in bytes */
        Sz DedicatedSystemMemory = 0;
        
        /** @brief Size of shared system memory in bytes */
        Sz SharedSystemMemory = 0;
    };
}