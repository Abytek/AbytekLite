#include "Abytek/RAObject.hpp"
#include "Abytek/RAObjectPool.hpp"


namespace Abytek
{
    namespace Internal
    {
        B8 GetRAObjectTryPushUnsafe(A_RAObject* ObjectRawP, F_StandardObjectKey ObjectKey)
        {
            if (auto Pool = ObjectRawP->GetPool())
            {
                Pool->Push(TU<A_RAObject>(ObjectRawP, ObjectKey));
                return true;
            }
            return false;
        }
    }

    A_RAObject::A_RAObject()
    {
        auto& ObjectReleaser = GetObjectReleaser<F_StandardObjectManagement>(this);
        _DefaultObjectReleaser = ObjectReleaser;
        ObjectReleaser = _ReleaseObject;
    }
    A_RAObject::~A_RAObject()
    {
    }

    void A_RAObject::_ReleaseObject(void* ObjectRawP)
    {
        A_RAObject* CastedObjectRawP = (A_RAObject*)ObjectRawP;
        CastedObjectRawP->RALowLevelReleaseUnsafe();
    }

    void A_RAObject::BuildMinimal()
    {
        if (_IsBuilt)
        {
            Release();
        }
        _IsBuilt = true;
    }
    
    void A_RAObject::Release()
    {
        _IsBuilt = false;
    }
    
    ABYTEK_RA_BEGIN_OBJECT(F_FakeRAObject)
    ABYTEK_RA_END_OBJECT()
}