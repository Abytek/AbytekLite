#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/CrtUseObject.hpp"
#include "Abytek/RAObjectPool.hpp"


namespace Abytek
{
    class A_RAObject;
    class F_RAObjectPool;

    template<typename __F_Object>
    class TF_RAObjectPool;

    namespace Internal
    {
        ABYTEK_BASE_OBJECT_API B8 GetRAObjectTryPushUnsafe(A_RAObject* ObjectRawP, F_StandardObjectKey ObjectKey);
    }
    
    class ABYTEK_BASE_OBJECT_API A_RAObject : public A_Object
    {
    public:
        friend class F_RAObjectPool;
        
    private:
        B8 _IsBuilt = false;
        TW<F_RAObjectPool> _Pool;
        F_ObjectReleaser _DefaultObjectReleaser;

    public:
        ABYTEK_FORCE_INLINE B8 IsBuilt() const noexcept
        {
            return _IsBuilt;
        }
        ABYTEK_FORCE_INLINE const auto& GetPool() const noexcept
        {
            return _Pool;
        }
        ABYTEK_FORCE_INLINE auto& InjectPool() noexcept
        {
            return _Pool;
        }
        ABYTEK_FORCE_INLINE auto GetDefaultObjectReleaser() const noexcept
        {
            return _DefaultObjectReleaser;
        }
        
    public:
        A_RAObject();
        virtual ~A_RAObject();
        
    private:
        static void _ReleaseObject(void* ObjectRawP);
        
    protected:
        void BuildMinimal();  
        
    public:
        virtual void Release();

    public:
        virtual void RALowLevelReleaseUnsafe() = 0;
    };

    namespace Internal
    {
        template<class F_Object>
        void RALowLevelReleaseUnsafe(void* BaseObjectRawP)
        {
            F_Object* ObjectRawP = (F_Object*)BaseObjectRawP;

            auto ObjectKey = Abytek::GetObjectKey<F_StandardObjectManagement>(BaseObjectRawP);
                
            if (GetRAObjectTryPushUnsafe((A_RAObject*)ObjectRawP, ObjectKey))
            {
                return;
            }
            
            auto ObjectReleaser = ObjectSmartPointerTemplates::GetDefaultManagedObjectReleaser<
                F_Object,
                TF_DefaultAllocator<U8>,
                F_StandardObjectManagement
            >();
            ObjectReleaser(ObjectRawP);
        };
    }
}

#define ABYTEK_RA_DECLARE_CREATE_FUNCTION(ClassName) static Abytek::WithCrtAllocator::TU<ClassName> Create()
#define ABYTEK_RA_DECLARE_LOW_LEVEL_RELEASE_UNSAFE_FUNCTION(ClassName) \
            ABYTEK_PUBLIC_KEYWORD \
                virtual void RALowLevelReleaseUnsafe() override \
                { \
                    Abytek::Internal::RALowLevelReleaseUnsafe<ClassName>(this); \
                }
#define ABYTEK_RA_BEGIN_CREATE_FUNCTION(ClassName) Abytek::WithCrtAllocator::TU<ClassName> ClassName::Create() {
#define ABYTEK_RA_END_CREATE_FUNCTION() return {}; }
 
#define ABYTEK_RA_DEFAULT_OBJECT_MEMBERS(ClassName) \
            ClassName() = default; \
            virtual ~ClassName() override \
            { \
                if (IsBuilt()) \
                    Release(); \
            }

#define ABYTEK_RA_DECLARE_OBJECT_CREATABLE(ClassName) \
            ABYTEK_RA_DEFAULT_OBJECT_MEMBERS(ClassName); \
            ABYTEK_RA_DECLARE_LOW_LEVEL_RELEASE_UNSAFE_FUNCTION(ClassName); \
            ABYTEK_RA_DECLARE_STATIC_OBJECT_POOL(ClassName); \
            ABYTEK_RA_DECLARE_CREATE_FUNCTION(ClassName);

#define ABYTEK_RA_DECLARE_OBJECT(ClassName) \
            ABYTEK_RA_DEFAULT_OBJECT_MEMBERS(ClassName); \
            ABYTEK_RA_DECLARE_LOW_LEVEL_RELEASE_UNSAFE_FUNCTION(ClassName);

#define ABYTEK_RA_BEGIN_OBJECT(ClassName) \
            ABYTEK_RA_DEFINE_STATIC_OBJECT_POOL(ClassName); \
            ABYTEK_RA_BEGIN_CREATE_FUNCTION(ClassName);

#define ABYTEK_RA_END_OBJECT(ClassName) \
            ABYTEK_RA_END_CREATE_FUNCTION(ClassName);

#define ABYTEK_RA_OBJECT_DEFAULT(ClassName) \
            ABYTEK_RA_DEFINE_STATIC_OBJECT_POOL(ClassName); \
            Abytek::WithCrtAllocator::TU<ClassName> ClassName::Create() \
            { \
                return Abytek::TU<ClassName>()(); \
            }

namespace Abytek
{
    template<class __F_AbstractRA>
    TS_Valid<__F_AbstractRA> RACreateShared()
    {
        if (RAInjectStaticObjectPool<__F_AbstractRA>())
        {
            return RAPopStatic<__F_AbstractRA>();
        }
        return __F_AbstractRA::Create();
    }
    template<class __F_AbstractRA, typename... __F_Args>
    auto RACreateAndBuildShared(__F_Args&&... Args)
    {
        auto Result = RACreateShared<__F_AbstractRA>();
        Result->Build(ABYTEK_FORWARD(Args)...);
        return boost::move(Result);
    }
    
    template<class __F_AbstractRA>
    auto RACreate()
    {
        if (RAInjectStaticObjectPool<__F_AbstractRA>())
        {
            return RAPopStatic<__F_AbstractRA>();
        }
        return __F_AbstractRA::Create();
    }
    template<class __F_AbstractRA, typename... __F_Args>
    auto RACreateAndBuild(__F_Args&&... Args)
    {
        auto Result = RACreate<__F_AbstractRA>();
        Result->Build(ABYTEK_FORWARD(Args)...);
        return boost::move(Result);
    }
}

#define ABYTEK_RA_FCREATE(FunctionName, ...) \
            ( \
                [](auto&&... Args) \
                { \
                    auto Result = Abytek::RACreate<__VA_ARGS__>(); \
                    Result->FunctionName(ABYTEK_FORWARD(Args)...); \
                    return boost::move(Result); \
                } \
            )

#define ABYTEK_RA_FCREATE_SHARED(FunctionName, ...) \
            ( \
                [](auto&&... Args) \
                { \
                    auto Result = Abytek::RACreateShared<__VA_ARGS__>(); \
                    Result->FunctionName(ABYTEK_FORWARD(Args)...); \
                    return boost::move(Result); \
                } \
            )


namespace Abytek
{
    struct F_FakeRAObject : A_RAObject
    {
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_FakeRAObject);
    };
    namespace Internal::RAObject
    {
        template <typename __F_Object, typename = void>
        struct HasCreate : std::false_type
        {
        };
        template <typename __F_Object>
        struct HasCreate<
            __F_Object,
            std::void_t<decltype(&__F_Object::Create)>
        > : std::true_type
        {
        };
    }
    template<typename __F_Object>
    static constexpr B8 RACanCreateObject = std::is_base_of_v<A_RAObject, __F_Object> && Internal::RAObject::HasCreate<__F_Object>::value;
}