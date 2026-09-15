#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    using F_RHISlangCustomizedParameterBinding = slang::SessionDesc::AbytekCustomizationConfig::ParameterBinding;
    using F_RHISlangCustomizedParameterBindingMap = std::map<std::string, F_RHISlangCustomizedParameterBinding>;
#endif
}