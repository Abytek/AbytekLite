#pragma once

#include "Abytek/Engine.Resource.prerequisites.hpp"
#include "Abytek/TaggedPath.hpp"
#include "Abytek/Resource.hpp"
#include "Abytek/ResourceSubsystem.hpp"
#include "Abytek/RootResource.hpp"


namespace Abytek
{
    class F_RootResource;
    class F_Resource;

    namespace Internal
    {
        extern ABYTEK_ENGINE_RESOURCE_API AU32 RandomResourceCounter;
    }
    static F_Name GenerateRandomResourceName();
    
    template<E_FrameParamType __FrameParamType>
    struct TH_ResourceUtilities
    {
        static TW_Valid<F_ResourceManager> GetManager()
        {
            return F_ResourceSubsystem::GetInstance()->GetManager(__FrameParamType);
        }
        static TS<F_RootResource> GetRoot()
        {
            return GetManager()->GetRootResource();
        }
        static TS<F_Resource> Query(const F_TaggedPath& Path)
        {
            if (Path.IsValid())
            {
                return GetRoot()->Query(Path);
            }
            return GetRoot();
        }
        template<class __F_Resource>
        static TS<__F_Resource> Query(const F_TaggedPath& Path)
        {
            if (auto TempResource = Query(Path))
            {
                if (TempResource.template CheckPolymorphism<__F_Resource>())
                {
                    return TempResource.template FastCast<__F_Resource>();
                }
            }
            return {};
        }
        static TS<F_Resource> Quick(const F_Name& ShortcutFullName)
        {
            return GetManager()->FindByShortcut(ShortcutFullName);
        }
        static void Remove(const F_TaggedPath& Path)
        {
            auto Resource = Query(Path);
            ABYTEK_ENGINE_RESOURCE_ASSERT(Resource) << "Not found resource at path: " << *Path;
            Resource->GetParent()->RemoveChild(Path.Tags.back());
        }
        static void Bind(const F_TaggedPath& Path, const TS_Valid<F_Resource>& Resource)
        {
            auto ResolvedPath = Resource->ResolvePathToCreate(Path);
    
            F_TaggedPath ParentPath = ResolvedPath - 1;
            auto Parent = Query(ParentPath);
            if (!Parent)
            {
                Parent = Create(ParentPath);
            }
            Parent->AddChild(ResolvedPath.Tags.back(), Resource);
        }
        template<class __F_Resource, typename... __F_Args>
        static TS_Valid<__F_Resource> Create(const F_TaggedPath& Path, __F_Args&&... Args)
        {
            auto Resource = TS<__F_Resource>()(ABYTEK_FORWARD(Args)...);
            auto ResolvedPath = Resource->ResolvePathToCreate(Path);
            
            F_TaggedPath ParentPath = ResolvedPath - 1;
            auto Parent = Ensure(ParentPath);
            Parent->AddChild(ResolvedPath.Tags.back(), Resource);
            return Resource;
        }
        static TS_Valid<F_Resource> Create(const F_TaggedPath& Path)
        {
            return Create<F_Resource>(Path);
        }
        static TS_Valid<F_Resource> Ensure(const F_TaggedPath& Path)
        {
            F_TaggedPath ParentPath = Path - 1;
            auto Parent = Query(ParentPath);
            if (!Parent)
            {
                Parent = Ensure(ParentPath);
            }
            
            F_Name Name = Path.Tags.back();
            if (Parent->HasChild(Name))
            {
                return Parent->FindChild(Name);
            }
            auto Resource = TS<F_Resource>()();
            Parent->AddChild(Name, Resource);
            return Resource;
        }
        template<typename __F_Callback>
        static void BFSTraverse(const TS<F_Resource>& Resource0, __F_Callback&& Callback)
        {
            if (!Resource0)
            {
                return;
            }
            TF_Queue<TS<F_Resource>> Resources;
            Resources.Push(Resource0);
            while (Resources.GetSize() > 0)
            {
                auto Num = Resources.GetSize();
                for (U32 Idx = 0; Idx < Num; ++Idx)
                {
                    TS<F_Resource> Resource = Resources.GetFront();
                    Resources.Pop();

                    if (!Callback(Resource))
                    {
                        return;
                    }

                    for (const auto& It : Resource->GetChilds())
                    {
                        Resources.Push(It);
                    }
                }
            }
        }
        template<typename __F_Callback>
        static void DFSTraverse(const TS<F_Resource>& Resource, __F_Callback&& Callback)
        {
            if (!Resource)
            {
                return;
            }
            if (!Callback(Resource))
            {
                return;
            }

            for (const auto& It : Resource->GetChilds())
            {
                DFSTraverse(It, ABYTEK_FORWARD(Callback));
            }
        }
        template<typename __F_Callback>
        static void BFSTraverse(const F_TaggedPath& Path, __F_Callback&& Callback)
        {
            BFSTraverse(Query(Path), ABYTEK_FORWARD(Callback));
        }
        template<typename __F_Callback>
        static void DFSTraverse(const F_TaggedPath& Path, __F_Callback&& Callback)
        {
            DFSTraverse(Query(Path), ABYTEK_FORWARD(Callback));
        }
    };
    using H_ResourceUtilities = TH_ResourceUtilities<E_FrameParamType::DEFAULT>;
}
