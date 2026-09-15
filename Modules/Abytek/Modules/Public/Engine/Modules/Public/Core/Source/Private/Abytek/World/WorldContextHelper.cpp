#include "Abytek/World/WorldContextHelper.hpp"


namespace Abytek
{
    F_FeedbackStatus H_WorldContext::ResolveAbsolutePath(
        const TW_Valid<A_WorldContext>& WorldContext,
        const F_Text& Raw, 
        F_Text& OutAbsolutePath
    )
    {
        return WorldContext->GetEnvironment()->ResolveAbsolutePath(Raw, OutAbsolutePath);
    }
    TS<F_SerializablePackage> H_WorldContext::EnsurePackage(
        const TW_Valid<A_WorldContext>& WorldContext,
        const F_Name& Name
    )
    {
        return WorldContext->GetEnvironment()->EnsurePackage(Name);
    }
    TW<A_SerializableObject> H_WorldContext::FindObject(
        const TW_Valid<A_WorldContext>& WorldContext,
        const F_Name& Name
    )
    {
        return WorldContext->GetEnvironment()->FindObject(Name);
    }
    TS<A_SerializableObject> H_WorldContext::CreateObject(
        const TW_Valid<A_WorldContext>& WorldContext,
        const F_Name& Name,
        const F_Name& PackageName,
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        return WorldContext->GetEnvironment()->CreateObject(
            Name,
            PackageName,
            Type
        );
    }
    B8 H_WorldContext::PopulateObject(
        const TW_Valid<A_WorldContext>& WorldContext,
        TS<A_SerializableObject>& OutObject,
        const F_Name& Name,
        const F_Name& PackageName,
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        return WorldContext->GetEnvironment()->PopulateObject(
            OutObject,
            Name,
            PackageName,
            Type
        );
    }

    B8 H_WorldContext::HasFlags(const TW_Valid<const A_WorldContext>& WorldContext, E_WorldFlag Flags)
    {
        return WorldContext->GetWorld()->HasFlags(Flags);
    }
    B8 H_WorldContext::HasFlagsAny(const TW_Valid<const A_WorldContext>& WorldContext, E_WorldFlag Flags)
    {
        return WorldContext->GetWorld()->HasFlagsAny(Flags);
    }
}
