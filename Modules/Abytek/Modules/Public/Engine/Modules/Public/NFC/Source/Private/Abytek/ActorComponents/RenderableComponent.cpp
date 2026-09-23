#include "Abytek/ActorComponents/RenderableComponent.hpp"
#include "Abytek/ApplicationUpdateFunction.hpp"
#include "Abytek/ActorComponents/RenderableComponentManager.hpp"
#include "Abytek/ActorComponents/RenderableComponentUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_RenderableComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_RenderableComponent"));
    }

    F_Name A_RenderableComponent::GetCreateAndDestroyRenderStateUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_RenderableComponent::CreateAndDestroyRenderState");
    }
    F_Name A_RenderableComponent::GetUpdateRenderStateUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_RenderableComponent::UpdateRenderState");
    }
    void A_RenderableComponent::GlobalInit()
    {
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                },
                GetCreateAndDestroyRenderStateUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                F_RenderableComponentUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_RenderableComponentUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_RenderableComponentManager::GetInstance()->UpdateRenderStates();
                },
                GetUpdateRenderStateUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                GetCreateAndDestroyRenderStateUpdateFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_RenderableComponentUpdateRange::GetEndFunctionName()  
            );
        }
    }
    void A_RenderableComponent::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetUpdateRenderStateUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetCreateAndDestroyRenderStateUpdateFunctionName()
        );
    }

    B8 A_RenderableComponent::MarkRenderStateDirty()
    {
        auto Result = _IsRenderStateDirty;
        _IsRenderStateDirty = true;
        if (IsRegistered() && !Result)
        {
            _ActualMarkRenderStateDirty();
        }
        return Result;
    }
    
    A_RenderableComponent::A_RenderableComponent(const F_SerializableObjectInitParams& InitParams) :
        A_ActorComponent(InitParams)
    {
    }
    A_RenderableComponent::~A_RenderableComponent()
    {
    }

    void A_RenderableComponent::OnRegisterComponent()
    {
        SetupRenderable();
        F_RenderableComponentManager::GetInstance()->_RegisterRenderableComponent(ABYTEK_WTHIS());
        if (_IsRenderStateDirty)
        {
            _ActualMarkRenderStateDirty();
        }
        if (_ShouldUpdateRenderStateEveryFrame)
        {
            _ActualEnableUpdateRenderStateEveryFrame();
        }
        if (_ShouldUpdateRenderStateThisFrame)
        {
            _ActualMarkUpdateRenderStateThisFrame();
        }
    }
    void A_RenderableComponent::OnUnregisterComponent()
    {
        if (_ShouldUpdateRenderStateEveryFrame)
        {
            _ActualDisableUpdateRenderStateEveryFrame();
        }
        F_RenderableComponentManager::GetInstance()->_UnregisterRenderableComponent(ABYTEK_WTHIS());
        CleanUpRenderable();
    }

    void A_RenderableComponent::OnUpdateRenderState()
    {
    }

    void A_RenderableComponent::_UpdateRenderState()
    {
        OnUpdateRenderState();
    }

    void A_RenderableComponent::EnableUpdateRenderStateEveryFrame()
    {
        if (_ShouldUpdateRenderStateEveryFrame)
        {
            return;
        }
        _ShouldUpdateRenderStateEveryFrame = true;
        if (IsRegistered())
        {
            _ActualEnableUpdateRenderStateEveryFrame();
        }
    }
    void A_RenderableComponent::DisableUpdateRenderStateEveryFrame()
    {
        if (!_ShouldUpdateRenderStateEveryFrame)
        {
            return;
        }
        _ShouldUpdateRenderStateEveryFrame = false;
        if (IsRegistered())
        {
            _ActualDisableUpdateRenderStateEveryFrame();
        }
    }
    void A_RenderableComponent::MarkUpdateRenderStateThisFrame()
    {
        if (_ShouldUpdateRenderStateThisFrame)
        {
            return;
        }
        _ShouldUpdateRenderStateEveryFrame = true;
        if (IsRegistered())
        {
            _ActualMarkUpdateRenderStateThisFrame();
        }
    }

    void A_RenderableComponent::_ActualMarkRenderStateDirty()
    {
        auto UpdateFunction = H_UpdateUtilities::GetFunction(GetCreateAndDestroyRenderStateUpdateFunctionName());
        UpdateFunction->Queue.Push(
            [this]
            {
                if (CreatedRenderState())
                {
                    DestroyRenderState();
                }
                _IsRenderStateDirty = false;
                if (IsRenderable())
                {
                    CreateRenderState();
                }
            }
        );
    }
    void A_RenderableComponent::_ActualEnableUpdateRenderStateEveryFrame()
    {
        F_RenderableComponentManager::GetInstance()->_RegisterRenderableComponentToUpdateRenderStateEveryFrame(ABYTEK_WTHIS());
    }
    void A_RenderableComponent::_ActualDisableUpdateRenderStateEveryFrame()
    {
        F_RenderableComponentManager::GetInstance()->_UnregisterRenderableComponentFromUpdateRenderStateEveryFrame(ABYTEK_WTHIS());
    }
    void A_RenderableComponent::_ActualMarkUpdateRenderStateThisFrame()
    {
        if (_ShouldUpdateRenderStateEveryFrame)
        {
            return;
        }
        auto UpdateFunction = H_UpdateUtilities::GetFunction(GetUpdateRenderStateUpdateFunctionName());
        UpdateFunction->Queue.Push(
            [this]
            {
                _UpdateRenderState();
            }
        );
    }
}
