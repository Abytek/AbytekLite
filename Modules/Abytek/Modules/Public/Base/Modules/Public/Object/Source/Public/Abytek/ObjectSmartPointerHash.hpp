#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectWeakSmartPointerTemplate.hpp"
#include "Abytek/ObjectUniqueSmartPointerTemplate.hpp"
#include "Abytek/ObjectSharedSmartPointerTemplate.hpp"
#include "Abytek/UnmanagedObjectWeakSmartPointerTemplate.hpp"
#include "Abytek/UnmanagedObjectUniqueSmartPointerTemplate.hpp"
#include "Abytek/UnmanagedObjectSharedSmartPointerTemplate.hpp"

namespace boost
{
    template<
        typename __F_PassedObject,
        class __F_Allocator,
        class __F_Config
    >
    struct hash<Abytek::ObjectSmartPointerTemplates::TU<__F_PassedObject, __F_Allocator, __F_Config>>
    {
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TU<__F_PassedObject, __F_Allocator, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TS<__F_PassedObject, __F_Allocator, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TW<__F_PassedObject, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
    };

    template<
        typename __F_PassedObject,
        class __F_Allocator,
        class __F_Config
    >
    struct hash<Abytek::ObjectSmartPointerTemplates::TS<__F_PassedObject, __F_Allocator, __F_Config>>
    {
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TS<__F_PassedObject, __F_Allocator, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TW<__F_PassedObject, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
    };

    template<
        typename __F_PassedObject,
        class __F_Config
    >
    struct hash<Abytek::ObjectSmartPointerTemplates::TW<__F_PassedObject, __F_Config>>
    {
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TW<__F_PassedObject, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
    };

    template<
        typename __F_PassedObject,
        class __F_Allocator,
        class __F_Config
    >
    struct hash<Abytek::ObjectSmartPointerTemplates::TU_Unmanaged<__F_PassedObject, __F_Allocator, __F_Config>>
    {
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TU_Unmanaged<__F_PassedObject, __F_Allocator, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TS_Unmanaged<__F_PassedObject, __F_Allocator, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TW_Unmanaged<__F_PassedObject, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
    };

    template<
        typename __F_PassedObject,
        class __F_Allocator,
        class __F_Config
    >
    struct hash<Abytek::ObjectSmartPointerTemplates::TS_Unmanaged<__F_PassedObject, __F_Allocator, __F_Config>>
    {
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TS_Unmanaged<__F_PassedObject, __F_Allocator, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TW_Unmanaged<__F_PassedObject, __F_Config>& X) const
        {
            return X.GetObjectKey().Value;
        }
    };

    template<
        typename __F_PassedObject,
        class __F_Config
    >
    struct hash<Abytek::ObjectSmartPointerTemplates::TW_Unmanaged<__F_PassedObject, __F_Config>>
    {
        size_t operator()(const Abytek::ObjectSmartPointerTemplates::TW_Unmanaged<__F_PassedObject, __F_Config>& X) const
        {
            return X.GetObjectRawP();
        }
    };
}