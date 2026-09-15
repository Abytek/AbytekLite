#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/JSONOptions.hpp"
#include "Abytek/JSONContext.hpp"


namespace Abytek
{
    struct F_JSONContext
    {
        const F_JSONOptions* Options = nullptr;
        B8 IsTopLevel = false;
        
        static F_JSONContext Make(const F_JSONOptions& Options)
        {
            F_JSONContext Context;
            Context.Options = &Options;
            return Context;
        }
    };
}