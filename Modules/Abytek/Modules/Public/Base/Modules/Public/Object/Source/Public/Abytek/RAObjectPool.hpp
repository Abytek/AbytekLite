#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/CrtUseObject.hpp"


namespace Abytek
{
    class A_RAObject;
    
    ABYTEK_BASE_OBJECT_API B8 IsRAObjectPoolingEnabled();
    ABYTEK_BASE_OBJECT_API void EnableRAObjectPooling();
    ABYTEK_BASE_OBJECT_API void DisableRAObjectPooling();

    class ABYTEK_BASE_OBJECT_API F_RAObjectPool : public A_Object
    {
    private:
        TF_ConcurrentQueue<TU<A_RAObject>> _Objects;
        U32 _Capacity = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetObjects() const noexcept
        {
            return _Objects;
        }
        ABYTEK_FORCE_INLINE auto& InjectObjects() noexcept
        {
            return _Objects;
        }
        ABYTEK_FORCE_INLINE auto GetCapacity() const noexcept
        {
            return _Capacity;
        }
        ABYTEK_FORCE_INLINE auto& InjectCapacity() noexcept
        {
            return _Capacity;
        }
        
    public:
        F_RAObjectPool(U32 Capacity = 256);
        virtual ~F_RAObjectPool();

#ifdef ABYTEK_BASE_CORE_ENABLE_ASSERTIONS
    public:
        virtual B8 CheckObject(const TW_Valid<A_RAObject>& Object) const
        {
            return true;
        }
#endif

    public:
        TU<A_RAObject> Pop();
        void Push(TU<A_RAObject>&& Object);

    public:
        virtual TU<A_RAObject> CreateObject();
    };

    template<class __F_Object>
    class TF_RAObjectPool final : public F_RAObjectPool
    {
    public:
        using F_Object = __F_Object;
        
    public:
        TF_RAObjectPool() = default;
        ~TF_RAObjectPool() override = default;

#ifdef ABYTEK_BASE_CORE_ENABLE_ASSERTIONS
    public:
        B8 CheckObject(const TW_Valid<A_RAObject>& Object) const override
        {
            return Object.CheckPolymorphism<F_Object>();
        }
#endif

    public:
        TU<A_RAObject> CreateObject() override
        {
            auto Result = __F_Object::Create();
            Result->InjectPool() = ABYTEK_WTHIS();
            return ABYTEK_MOVE(Result);
        }
    };
}

#define ABYTEK_RA_DECLARE_STATIC_OBJECT_POOL(...) \
            ABYTEK_PUBLIC_KEYWORD \
                static Abytek::TU<TF_RAObjectPool<__VA_ARGS__>> RAStaticObjectPoolInstance;

#define ABYTEK_RA_DEFINE_STATIC_OBJECT_POOL(...) \
            Abytek::TU<TF_RAObjectPool<__VA_ARGS__>> __VA_ARGS__::RAStaticObjectPoolInstance;

namespace Abytek
{
    template<class __F_Object>
    ABYTEK_FORCE_INLINE TW_Valid<TF_RAObjectPool<__F_Object>> RAGetStaticObjectPool()
    {
        return __F_Object::RAStaticObjectPoolInstance.Weak();
    }
    template<class __F_Object>
    ABYTEK_FORCE_INLINE TU<TF_RAObjectPool<__F_Object>>& RAInjectStaticObjectPool()
    {
        return __F_Object::RAStaticObjectPoolInstance;
    }
    template<class __F_Object>
    ABYTEK_FORCE_INLINE TW_Valid<TF_RAObjectPool<__F_Object>> RACreateStaticObjectPool()
    {
        auto OwnedPointer = TU<TF_RAObjectPool<__F_Object>>()();
        auto Pointer = OwnedPointer.Weak();
        RAInjectStaticObjectPool<__F_Object>() = ABYTEK_MOVE(OwnedPointer);
        return Pointer;
    }
    template<class __F_Object>
    ABYTEK_FORCE_INLINE void RADestroyStaticObjectPool()
    {
        RAInjectStaticObjectPool<__F_Object>().Reset();
    }

    template<class __F_Object>
    ABYTEK_FORCE_INLINE TU<__F_Object> RAPopStatic()
    {
        return RAGetStaticObjectPool<__F_Object>()->Pop().template FastCast<__F_Object>();
    }
    template<class __F_Object>
    ABYTEK_FORCE_INLINE void RAPushStatic(TU<__F_Object>&& Object)
    {
        RAGetStaticObjectPool<__F_Object>()->Push(ABYTEK_MOVE(Object));
    }

    template<class...>
    struct TF_RAScopedStaticObjectPoolRegister;

    template<>
    struct TF_RAScopedStaticObjectPoolRegister<>
    {
    };

    template<class __F_Object, class... __F_Rest>
    struct TF_RAScopedStaticObjectPoolRegister<__F_Object, __F_Rest...> : TF_RAScopedStaticObjectPoolRegister<__F_Rest...>
    {
        TF_RAScopedStaticObjectPoolRegister()
        {
            ABYTEK_BASE_OBJECT_ASSERT(!RAInjectStaticObjectPool<__F_Object>())
                << "RA static object pool of type "
                << TypeFullName<__F_Object>()
                << " was already registered";
            RACreateStaticObjectPool<__F_Object>();
        }
        ~TF_RAScopedStaticObjectPoolRegister()
        {
            RADestroyStaticObjectPool<__F_Object>();
        }
    };
}