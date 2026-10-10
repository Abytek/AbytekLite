#include "Abytek/Renderer/StandardPrimitive/MaterialTargetTypeManager.hpp"
#include "Abytek/Renderer/RendererManager.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetType.hpp"
#include "Abytek/ApplicationModuleContainer.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_MaterialTargetTypeManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialTargetTypeManager"));
    }
    
    ABYTEK_DEFINE_STATIC_SUBSYSTEM(F_MaterialTargetTypeManager);
    
    F_MaterialTargetTypeManager::F_MaterialTargetTypeManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_WorldSubsystem(BuildParams)
    {
        AddDependency<F_RendererManager>();
    }
    F_MaterialTargetTypeManager::~F_MaterialTargetTypeManager()
    {
    }

    void F_MaterialTargetTypeManager::_RegisterType(const TS<A_MaterialTargetType>& Type)
    {
        _Types.push_back(Type);
    }
    void F_MaterialTargetTypeManager::_UnregisterType(const TS<A_MaterialTargetType>& Type)
    {
        _Types.erase(
            std::find(
                _Types.begin(),    
                _Types.end(),
                Type
            )    
        );
    }

    void F_MaterialTargetTypeManager::OnInit()
    {
        {
            TF_Vector<TF_ReflectionTypeHandle<A_MaterialTargetType>> TypeHandles;
            
            auto BaseTypeHandle = TF_ReflectionTypeHandle<A_MaterialTargetType>(F_ReflectionContext::GetGlobal());
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
                      ABYTEK_TEXT("Abytek::MaterialTargetType_") + ToText(HashCode),
                      ABYTEK_TEXT("@") + *A_ApplicationCore::GetInstance()->GetName() + ABYTEK_TEXT("::Intermediate::Assets:/Abytek/Internal/Engine/NFC/MaterialTargetTypes/") + ToText(HashCode),
                      TypeHandle
                );
                Type->CallLoad();
                _Types.push_back(Type);
                ABYTEK_LOG_INFO() << "Added geometry factory type, name: " << TypeCanonical << ", hash code: " << HashCode;
            }
        }
    }
    void F_MaterialTargetTypeManager::OnRelease()
    {
        _Types = {};
    }
}
