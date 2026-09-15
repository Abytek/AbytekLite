#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"


#ifdef ABYTEK_DEBUG_INFO
namespace Abytek
{
    class A_Object;

    ABYTEK_BASE_OBJECT_API B8 IsObjectTrackingEnabled();
    ABYTEK_BASE_OBJECT_API void EnableObjectTracking();
    ABYTEK_BASE_OBJECT_API void LockObjectTracking();
    ABYTEK_BASE_OBJECT_API void UnlockObjectTracking();
    ABYTEK_BASE_OBJECT_API void TrackObject_Active(A_Object* ObjectRawP);
    ABYTEK_BASE_OBJECT_API void UntrackObject_Active(A_Object* ObjectRawP);
    ABYTEK_BASE_OBJECT_API void TrackObject_InActive(A_Object* ObjectRawP);
    ABYTEK_BASE_OBJECT_API void UntrackObject_InActive(A_Object* ObjectRawP);
    
    // No locking => Non-thread-safe
    ABYTEK_BASE_OBJECT_API TF_Vector<A_Object*> GetTrackedObjects(B8 ActiveObjects = true, B8 NonActiveObjects = false);
    
    // Object tracking utilities
    ABYTEK_BASE_OBJECT_API F_Text GenerateObjectTrackingLog(B8 ActiveObjects = true, B8 NonActiveObjects = false);
    ABYTEK_BASE_OBJECT_API void LogObjectTracking(B8 ActiveObjects = true, B8 NonActiveObjects = false);
}
#endif