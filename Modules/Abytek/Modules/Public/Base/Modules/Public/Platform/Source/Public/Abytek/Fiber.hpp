#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_Fiber final
    {
    private:
        boost::context::fiber _BoostFiber;

    public:
        ABYTEK_FORCE_INLINE auto&& GetBoostFiber() && noexcept
        {
            return ABYTEK_MOVE(_BoostFiber);
        }
        ABYTEK_FORCE_INLINE const auto& GetBoostFiber() const && noexcept
        {
            return _BoostFiber;
        }
        ABYTEK_FORCE_INLINE const auto& GetBoostFiber() const & noexcept
        {
            return _BoostFiber;
        }

    public:
        F_Fiber() = default;
        template<typename __F_PassedFunctor>
        F_Fiber(__F_PassedFunctor&& PassedFunctor, Sz StackSize = 128 * 1024 * 1024) :
            _BoostFiber(
                std::allocator_arg,
                boost::context::fixedsize_stack{StackSize},
                [Functor = ABYTEK_FORWARD(PassedFunctor)](boost::context::fiber&& BoostFiber)
                {
                    return Functor(
                        F_Fiber(ABYTEK_MOVE(BoostFiber))
                    ).GetBoostFiber();
                }
            )
        {
        }
        F_Fiber(boost::context::fiber&& BoostFiber) :
            _BoostFiber(ABYTEK_MOVE(BoostFiber))
        {
        }
        F_Fiber(F_Fiber&& X) noexcept :
            _BoostFiber(ABYTEK_MOVE(X._BoostFiber))
        {
        }
        F_Fiber& operator = (F_Fiber&& X) noexcept
        {
            _BoostFiber = ABYTEK_MOVE(X._BoostFiber);
            return *this;
        }
        ~F_Fiber() noexcept
        {
        }

    public:
        F_Fiber Resume() &&
        {
            return F_Fiber(ABYTEK_MOVE(_BoostFiber).resume());
        }
    };
}
