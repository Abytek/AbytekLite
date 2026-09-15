/**
 * @file LogBoostContainers.hpp
 * @brief Stream operators for logging Boost containers
 * 
 * This file provides stream operators for Boost container types that allow them
 * to be logged using the Abytek logging system.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/LogStream.hpp"


namespace Abytek::Operators
{
    /**
     * @brief Stream operator for logging boost::container::basic_string
     * @tparam __F_Char Character type
     * @tparam __F_CharTraits Character traits
     * @tparam __F_Allocator Allocator type
     * @param LogStream Log stream to output to
     * @param Value String to log
     * @return Reference to the log stream
     */
    template<typename __F_Char, typename __F_CharTraits, typename __F_Allocator>
    inline A_LogStream& operator << (
        A_LogStream& LogStream,
        const boost::container::basic_string<__F_Char, __F_CharTraits, __F_Allocator>& Value
    )
    {
        return (LogStream << Value.c_str());
    }
}