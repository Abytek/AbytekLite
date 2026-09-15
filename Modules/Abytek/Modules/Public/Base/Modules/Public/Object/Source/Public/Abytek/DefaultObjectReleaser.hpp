#pragma once

#include "Abytek/ObjectBase.hpp"
#include "Abytek/ObjectMemory.hpp"
#include "Abytek/ObjectHeader.hpp"
#include "Abytek/ObjectTracking.hpp"


namespace Abytek::ObjectSmartPointerTemplates
{
    template<class __F_Object, class __F_Allocator, class __F_ObjectManagement>
    F_ObjectReleaser GetDefaultManagedObjectReleaser()
    {
        using F_ObjectKeyPool = typename __F_ObjectManagement::F_ObjectKeyPool;
        using E_ObjectTrackingFlag = typename __F_ObjectManagement::E_ObjectTrackingFlag;
        
        using F_ObjectHeader = TF_ManagedObjectHeader<__F_Object, __F_ObjectManagement>;
        using F_ObjectMemory = TF_ManagedObjectMemory<__F_Object, __F_ObjectManagement>;
            
        using F_ObjectMemoryAllocator = TF_RebindAllocator<F_ObjectMemory, __F_Allocator>;
            
        return [](void* BaseObjectRawP)
        {
            auto ObjectKey = Abytek::GetObjectKey<__F_ObjectManagement>(BaseObjectRawP);
            F_ObjectKeyPool::GetInstance().AddTrackingFlag(ObjectKey, E_ObjectTrackingFlag::EARLY_DESTRUCTED);
            
#ifdef ABYTEK_DEBUG_INFO
            UntrackObject_Active((A_Object*)BaseObjectRawP);
#endif
                
            ((__F_Object*)BaseObjectRawP)->~__F_Object();
                
            F_ObjectMemoryAllocator Allocator;
            Allocator.deallocate(
                (F_ObjectMemory*)(
                    ((F_ObjectHeader*)BaseObjectRawP)
                    - 1
                ),
                1
            );
                
            F_ObjectKeyPool::GetInstance().Deregister(ObjectKey);
        };
    }
}