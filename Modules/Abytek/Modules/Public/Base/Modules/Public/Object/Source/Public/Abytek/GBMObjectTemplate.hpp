#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectSmartPointerTemplates.hpp"


namespace Abytek
{
    enum class E_GBMObjectFlag : U8
    {
        NONE = 0x0,
        ROOT = 0x1,
        DIRTY = 0x2,
        DESTROY = 0x4,
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_GBMObjectFlag);
    
    struct F_GBMObjectCreationParams
    {
        E_GBMObjectFlag Flags = E_GBMObjectFlag::NONE;
    };
}

namespace Abytek::ObjectUtilityTemplates
{
    template<typename __F_Allocator, class __F_ObjectConfig>
    class TA_GBMObject;
    template<typename __F_Allocator, class __F_ObjectConfig>
    class TF_GBMObjectContainer;
    
    template<typename __F_Allocator, class __F_ObjectConfig>
    struct TF_GBMObjectConfig
    {
        ObjectSmartPointerTemplates::TW<TF_GBMObjectContainer<__F_Allocator, __F_ObjectConfig>, __F_ObjectConfig> Container;
        E_GBMObjectFlag Flags = E_GBMObjectFlag::NONE;
    };
    
    template<typename __F_Allocator, class __F_ObjectConfig>
    class TA_GBMObject : public A_Object
    {
    public:
        template<typename __F_AllocatorFr, class __F_ObjectConfigFr>
        friend class TF_GBMObjectContainer;
        
    public:
        using F_Allocator = __F_Allocator;
        using F_ObjectConfig = __F_ObjectConfig;
        
    public:
        using F_GBMContainer = TF_GBMObjectContainer<__F_Allocator, __F_ObjectConfig>;

    private:
        using U = ObjectSmartPointerTemplates::TU<TA_GBMObject, F_Allocator, F_ObjectConfig>;
        using W = ObjectSmartPointerTemplates::TW<TA_GBMObject, F_ObjectConfig>;
        using W_Valid = ObjectSmartPointerTemplates::TW_Valid<TA_GBMObject, F_ObjectConfig>;
        
        using W_Container = ObjectSmartPointerTemplates::TW<F_GBMContainer, F_ObjectConfig>;
        using W_ValidContainer = ObjectSmartPointerTemplates::TW_Valid<F_GBMContainer, F_ObjectConfig>;
        
        using F_References = ContainerTemplates::TF_Vector<W>;
        
    private:
        W_Container _GBMContainer;
        E_GBMObjectFlag _GBMFlags = E_GBMObjectFlag::NONE;
        F_References _GBMReferences;
        F_References _GBMReverseReferences;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GBMGetContainer() const noexcept
        {
            return _GBMContainer;
        }
        ABYTEK_FORCE_INLINE auto GBMGetFlags() noexcept
        {
            return _GBMFlags;
        }
        ABYTEK_FORCE_INLINE const auto& GBMGetReferences() const noexcept
        {
            return _GBMReferences;
        }
        ABYTEK_FORCE_INLINE const auto& GBMGetReverseReferences() const noexcept
        {
            return _GBMReverseReferences;
        }
        
    protected:
        TA_GBMObject(const TF_GBMObjectConfig<F_Allocator, F_ObjectConfig>& GBMConfig);
        
    public:
        ~TA_GBMObject() override;
        
    private:
        void _GBMMarkDirty();
        void _GBMUnmarkDirty();
        
    public:
        B8 GBMHasFlags(E_GBMObjectFlag Value)
        {
            return FlagHas(_GBMFlags, Value);
        }
        B8 GBMHasFlagsAny(E_GBMObjectFlag Value)
        {
            return FlagHasAny(_GBMFlags, Value);
        }
        void GBMAddFlags(E_GBMObjectFlag Value)
        {
            E_GBMObjectFlag ParsedValue = Value & ~E_GBMObjectFlag::DIRTY;
            _GBMFlags |= ParsedValue;
            _GBMMarkDirty();
        }
        void GBMRemoveFlags(E_GBMObjectFlag Value)
        {
            E_GBMObjectFlag ParsedValue = Value & ~E_GBMObjectFlag::DIRTY;
            _GBMFlags &= ~ParsedValue;
            _GBMMarkDirty();
        }
        
    public:
        B8 GBMMarkAsRoot()
        {
            B8 Result = !GBMHasFlags(E_GBMObjectFlag::ROOT);
            GBMAddFlags(E_GBMObjectFlag::ROOT);
            return Result;
        }
        B8 GBMUnmarkAsRoot()
        {
            B8 Result = GBMHasFlags(E_GBMObjectFlag::ROOT);
            GBMRemoveFlags(E_GBMObjectFlag::ROOT);
            return Result;
        }
        
    public:
        B8 GBMHasReverseReference(const W_Valid& Reference)
        {
            auto It = std::find(
                _GBMReverseReferences.begin(),
                _GBMReverseReferences.end(),
                Reference
            );
            return (It != _GBMReverseReferences.end());
        }
        
    public:
        B8 GBMHasReference(const W_Valid& Reference)
        {
            auto It = std::find(
                _GBMReferences.begin(),
                _GBMReferences.end(),
                Reference
            );
            return (It != _GBMReferences.end());
        }
        void GBMAddReference(const W_Valid& Reference)
        {
            ABYTEK_BASE_OBJECT_ASSERT(!GBMHasFlags(E_GBMObjectFlag::DESTROY));
            ABYTEK_BASE_OBJECT_ASSERT(!GBMHasReference(Reference));
            _GBMReferences.push_back(Reference);
            GBMAddFlags(E_GBMObjectFlag::DIRTY);
            
            {
                Reference->_GBMReverseReferences.push_back(
                    ObjectSmartPointerTemplates::GetWThis<TA_GBMObject, F_ObjectConfig>(this)
                );
                Reference->GBMAddFlags(E_GBMObjectFlag::DIRTY);
            }
        }
        void GBMRemoveReference(const W_Valid& Reference)
        {
            auto It = std::find(
                _GBMReferences.begin(),
                _GBMReferences.end(),
                Reference
            );
            ABYTEK_BASE_OBJECT_ASSERT(GBMHasReference(Reference));
            _GBMReferences.erase(It);
            if (!GBMHasFlags(E_GBMObjectFlag::DESTROY))
            {
                GBMAddFlags(E_GBMObjectFlag::DIRTY);
            }
            
            {
                auto ReverseIt = std::find(
                    Reference->_GBMReverseReferences.begin(),
                    Reference->_GBMReverseReferences.end(),
                    ObjectSmartPointerTemplates::GetWThis<TA_GBMObject, F_ObjectConfig>(this)
                );
                Reference->_GBMReverseReferences.erase(ReverseIt);
                Reference->GBMAddFlags(E_GBMObjectFlag::DIRTY);
            }
        }
        void GBMEnsureReference(const W_Valid& Reference)
        {
            if (GBMHasReference(Reference))
            {
                return;
            }
            GBMAddReference(Reference);
        }
        
    public:
        template<typename __F_Callback>
        void GBMBFSTraverseReferences(__F_Callback&& Callback, B8 Forward = true, B8 Reverse = false);
        B8 GBMBFSCheckReference(const W_Valid& Reference, B8 Forward = true, B8 Reverse = false);
    };
    
    template<typename __F_Allocator, class __F_ObjectConfig>
    class TF_GBMObjectContainer : public A_Object
    {
    public:
        template<typename __F_AllocatorFr, class __F_ObjectConfigFr>
        friend class TA_GBMObject;
        
    public:
        using F_Allocator = __F_Allocator;
        using F_ObjectConfig = __F_ObjectConfig;
        
    public:
        using A_GBMObject = TA_GBMObject<__F_Allocator, __F_ObjectConfig>;

    private:
        using U = ObjectSmartPointerTemplates::TU<TF_GBMObjectContainer, F_Allocator, F_ObjectConfig>;
        using W = ObjectSmartPointerTemplates::TW<TF_GBMObjectContainer, F_ObjectConfig>;
        using W_Valid = ObjectSmartPointerTemplates::TW_Valid<TF_GBMObjectContainer, F_ObjectConfig>;
        
        using S_Object = ObjectSmartPointerTemplates::TS<A_GBMObject, F_Allocator, F_ObjectConfig>;
        using S_ValidObject = ObjectSmartPointerTemplates::TS_Valid<A_GBMObject, F_Allocator, F_ObjectConfig>;
        using W_Object = ObjectSmartPointerTemplates::TW<A_GBMObject, F_ObjectConfig>;
        using W_ValidObject = ObjectSmartPointerTemplates::TW_Valid<A_GBMObject, F_ObjectConfig>;
        
    private:
        ContainerTemplates::TF_Set<S_Object, std::less<S_Object>, TF_RebindAllocator<S_Object, F_Allocator>> _Objects;
        ContainerTemplates::TF_Vector<W_Object, TF_RebindAllocator<W_Object, F_Allocator>> _DirtyObjects;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetObjects() const noexcept
        {
            return _Objects;
        }
        ABYTEK_FORCE_INLINE const auto& GetDirtyObjects() const noexcept
        {
            return _DirtyObjects;
        }
        
    public:
        TF_GBMObjectContainer();
        ~TF_GBMObjectContainer() override;
        
    private:
        void _AddObject(const S_ValidObject& Object);
        void _AddDirtyObject(const W_ValidObject& Object);
        
    public:
        void Flush();
        
    public:
        template<typename __F_Object, typename... __F_Args>
        auto CreateObject(const F_GBMObjectCreationParams& CreationParams, __F_Args&&... Args)
        {
            TF_GBMObjectConfig<F_Allocator, F_ObjectConfig> GBMObjectConfig;
            GBMObjectConfig.Container = ObjectSmartPointerTemplates::GetWThis<TF_GBMObjectContainer, F_ObjectConfig>(this);
            GBMObjectConfig.Flags = CreationParams.Flags;
            auto Object = ObjectSmartPointerTemplates::TS<__F_Object, F_Allocator, F_ObjectConfig>()(
                GBMObjectConfig,
                ABYTEK_FORWARD(Args)...    
            );
            return Object.Weak();
        }
        
    public:
        ABYTEK_FORCE_INLINE auto GetSize() const noexcept
        {
            return _Objects.size();
        }
        
    public:
        ABYTEK_FORCE_INLINE auto GetBegin() noexcept
        {
            return _Objects.begin();
        }
        ABYTEK_FORCE_INLINE auto GetEnd() noexcept
        {
            return _Objects.end();
        }
        ABYTEK_FORCE_INLINE auto GetBegin() const noexcept
        {
            return _Objects.cbegin();
        }
        ABYTEK_FORCE_INLINE auto GetEnd() const noexcept
        {
            return _Objects.cend();
        }
        ABYTEK_FORCE_INLINE auto GetReverseBegin() noexcept
        {
            return _Objects.rbegin();
        }
        ABYTEK_FORCE_INLINE auto GetReverseEnd() noexcept
        {
            return _Objects.rend();
        }
        ABYTEK_FORCE_INLINE auto GetReverseBegin() const noexcept
        {
            return _Objects.crbegin();
        }
        ABYTEK_FORCE_INLINE auto GetReverseEnd() const noexcept
        {
            return _Objects.crend();
        }
        
        ABYTEK_FORCE_INLINE auto begin() noexcept
        {
            return _Objects.begin();
        }
        ABYTEK_FORCE_INLINE auto end() noexcept
        {
            return _Objects.end();
        }
        ABYTEK_FORCE_INLINE auto begin() const noexcept
        {
            return _Objects.cbegin();
        }
        ABYTEK_FORCE_INLINE auto end() const noexcept
        {
            return _Objects.cend();
        }
        ABYTEK_FORCE_INLINE auto rbegin() noexcept
        {
            return _Objects.rbegin();
        }
        ABYTEK_FORCE_INLINE auto rend() noexcept
        {
            return _Objects.rend();
        }
        ABYTEK_FORCE_INLINE auto rbegin() const noexcept
        {
            return _Objects.crbegin();
        }
        ABYTEK_FORCE_INLINE auto rend() const noexcept
        {
            return _Objects.crend();
        }
    };

    template <typename __F_Allocator, class __F_ObjectConfig>
    TA_GBMObject<__F_Allocator, __F_ObjectConfig>::TA_GBMObject(const TF_GBMObjectConfig<F_Allocator, F_ObjectConfig>& GBMConfig) :
        _GBMContainer(GBMConfig.Container),
        _GBMFlags(GBMConfig.Flags)
    {
        _GBMContainer->_AddObject(
            ObjectSmartPointerTemplates::GetSThis<TA_GBMObject, F_Allocator, F_ObjectConfig>(this)
        );
        _GBMMarkDirty();
    }
    template <typename __F_Allocator, class __F_ObjectConfig>
    TA_GBMObject<__F_Allocator, __F_ObjectConfig>::~TA_GBMObject()
    {
    }

    template <typename __F_Allocator, class __F_ObjectConfig>
    void TA_GBMObject<__F_Allocator, __F_ObjectConfig>::_GBMMarkDirty()
    {
        _GBMContainer->_AddDirtyObject(
            ObjectSmartPointerTemplates::GetWThis<TA_GBMObject, F_ObjectConfig>(this)
        );
        _GBMFlags |= E_GBMObjectFlag::DIRTY;
    }
    template <typename __F_Allocator, class __F_ObjectConfig>
    void TA_GBMObject<__F_Allocator, __F_ObjectConfig>::_GBMUnmarkDirty()
    {
        _GBMFlags &= ~E_GBMObjectFlag::DIRTY;
    }
    
    template <typename __F_Allocator, class __F_ObjectConfig>
    template<typename __F_Callback>
    void TA_GBMObject<__F_Allocator, __F_ObjectConfig>::GBMBFSTraverseReferences(__F_Callback&& Callback, B8 Forward, B8 Reverse)
    {
        ContainerTemplates::TF_Set<
            W, 
            std::less<W>, 
            TF_RebindAllocator<W, F_Allocator>
        > TraversedObjects;
        TraversedObjects.insert(
            ObjectSmartPointerTemplates::GetWThis<TA_GBMObject, F_ObjectConfig>(this)
        );
        
        ContainerTemplates::TF_Set<
            W, 
            std::less<W>, 
            TF_RebindAllocator<W, F_Allocator>
        > ObjectsToTraverse;
        ObjectsToTraverse.insert(
            ObjectSmartPointerTemplates::GetWThis<TA_GBMObject, F_ObjectConfig>(this)
        );
            
        B8 ContinueTraversal = false;
            
        while ((ObjectsToTraverse.size() > 0) && !ContinueTraversal)
        {
            auto LastObjectsToTraverse = ABYTEK_MOVE(ObjectsToTraverse);
            for (const auto& ObjectToTraverse : LastObjectsToTraverse)
            {
                // process
                {
                    if (!Callback(ObjectToTraverse))
                    {
                        ContinueTraversal = false;
                        break;
                    }
                }
                    
                // references
                if (Forward)
                {
                    for (const auto& ReferencedObject : ObjectToTraverse->GBMGetReferences())
                    {
                        if (TraversedObjects.find(ReferencedObject) != TraversedObjects.end())
                        {
                            continue;
                        }
                        if (ObjectsToTraverse.find(ReferencedObject) != ObjectsToTraverse.end())
                        {
                            continue;
                        }
                        ObjectsToTraverse.insert(ReferencedObject);
                        TraversedObjects.insert(ReferencedObject);
                    }
                }
                if (Reverse)
                {
                    for (const auto& ReferencedObject : ObjectToTraverse->GBMGetReverseReferences())
                    {
                        if (TraversedObjects.find(ReferencedObject) != TraversedObjects.end())
                        {
                            continue;
                        }
                        if (ObjectsToTraverse.find(ReferencedObject) != ObjectsToTraverse.end())
                        {
                            continue;
                        }
                        ObjectsToTraverse.insert(ReferencedObject);
                        TraversedObjects.insert(ReferencedObject);
                    }
                }
            }
        }
    }
    template <typename __F_Allocator, class __F_ObjectConfig>
    B8 TA_GBMObject<__F_Allocator, __F_ObjectConfig>::GBMBFSCheckReference(const W_Valid& Reference, B8 Forward, B8 Reverse)
    {
        B8 Result = false;
        GBMBFSTraverseReferences(
            [&Reference, &Result](const auto& R)
            {
                if (R == Reference)
                {
                    Result = true;
                    return false;
                }
                return true;
            },
            Forward,
            Reverse
        );
        return Result;
    }
    
    template <typename __F_Allocator, class __F_ObjectConfig>
    TF_GBMObjectContainer<__F_Allocator, __F_ObjectConfig>::TF_GBMObjectContainer()
    {
    }
    template <typename __F_Allocator, class __F_ObjectConfig>
    TF_GBMObjectContainer<__F_Allocator, __F_ObjectConfig>::~TF_GBMObjectContainer()
    {
    }
    
    template <typename __F_Allocator, class __F_ObjectConfig>
    void TF_GBMObjectContainer<__F_Allocator, __F_ObjectConfig>::_AddObject(const S_ValidObject& Object)
    {
        if (_Objects.find(Object) != _Objects.end())
        {
            return;
        }
        _Objects.insert(Object);
    }
    template <typename __F_Allocator, class __F_ObjectConfig>
    void TF_GBMObjectContainer<__F_Allocator, __F_ObjectConfig>::_AddDirtyObject(const W_ValidObject& Object)
    {
        if (FlagHas(Object->GBMGetFlags(), E_GBMObjectFlag::DIRTY))
        {
            return;
        }
        _DirtyObjects.push_back(Object);
    }

    template <typename __F_Allocator, class __F_ObjectConfig>
    void TF_GBMObjectContainer<__F_Allocator, __F_ObjectConfig>::Flush()
    {
        using F_ObjectSet = ContainerTemplates::TF_Set<
            W_Object,
            std::less<W_Object>,
            TF_RebindAllocator<W_Object, F_Allocator>
        >;

        while (!_DirtyObjects.empty())
        {
            auto LastDirtyObjects = ABYTEK_MOVE(_DirtyObjects);

            F_ObjectSet ObjectsToDestroy;

            //------------------------------------------------------
            // Gather objects to destroy
            //------------------------------------------------------

            for (const auto& DirtyObject : LastDirtyObjects)
            {
                DirtyObject->_GBMUnmarkDirty();

                if (ObjectsToDestroy.find(DirtyObject) != ObjectsToDestroy.end())
                {
                    continue;
                }

                bool Reachable = false;
                DirtyObject->GBMBFSTraverseReferences(
                    [&](const W_Object& Current)
                    {
                        if (Current->GBMHasFlags(E_GBMObjectFlag::ROOT))
                        {
                            Reachable = true;
                            return false;
                        }
                        return true;
                    },
                    false,
                    true
                );

                if (!Reachable)
                {
                    DirtyObject->GBMBFSTraverseReferences(
                        [&](const W_Object& Current)
                        {
                            ObjectsToDestroy.insert(Current);
                            return true;
                        },
                        false,
                        true
                    );
                }
            }

            //------------------------------------------------------
            // Destroy objects
            //------------------------------------------------------

            for (const auto& Object : ObjectsToDestroy)
            {
                Object->_GBMFlags |= E_GBMObjectFlag::DESTROY;
                
                //
                // Remove outgoing references
                //
                auto References = Object->_GBMReferences;
                for (const auto& Reference : References)
                {
                    if (ObjectsToDestroy.find(Reference) != ObjectsToDestroy.end())
                    {
                        continue;
                    }
                    Object->GBMRemoveReference(Reference);
                }

                //
                // Remove from object container
                //
                auto Share =
                    ObjectSmartPointerTemplates::ShareObject<
                        A_GBMObject,
                        F_Allocator,
                        F_ObjectConfig
                    >(Object);

                auto It = _Objects.find(Share);

                if (It != _Objects.end())
                {
                    _Objects.erase(It);
                }
            }
        }
    }
}
