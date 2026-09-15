#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHITemplate.hpp"
#include "Abytek/RHITemplateDatabase.hpp"


namespace Abytek
{
    struct ABYTEK_ENGINE_RHI_API H_RHITemplateUtilities
    {
        template<typename __F_Callback>
        static void BFSTraverse_OncePerItem(const TF_Vector<TW<A_RHITemplate>>& InitialTemplates, __F_Callback&& Callback)
        {
            TF_Set<TW<A_RHITemplate>> IteratedTemplates;
            TF_Queue<TW<A_RHITemplate>> TemplatesToIterate;
            for (const auto& Template : InitialTemplates)
            {
                TemplatesToIterate.Push(Template);
            }
            while (TemplatesToIterate.GetSize())
            {
                U32 NumIterate = TemplatesToIterate.GetSize();
                for (U32 Idx = 0; Idx < NumIterate; ++Idx)
                {
                    TW<A_RHITemplate> Template;
                    if (TemplatesToIterate.TryPop(Template))
                    {
                        if (IteratedTemplates.find(Template) != IteratedTemplates.end())
                        {
                            continue;
                        }
                        
                        if (!Callback(Template))
                        {
                            return;
                        }
                        IteratedTemplates.insert(Template);

                        for (const auto& DependencyHashCode : Template->GetDependencyHashCodes())
                        {
                            TemplatesToIterate.Push(Template->GetDatabase()->GetTemplate(DependencyHashCode).Weak());
                        }
                    }
                }
            }
        }
        template<typename __F_Callback>
        static void BFSTraverse_OncePerItem(const TW_Valid<A_RHITemplate>& InitialTemplate, __F_Callback&& Callback)
        {
            BFSTraverse_OncePerItem(TF_Vector<TW<A_RHITemplate>> { { InitialTemplate } }, ABYTEK_FORWARD(Callback));
        }
        template<typename __F_Callback>
        static void BFSTraverse(const TF_Vector<TW<A_RHITemplate>>& InitialTemplates, __F_Callback&& Callback)
        {
            TF_Queue<TW<A_RHITemplate>> TemplatesToIterate;
            for (const auto& Template : InitialTemplates)
            {
                TemplatesToIterate.Push(Template);
            }
            while (TemplatesToIterate.GetSize())
            {
                U32 NumIterate = TemplatesToIterate.GetSize();
                for (U32 Idx = 0; Idx < NumIterate; ++Idx)
                {
                    TW<A_RHITemplate> Template;
                    if (TemplatesToIterate.TryPop(Template))
                    {
                        if (!Callback(Template))
                        {
                            return;
                        }

                        for (const auto& DependencyHashCode : Template->GetDependencyHashCodes())
                        {
                            TemplatesToIterate.Push(Template->GetDatabase()->GetTemplate(DependencyHashCode).Weak());
                        }
                    }
                }
            }
        }
        template<typename __F_Callback>
        static void BFSTraverse(const TW_Valid<A_RHITemplate>& InitialTemplate, __F_Callback&& Callback)
        {
            BFSTraverse(TF_Vector<TW<A_RHITemplate>> {{ InitialTemplate }}, ABYTEK_FORWARD(Callback));
        }
        template<typename __F_Callback>
        static B8 DFSTraverse_OncePerItem(TF_Set<TW<A_RHITemplate>>& IteratedTemplates, const TW_Valid<A_RHITemplate>& InitialTemplate, __F_Callback&& Callback)
        {
            if (IteratedTemplates.find(InitialTemplate) != IteratedTemplates.end())
            {
                return true;
            }
            
            Callback(InitialTemplate);
            IteratedTemplates.insert(InitialTemplate);

            for (const auto& DependencyHashCode : InitialTemplate->GetDependencyHashCodes())
            {
                if (!DFSTraverse_OncePerItem(IteratedTemplates, InitialTemplate->GetDatabase()->GetTemplate(DependencyHashCode).Weak(), ABYTEK_FORWARD(Callback)))
                {
                    return false;
                }
            }
            return true;
        }
        template<typename __F_Callback>
        static B8 DFSTraverse_OncePerItem(const TW_Valid<A_RHITemplate>& InitialTemplate, __F_Callback&& Callback)
        {
            return DFSTraverse_OncePerItem({}, InitialTemplate, ABYTEK_FORWARD(Callback));
        }
        template<typename __F_Callback>
        static B8 DFSTraverse_OncePerItem(const TF_Vector<TW<A_RHITemplate>>& InitialTemplates, __F_Callback&& Callback)
        {
            TF_Set<TW<A_RHITemplate>> IteratedTemplates;
            for (const auto& InitialTemplate : InitialTemplates)
            {
                if (!DFSTraverse_OncePerItem(IteratedTemplates, InitialTemplate, ABYTEK_FORWARD(Callback)))
                {
                    return false;
                }
            }
            return true;
        }
        template<typename __F_Callback>
        static B8 DFSTraverse(const TW_Valid<A_RHITemplate>& InitialTemplate, __F_Callback&& Callback)
        {
            Callback(InitialTemplate);

            for (const auto& DependencyHashCode : InitialTemplate->GetDependencyHashCodes())
            {
                if (!DFSTraverse(InitialTemplate->GetDatabase()->GetTemplate(DependencyHashCode).Weak(), ABYTEK_FORWARD(Callback)))
                {
                    return false;
                }
            }
            return true;
        }
        template<typename __F_Callback>
        static B8 DFSTraverse(const TF_Vector<TW<A_RHITemplate>>& InitialTemplates, __F_Callback&& Callback)
        {
            for (const auto& InitialTemplate : InitialTemplates)
            {
                if (!DFSTraverse(InitialTemplate, ABYTEK_FORWARD(Callback)))
                {
                    return false;
                }
            }
            return true;
        }
    };
}
