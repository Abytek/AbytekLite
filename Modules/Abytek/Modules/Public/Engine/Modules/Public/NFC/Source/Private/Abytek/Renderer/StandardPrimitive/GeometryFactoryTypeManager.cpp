#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeManager.hpp"
#include "Abytek/Renderer/RendererManager.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType.hpp"
#include "Abytek/ApplicationModuleContainer.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_GeometryFactoryTypeManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_GeometryFactoryTypeManager"));
    }
    
    ABYTEK_DEFINE_STATIC_SUBSYSTEM(F_GeometryFactoryTypeManager);
    
    F_GeometryFactoryTypeManager::F_GeometryFactoryTypeManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_WorldSubsystem(BuildParams)
    {
        AddDependency<F_RendererManager>();
    }
    F_GeometryFactoryTypeManager::~F_GeometryFactoryTypeManager()
    {
    }

    void F_GeometryFactoryTypeManager::_RegisterType(const TS<A_GeometryFactoryType>& Type)
    {
        _Types.push_back(Type);
    }
    void F_GeometryFactoryTypeManager::_UnregisterType(const TS<A_GeometryFactoryType>& Type)
    {
        _Types.erase(
            std::find(
                _Types.begin(),    
                _Types.end(),
                Type
            )    
        );
    }

    void F_GeometryFactoryTypeManager::OnInit()
    {
        {
            TF_Vector<TF_ReflectionTypeHandle<A_GeometryFactoryType>> TypeHandles;
            
            auto BaseTypeHandle = TF_ReflectionTypeHandle<A_GeometryFactoryType>(F_ReflectionContext::GetGlobal());
            ABYTEK_ENGINE_NFC_ASSERT(BaseTypeHandle);
        
            F_ApplicationModuleContainer::GetInstance()->ForEachUnit(
                [&BaseTypeHandle, &TypeHandles](const TW_Valid<F_ProgramUnit>& Unit)
                {
                    auto Module = Unit.FastCast<F_Module>();
                    auto ReflectionSession = Module->GetReflectionSession();
                    ReflectionSession->ForEachTypeDerivedFrom(
                        BaseTypeHandle,
                        [&TypeHandles](const TW_Valid<F_ReflectionType>& Type)
                        {
                            if (Type->IsAbstract())
                            {
                                return true;
                            }
                            TypeHandles.push_back(Type);
                            return true;
                        }
                    );
                    return true;
                }
            );
            
            for (const auto& TypeHandle : TypeHandles)
            {
                auto TypeCanonical = TypeHandle->GetCanonical();
                auto HashCode = TypeHandle->GetHashCode();
                auto Type = H_WorldContext::CreateObjectDelayLoading(
                      GetWorld(),
                      ABYTEK_TEXT("Abytek::GeometryFactoryType_") + ToText(HashCode),
                      ABYTEK_TEXT("@") + *A_ApplicationCore::GetInstance()->GetName() + ABYTEK_TEXT("::Intermediate::Assets:/Abytek/Internal/Engine/NFC/GeometryFactoryTypes/") + ToText(HashCode),
                      TypeHandle
                );
                Type->CallLoad();
                _Types.push_back(Type);
                ABYTEK_LOG_INFO() << "Added geometry factory type, name: " << TypeCanonical << ", hash code: " << HashCode;
            }
        }
    }
    void F_GeometryFactoryTypeManager::OnRelease()
    {
        _Types = {};
    }
}
