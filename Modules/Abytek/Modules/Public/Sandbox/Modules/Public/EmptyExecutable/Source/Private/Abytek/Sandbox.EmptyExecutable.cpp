#include "Abytek/Sandbox.EmptyExecutable.prerequisites.pch.hpp"


namespace Abytek
{
    class F_EmptyExecutable : public F_Executable
    {
    public:
        F_EmptyExecutable(const F_ExecutableInput& Input);
        ~F_EmptyExecutable() override = default;

    protected:
        virtual void OnStartup() override;
    };
    
    F_EmptyExecutable::F_EmptyExecutable(const F_ExecutableInput& Input) :
        F_Executable(Input)
    {
    }
    
    template<typename __F_Value>
    struct F_Foo 
    {
        static B8 Invoke(__F_Value& Value)
        {
            ABYTEK_LOG_INFO() << ABYTEK_TEXT("Foo: ") << ToText(TypeFullName<__F_Value>());      
            return true;
        }
    };
    template<>
    struct F_Foo<U32>
    {
        static B8 Invoke(U32& Value)
        {
            Value = 8;
            return true;
        }
    };
    
    void F_EmptyExecutable::OnStartup()
    {
        F_Executable::OnStartup();
        
        auto ReflectionContext = TU<F_ReflectionContext>()();
        F_ReflectionContext::SetGlobal(ReflectionContext.Weak());
        auto ReflectionSession = ReflectionContext->CreateSession({});
        ReflectionSession->Begin();
        SetupReflectionSession_Base(ReflectionSession);
        ReflectionSession->End();
        
        TF_Vector<U32> Data;
        Data.push_back(3);
        Data.push_back(1);
        DataTraverse<F_Foo>(Data);
        
        F_Vector4 V = { 3.0f, 1.0f, 5.0f, 0.0f };
        F_Vector4 V2;
        
        F_Archive Archive;
        auto RWView = F_ArchiveReadWriteView::From(Archive);
        RWView << V;
        auto ROView = RWView.GetReadOnly();
        ROView >> V2;
        
        F_Text VJSON;
        H_JSON::Stringify(V2, VJSON);
        ABYTEK_LOG_INFO() << VJSON;
        
        F_ReflectionContext::SetGlobal({});
    }
}

ABYTEK_DEFINE_EXECUTABLE(Abytek::F_EmptyExecutable);
    