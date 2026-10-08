#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_WorldContextDevelopmentData : public A_WorldContext
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_WorldContextDevelopmentData);
        
    private:
        F_Name _GUID;
        F_Text _DirectoryPath;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGUID() const noexcept
        {
            return _GUID;
        }
        ABYTEK_FORCE_INLINE const auto& GetDirectoryPath() const noexcept
        {
            return _DirectoryPath;
        }
        
    public:
        F_WorldContextDevelopmentData(const F_SerializableObjectInitParams& InitParams);
        ~F_WorldContextDevelopmentData() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    public:
        B8 CanSerialize(const TW_Valid<F_SerializableEnvironment>& Environment) const override;
    };
}
#endif
