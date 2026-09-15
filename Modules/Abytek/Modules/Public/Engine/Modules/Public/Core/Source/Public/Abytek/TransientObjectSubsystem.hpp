#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/TransientAllocator.hpp"


namespace Abytek
{
    class F_TransientObjectAdapter;
    
    class ABYTEK_ENGINE_CORE_API F_TransientObjectManager final : public A_Object
    {
    public:
        friend class F_TransientObjectRunnable;

    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_TransientObjectManager);

    private:
        struct F_ObjectTracker
        {
            A_Object* ObjectRawP = 0;
            void (*DestructorP)(A_Object*) = 0;
        };

    private:
        F_SpinLock _AdapterLifetimeLock;
        WithCrtAllocator::TF_List<WithCrtAllocator::TU<F_TransientObjectAdapter>> _Adapters;

    public:

    public:
        F_TransientObjectManager();
        ~F_TransientObjectManager();

    private:
        void _EnqueueObjectTracker(const F_ObjectTracker& Tracker);

    public:
        void BeginFrame();
        void EndFrame();

    public:
        WithCrtAllocator::TW_Valid<F_TransientObjectAdapter> RegisterAdapter();
        void DeregisterAdapter(const WithCrtAllocator::TW_Valid<F_TransientObjectAdapter>& Adapter);
    };

    class ABYTEK_ENGINE_CORE_API F_TransientObjectAdapter : public A_Object
    {
    public:
        friend class F_TransientObjectManager;
        
    private:
        I16 _ResidentFrames[Sz(E_TransientMemoryParam::NUM)];
        struct F_ObjectTracker
        {
            A_Object* ObjectRawP = 0;
            void (*DestructorP)(A_Object*) = 0;
        };
        TF_Array<
            WithCrtAllocator::TF_Vector<
                WithCrtAllocator::TF_Vector<F_ObjectTracker>
            >,
            Sz(E_TransientMemoryParam::NUM)
        > _ObjectTrackers;

        WithCrtAllocator::TF_List<WithCrtAllocator::TU<F_TransientObjectAdapter>>::iterator _Handle;
        
        F_TransientObjectAdapter* _LastP;

    public:
        
    public:
        F_TransientObjectAdapter();
        ~F_TransientObjectAdapter();

    public:
        void Bind();
        void Unbind();
        void DestroyChunks();

    public:
        void BeginFrame();
        void EndFrame();

    public:
        static F_TransientObjectAdapter& GetCurrent();
        
    public:
        template<class __F_Object, E_TransientMemoryParam TransientMemoryParam, typename... F_Args>
        ABYTEK_FORCE_INLINE auto CreateObject(F_Args&&... Args)
        {
            TF_StaticTransientAllocator<__F_Object, TransientMemoryParam> Allocator;
             __F_Object* ObjectRawP = Allocator.New(ABYTEK_FORWARD(Args)...);

            F_ObjectTracker Tracker;
            Tracker.ObjectRawP = ObjectRawP;
            Tracker.DestructorP = [](A_Object* ObjectRawP)
            {
                ((__F_Object*)ObjectRawP)->~__F_Object();  
            };

            _ObjectTrackers[F_TransientMemoryParamIndex(TransientMemoryParam)][0].push_back(Tracker);

            return ObjectSmartPointerTemplates::TW_Unmanaged<__F_Object, TF_ObjectPointerConfig<F_StandardObjectManagement>>(ObjectRawP);
        }
    };

    template<class __F_Object, typename... F_Args>
    ABYTEK_FORCE_INLINE auto CreateTransientObject_Main(F_Args&&... Args)
    {
        return F_TransientObjectAdapter::GetCurrent().template CreateObject<__F_Object, E_TransientMemoryParam::MAIN>(
            ABYTEK_FORWARD(Args)...
        );
    }
    template<class __F_Object, typename... F_Args>
    ABYTEK_FORCE_INLINE auto CreateTransientObject_Render(F_Args&&... Args)
    {
        return F_TransientObjectAdapter::GetCurrent().template CreateObject<__F_Object, E_TransientMemoryParam::RENDER>(
            ABYTEK_FORWARD(Args)...
        );
    }
    template<class __F_Object, typename... F_Args>
    ABYTEK_FORCE_INLINE auto CreateTransientObject(F_Args&&... Args)
    {
        return CreateTransientObject_Main<__F_Object>(
            ABYTEK_FORWARD(Args)...
        );
    }
}
