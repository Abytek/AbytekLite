#include "Abytek/TransientObjectSubsystem.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_TransientObjectManager);

    F_TransientObjectManager::F_TransientObjectManager()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_TransientObjectManager::~F_TransientObjectManager()
    {
    }

    void F_TransientObjectManager::_EnqueueObjectTracker(const F_ObjectTracker& Tracker)
    {
    }

    void F_TransientObjectManager::BeginFrame()
    {
    }
    void F_TransientObjectManager::EndFrame()
    {
    }

    WithCrtAllocator::TW_Valid<F_TransientObjectAdapter> F_TransientObjectManager::RegisterAdapter()
    {
        TF_ScopeLock<F_SpinLock> _(_AdapterLifetimeLock);
        
        auto OwnedAdapter = WithCrtAllocator::TU<F_TransientObjectAdapter>()();
        auto Adapter = OwnedAdapter.Weak();

        _Adapters.push_back(boost::move(OwnedAdapter));
        Adapter->_Handle = --(_Adapters.end());
        return Adapter;
    }
    void F_TransientObjectManager::DeregisterAdapter(const WithCrtAllocator::TW_Valid<F_TransientObjectAdapter>& Adapter)
    {
        _Adapters.erase(Adapter->_Handle);
    }

    F_TransientObjectAdapter::F_TransientObjectAdapter()
    {
        auto TransientMemoryManager = F_TransientMemoryManager::GetInstance();
        auto& Heap = TransientMemoryManager->GetHeap();
        
        memcpy(
            _ResidentFrames,
            Heap.GetChunkResidentFrames(),
            sizeof(I16) * static_cast<F_TransientMemoryParamIndex>(E_TransientMemoryParam::NUM)
        );
        for(
            F_TransientMemoryParamIndex ParamIdx = 0;
            ParamIdx < static_cast<F_TransientMemoryParamIndex>(E_TransientMemoryParam::NUM);
            ++ParamIdx
        )
        {
            I16 NumResidentFrames = _ResidentFrames[ParamIdx];
            
            auto& ObjectTrackers = _ObjectTrackers[ParamIdx];
            ObjectTrackers.resize(NumResidentFrames);
        }
    }
    F_TransientObjectAdapter::~F_TransientObjectAdapter()
    {
    }

    thread_local F_TransientObjectAdapter* CurrentTransientObjectAdapter;
    
    void F_TransientObjectAdapter::Bind()
    {
        _LastP = CurrentTransientObjectAdapter;
        CurrentTransientObjectAdapter = this;
    }
    void F_TransientObjectAdapter::Unbind()
    {
        CurrentTransientObjectAdapter = _LastP;
    }

    void F_TransientObjectAdapter::DestroyChunks()
    {
        for(
            F_TransientMemoryParamIndex ParamIdx = 0;
            ParamIdx < static_cast<F_TransientMemoryParamIndex>(E_TransientMemoryParam::NUM);
            ++ParamIdx
        )
        {
            I16 NumResidentFrames = _ResidentFrames[ParamIdx];
            
            auto& ObjectTrackers = _ObjectTrackers[ParamIdx];
            
            auto& TargetObjectTrackers = ObjectTrackers[NumResidentFrames - 1];
            for(auto& ObjectTracker : TargetObjectTrackers)
            {
                ObjectTracker.DestructorP(ObjectTracker.ObjectRawP);
            }
            TargetObjectTrackers = {};
            
            auto TempObjectTrackers = ObjectTrackers;
            for(I16 ResidentFrameIdx = 0; ResidentFrameIdx < NumResidentFrames; ++ResidentFrameIdx)
            {
                ObjectTrackers[ResidentFrameIdx] = TempObjectTrackers[
                    (ResidentFrameIdx + NumResidentFrames - 1) % NumResidentFrames
                ];
            }
        }
    }

    void F_TransientObjectAdapter::BeginFrame()
    {
        Bind();
    }
    void F_TransientObjectAdapter::EndFrame()
    {
        DestroyChunks();
        Unbind();
    }

    F_TransientObjectAdapter& F_TransientObjectAdapter::GetCurrent()
    {
        return *CurrentTransientObjectAdapter;
    }
}
