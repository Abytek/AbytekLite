#include "Abytek/EmptySubsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_EmptySubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_EmptySubsystem"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_EmptySubsystem)
    
    F_EmptySubsystem::F_EmptySubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_EmptySubsystem::~F_EmptySubsystem()
    {
    }
    
    class F_Foo : public A_SerializableObject
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_SerializableObject)
        ABYTEK_END_REFLECTOR(F_Foo);
        
    public:
        U32 A = 3;
        TS<F_Foo> B;
        
    public:
        F_Foo(const F_SerializableObjectInitParams& InitParams) :
            A_SerializableObject(InitParams)
        {
        }
        ~F_Foo() override
        {
        }
        
    protected:
        void OnLoad() override
        {
            ABYTEK_LOG_INFO() << "Load: " << GetName();
        }
        void OnUnload() override
        {
            ABYTEK_LOG_INFO() << "Unload: " << GetName();
        }
    };
    
    class F_Foo2 : public F_Foo
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Foo)
        ABYTEK_END_REFLECTOR(F_Foo2)
        
    public:
        U32 C = 6;
        
    public:
        F_Foo2(const F_SerializableObjectInitParams& InitParams) :
            F_Foo(InitParams)
        {
        }
        ~F_Foo2() override
        {
        }
        
    protected:
        void OnLoad() override
        {
            ABYTEK_LOG_INFO() << "Load: " << GetName();
        }
        void OnUnload() override
        {
            ABYTEK_LOG_INFO() << "Unload: " << GetName();
        }
    };
    
    ABYTEK_REFLECT(F_Foo)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Foo"));
    
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(A);
        ABYTEK_REFLECT_PROPERTY_JSON_SERIALIZABLE(B);
    }
    
    ABYTEK_REFLECT(F_Foo2)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Foo2"));
    
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(C);
    }

    void F_EmptySubsystem::OnReflect()
    {
        RegisterStaticType<F_Foo>();
        RegisterStaticType<F_Foo2>();
    }
    
    void F_EmptySubsystem::OnStartup()
    {
        auto A = H_Serializable::CreateObject<F_Foo>(
            ABYTEK_NAME("A"),
            ABYTEK_NAME("@Abytek.Sandbox.EmptyApplication:/Demo")
        );
        auto B = H_Serializable::CreateObject<F_Foo2>(
            ABYTEK_NAME("B"),
            ABYTEK_NAME("@Abytek.Sandbox.EmptyApplication:/Demo")
        );
        A->A = 2;
        A->B = B;
        B->A = 1;
        B->C = 100;
        A->GetPackage()->Save();
    }
    void F_EmptySubsystem::OnShutdown()
    {
    }
}
