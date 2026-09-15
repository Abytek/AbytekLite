#include "Abytek/RAObjectPool.hpp"
#include "Abytek/RAObject.hpp"


namespace Abytek
{
    namespace Internal
    {
        AB8 IsRAObjectPoolingEnabled = 0;
    }
    B8 IsRAObjectPoolingEnabled()
    {
        return Internal::IsRAObjectPoolingEnabled.load(boost::memory_order_acquire);
    }
    void EnableRAObjectPooling()
    {
        Internal::IsRAObjectPoolingEnabled.store(true, boost::memory_order_release);
    }
    void DisableRAObjectPooling()
    {
        Internal::IsRAObjectPoolingEnabled.store(false, boost::memory_order_release);
    }
    
    F_RAObjectPool::F_RAObjectPool(U32 Capacity) :
        _Capacity(Capacity)
    {
    }
    F_RAObjectPool::~F_RAObjectPool()
    {
        TU<A_RAObject> Object;
        while (_Objects.TryPop(Object));
    }

    TU<A_RAObject> F_RAObjectPool::Pop()
    {
        TU<A_RAObject> Object;
        if (IsRAObjectPoolingEnabled())
        {
            if (_Objects.TryPop(Object))
            {
                Object.SetObjectReleaser(
                    A_RAObject::_ReleaseObject
                );
#ifdef ABYTEK_DEBUG_INFO
                UntrackObject_InActive(Object.GetObjectRawP());
                TrackObject_Active(Object.GetObjectRawP());
#endif
                return ABYTEK_MOVE(Object);
            }
        }
        return CreateObject();
    }
    void F_RAObjectPool::Push(TU<A_RAObject>&& Object)
    {
        auto ObjectRawP = Object.GetObjectRawP();
        
        IncreaseSharedObjectCounter<F_StandardObjectManagement>(ObjectRawP);
        ABYTEK_BASE_OBJECT_ASSERT(CheckObject(Object.Weak()));
        if (Object->IsBuilt())
        {
            Object->Release();
        }
        
        Object.SetObjectReleaser(
            Object->GetDefaultObjectReleaser()
        );
        
#ifdef ABYTEK_DEBUG_INFO
        UntrackObject_Active(ObjectRawP);
#endif
        
        if (!IsRAObjectPoolingEnabled() || (_Objects.GetSize() >= _Capacity))
        {
            return;
        }
            
#ifdef ABYTEK_DEBUG_INFO
        TrackObject_InActive(ObjectRawP);
#endif
        _Objects.Push(ABYTEK_MOVE(Object));
    }
    
    TU<A_RAObject> F_RAObjectPool::CreateObject()
    {
        return {};
    }
}
