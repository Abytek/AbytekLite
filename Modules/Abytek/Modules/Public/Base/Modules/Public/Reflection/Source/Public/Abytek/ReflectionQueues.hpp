#pragma once

#include "Abytek/ReflectionCommon.hpp"
#include "Abytek/ReflectionData.hpp"


namespace Abytek
{
    namespace ReflectionCommands
    {
        using F_OnRegisterLinkedTypes = TF_Function<void()>;
        using F_OnPrepare = TF_Function<void()>;
        using F_OnReflectCanonicals = TF_Function<void()>;
        using F_OnReflectMembers = TF_Function<void()>;
        using F_OnReflectTypes = TF_Function<void()>;
    }
    struct F_ReflectionQueues
    {
        TF_Queue<ReflectionCommands::F_OnRegisterLinkedTypes> OnRegisterLinkedTypes;
        TF_Queue<ReflectionCommands::F_OnPrepare> OnPrepare;
        TF_Queue<ReflectionCommands::F_OnReflectCanonicals> OnReflectCanonicals;
        TF_Queue<ReflectionCommands::F_OnReflectMembers> OnReflectMembers;
        TF_Queue<ReflectionCommands::F_OnReflectTypes> OnReflectTypes;
    };
}