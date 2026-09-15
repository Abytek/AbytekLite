#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Frame/FrameCommon.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_Frame final : public A_Object
    {
    public:
        friend class F_FrameManager;
        
    private:
        E_FramesInFlightMode _FramesInFlightMode = E_FramesInFlightMode::NONE;
        U64 _Index = 0;
        TF_Vector<TF_ConcurrentQueue<F_FrameCommand>> _Queues;
        E_FrameParamType _CurrentParamType = E_FrameParamType::MAIN;
        TF_Optional<E_FramesInFlightMode> _LateFlushMode;
        
        TU_Unmanaged<F_Arena> _Arena;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetFramesInFlightMode() const noexcept
        {
            return _FramesInFlightMode;
        }   
        ABYTEK_FORCE_INLINE auto GetIndex() const noexcept
        {
            return _Index;
        }   
        ABYTEK_FORCE_INLINE auto GetCurrentParamType() const noexcept
        {
            return _CurrentParamType;
        }   
        ABYTEK_FORCE_INLINE auto IsCompleted() const noexcept
        {
            return _CurrentParamType == E_FrameParamType::NUM;
        }   
        ABYTEK_FORCE_INLINE const auto& GetLateFlushMode() const noexcept
        {
            return _LateFlushMode;
        }   
        void SetLateFlushMode(E_FramesInFlightMode FramesInFlightMode)
        {
            if (_LateFlushMode)
            {
                if (*_LateFlushMode < FramesInFlightMode)
                {
                    return;
                }
            }
            _LateFlushMode = FramesInFlightMode;
        }

        ABYTEK_FORCE_INLINE const auto& GetArena() const noexcept
        {
            return _Arena;
        }   
        
    public:
        F_Frame(E_FramesInFlightMode FramesInFlightMode, U64 Index);
        ~F_Frame() override;
        
    public:
        void EnqueueCommand(F_FrameCommand&& Command, E_FrameParamType ParamType);
        template<E_FrameParamType ParamType>
        void EnqueueCommand(F_FrameCommand&& Command)
        {
            EnqueueCommand(ABYTEK_MOVE(Command), ParamType);
        }
        
    public:
        void Flush(E_FrameParamType ParamType);
    };
}
