#include "Abytek/ActorComponents/InputComponent.hpp"
#include "Abytek/ApplicationUpdateFunction.hpp"
#include "Abytek/IHIMouseManager.hpp"
#include "Abytek/IHIKeyboardManager.hpp"
#include "Abytek/ActorComponents/InputComponentManager.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_InputComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_InputComponent"));
    }

    F_Name F_InputComponent::GetUpdateStatesUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_InputComponent::UpdateStates");
    }
    F_Name F_InputComponent::GetPostUpdateStatesUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_InputComponent::PostUpdateStates");
    }
    void F_InputComponent::GlobalInit()
    {
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_InputComponentManager::GetInstance()->UpdateStates();
                },
                GetUpdateStatesUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                A_IHIMouseManager::GetUpdateUpdateFunctionName()
            );
            UpdateFunction->AddDependency(
                A_IHIKeyboardManager::GetUpdateUpdateFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_HighLevelUpdateRange::GetBeginFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_InputComponentManager::GetInstance()->PostUpdateStates();
                },
                GetPostUpdateStatesUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                F_HighLevelUpdateRange::GetEndFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                A_IHIKeyboardManager::GetPostUpdateUpdateFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                A_IHIMouseManager::GetPostUpdateUpdateFunctionName()
            );
        }
    }
    void F_InputComponent::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetPostUpdateStatesUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetUpdateStatesUpdateFunctionName()
        );
    }

    F_InputComponent::F_InputComponent(const F_SerializableObjectInitParams& InitParams) :
        A_ActorComponent(InitParams)
    {
    }
    F_InputComponent::~F_InputComponent()
    {
    }

    void F_InputComponent::OnRegisterComponent()
    {
        A_ActorComponent::OnRegisterComponent();
        F_InputComponentManager::GetInstance()->_RegisterInputComponent(ABYTEK_WTHIS());
    }

    void F_InputComponent::OnUnregisterComponent()
    {
        F_InputComponentManager::GetInstance()->_UnregisterInputComponent(ABYTEK_WTHIS());
        A_ActorComponent::OnUnregisterComponent();
    }

    B8 F_InputComponent::IsButtonDown(const F_IHIButton& Button) const
    {
        return (
            std::find(
                _Buttons_Down.begin(),    
                _Buttons_Down.end(),
                Button
            )    
            != _Buttons_Down.end()
        );
    }
    B8 F_InputComponent::IsButtonPressed(const F_IHIButton& Button) const
    {
        return (
            std::find(
                _Buttons_Pressed.begin(),    
                _Buttons_Pressed.end(),
                Button
            )    
            != _Buttons_Pressed.end()
        );
    }
    B8 F_InputComponent::IsButtonUp(const F_IHIButton& Button) const
    {
        return (
            std::find(
                _Buttons_Up.begin(),    
                _Buttons_Up.end(),
                Button
            )    
            != _Buttons_Up.end()
        );
    }
}
