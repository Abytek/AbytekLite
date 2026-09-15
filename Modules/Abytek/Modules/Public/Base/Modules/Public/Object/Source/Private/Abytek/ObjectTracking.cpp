#include "Abytek/ObjectTracking.hpp"
#include "Abytek/ObjectBase.hpp"


#ifdef ABYTEK_DEBUG_INFO
namespace Abytek
{
    namespace Internal
    {
        AB8 IsObjectTrackingEnabled = false;
        F_SpinLock ObjectTrackingLock;
        ContainerTemplates::TF_Set<A_Object*> ObjectTrackingRawPs_Active;
        ContainerTemplates::TF_Set<A_Object*> ObjectTrackingRawPs_NonActive;
    }
    B8 IsObjectTrackingEnabled()
    {
        return Internal::IsObjectTrackingEnabled.load(boost::memory_order_acquire);
    }
    void EnableObjectTracking()
    {
        Internal::IsObjectTrackingEnabled.store(true, boost::memory_order_release);
    }
    void LockObjectTracking()
    {
        Internal::ObjectTrackingLock.ProducerLock();
    }
    void UnlockObjectTracking()
    {
        Internal::ObjectTrackingLock.ProducerUnlock();
    }
    void TrackObject_Active(A_Object* ObjectRawP)
    {
        if (!IsObjectTrackingEnabled())
        {
            return;
        }
        LockObjectTracking();
        if (Internal::ObjectTrackingRawPs_NonActive.find(ObjectRawP) == Internal::ObjectTrackingRawPs_NonActive.end())
        {
            Internal::ObjectTrackingRawPs_Active.insert(ObjectRawP);
        }
        UnlockObjectTracking();
    }
    void UntrackObject_Active(A_Object* ObjectRawP)
    {
        if (!IsObjectTrackingEnabled())
        {
            return;
        }
        LockObjectTracking();
        auto It = Internal::ObjectTrackingRawPs_Active.find(ObjectRawP);
        if (It != Internal::ObjectTrackingRawPs_Active.end())
        {
            Internal::ObjectTrackingRawPs_Active.erase(It);
        }
        UnlockObjectTracking();
    }
    void TrackObject_InActive(A_Object* ObjectRawP)
    {
        if (!IsObjectTrackingEnabled())
        {
            return;
        }
        LockObjectTracking();
        if (Internal::ObjectTrackingRawPs_Active.find(ObjectRawP) == Internal::ObjectTrackingRawPs_Active.end())
        {
            Internal::ObjectTrackingRawPs_NonActive.insert(ObjectRawP);
        }
        UnlockObjectTracking();
    }
    void UntrackObject_InActive(A_Object* ObjectRawP)
    {
        if (!IsObjectTrackingEnabled())
        {
            return;
        }
        LockObjectTracking();
        auto It = Internal::ObjectTrackingRawPs_NonActive.find(ObjectRawP);
        if (It != Internal::ObjectTrackingRawPs_NonActive.end())
        {
            Internal::ObjectTrackingRawPs_NonActive.erase(It);
        }
        UnlockObjectTracking();
    }
    
    TF_Vector<A_Object*> GetTrackedObjects(B8 ActiveObjects, B8 NonActiveObjects)
    {
        if (!IsObjectTrackingEnabled())
        {
            return {};
        }
        TF_Vector<A_Object*> Result;
        TF_Set<A_Object*> Set;
        if (ActiveObjects)
        {
            for (auto ObjectRawP : Internal::ObjectTrackingRawPs_Active)
            {
                if (Set.find(ObjectRawP) == Set.end())
                {
                    Set.insert(ObjectRawP);
                }
            }
        }
        if (NonActiveObjects)
        {
            for (auto ObjectRawP : Internal::ObjectTrackingRawPs_NonActive)
            {
                if (Set.find(ObjectRawP) == Set.end())
                {
                    Set.insert(ObjectRawP);
                }
            }
        }
        for (auto ObjectRawP : Set)
        {
            Result.push_back(ObjectRawP);
        }
        return ABYTEK_MOVE(Result);
    }

    F_Text GenerateObjectTrackingLog(B8 ActiveObjects, B8 NonActiveObjects)
    {
        F_Text Result;
        LockObjectTracking();
        auto ObjectRawPs = GetTrackedObjects(ActiveObjects, NonActiveObjects);
        U32 Index = 0;
        for (auto ObjectRawP : ObjectRawPs)
        {
            Result += ABYTEK_TEXT("[");
            Result += ToText(Index);
            Result += ABYTEK_TEXT("]: ");
            {
                std::stringstream SS;
                SS << ObjectRawP;  
                Result += ToText(SS.str().c_str());
            }
            Result += ABYTEK_TEXT("@");
            Result += ToText(*ObjectRawP->GetDebugName());
            Result += ToText(F_NewLine {});
            ++Index;
        }
        UnlockObjectTracking();
        return ABYTEK_MOVE(Result);
    }
    void LogObjectTracking(B8 ActiveObjects, B8 NonActiveObjects)
    {
        ABYTEK_LOG_INFO() << GenerateObjectTrackingLog(ActiveObjects, NonActiveObjects);
    }
}
#endif
