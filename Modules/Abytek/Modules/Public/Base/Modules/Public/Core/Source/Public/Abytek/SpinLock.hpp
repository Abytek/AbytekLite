#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/SPSCSpinLock.hpp"
#include "Abytek/MPSCSpinLock.hpp"
#include "Abytek/SPMCSpinLock.hpp"
#include "Abytek/NoLock.hpp"


namespace Abytek
{
    /**
     * @brief Template class selecting appropriate spin lock implementation based on producer/consumer counts
     * 
     * Selects between different spin lock implementations based on whether there are
     * single or multiple producers and consumers.
     * 
     * @tparam __IsSingleProducer Whether there is a single producer (true) or multiple producers (false)
     * @tparam __IsSingleConsumer Whether there is a single consumer (true) or multiple consumers (false)
     */
    template<B8 __IsSingleProducer = true, B8 __IsSingleConsumer = true>
    class TF_SpinLock;

    /**
     * @brief Single Producer Single Consumer spin lock specialization
     */
    template<>
    class TF_SpinLock<true, true> : public F_SPSCSpinLock {};

    /**
     * @brief Multiple Producer Single Consumer spin lock specialization
     */
    template<>
    class TF_SpinLock<false, true> : public F_MPSCSpinLock {};

    /**
     * @brief Single Producer Multiple Consumer spin lock specialization
     */
    template<>
    class TF_SpinLock<true, false> : public F_SPMCSpinLock {};

    /**
     * @brief Multiple Producer Multiple Consumer spin lock specialization
     * 
     * Note: This currently uses the no-lock implementation.
     */
    template<>
    class TF_SpinLock<false, false> : public F_NoLock {};

    /**
     * @brief Default spin lock type (Single Producer Single Consumer)
     */
    using F_SpinLock = TF_SpinLock<>;
}