#pragma once

#include "WorldSubsystemContainer.hpp"
#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldContext.hpp"


namespace Abytek
{
    struct ABYTEK_ENGINE_CORE_API H_WorldContext
    {
        static F_FeedbackStatus ResolveAbsolutePath(
            const TW_Valid<A_WorldContext>& WorldContext,
            const F_Text& Raw, 
            F_Text& OutAbsolutePath
        );
        static TS<F_SerializablePackage> EnsurePackage(
            const TW_Valid<A_WorldContext>& WorldContext,
            const F_Name& Name
        );
        static TW<A_SerializableObject> FindObject(
            const TW_Valid<A_WorldContext>& WorldContext,
            const F_Name& Name
        );
        template<typename __F_Object>
        static TW<__F_Object> FindObject(
            const TW_Valid<A_WorldContext>& WorldContext,
            const F_Name& Name
        )
        {
            return WorldContext->GetEnvironment()->FindObject<__F_Object>(Name);
        }
        static TS<A_SerializableObject> CreateObject(
            const TW_Valid<A_WorldContext>& WorldContext,
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<A_SerializableObject>& Type = {}
        );
        template<typename __F_Object>
        static TS<__F_Object> CreateObject(
            const TW_Valid<A_WorldContext>& WorldContext,
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())
            )
        {
            return WorldContext->GetEnvironment()->CreateObject<__F_Object>(
                Name,
                PackageName,
                Type
            );
        }
        static B8 PopulateObject(
            const TW_Valid<A_WorldContext>& WorldContext,
            TS<A_SerializableObject>& OutObject,
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<A_SerializableObject>& Type = {}
        );
        template<typename __F_Object>
        static B8 PopulateObject(
            const TW_Valid<A_WorldContext>& WorldContext,
            TS<__F_Object>& OutObject,
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())
        )
        {
            return WorldContext->GetEnvironment()->PopulateObject<__F_Object>(
                OutObject,
                Name,
                PackageName,
                Type
            );
        }
        
        template<typename __F_Unit = F_ProgramUnit>
        static B8 HasUnit(const TW_Valid<const A_WorldContext>& WorldContext, const F_Name& Name = __F_Unit::GetStaticName()) noexcept
        {
            return WorldContext->GetWorld()->GetSubsystemContainer()->HasUnit<__F_Unit>(Name);
        }
        template<typename __F_Unit = F_ProgramUnit>
        static TW_Valid<__F_Unit> GetUnit(const TW_Valid<const A_WorldContext>& WorldContext, const F_Name& Name = __F_Unit::GetStaticName()) noexcept
        {
            return WorldContext->GetWorld()->GetSubsystemContainer()->GetUnit<__F_Unit>(Name);
        }
        template<typename __F_Unit = F_ProgramUnit>
        static TW<__F_Unit> FindUnit(const TW_Valid<const A_WorldContext>& WorldContext, const F_Name& Name = __F_Unit::GetStaticName()) noexcept
        {
            return WorldContext->GetWorld()->GetSubsystemContainer()->FindUnit<__F_Unit>(Name);
        }
        
        static B8 HasFlags(const TW_Valid<const A_WorldContext>& WorldContext, E_WorldFlag Flags);
        static B8 HasFlagsAny(const TW_Valid<const A_WorldContext>& WorldContext, E_WorldFlag Flags);
    };
}