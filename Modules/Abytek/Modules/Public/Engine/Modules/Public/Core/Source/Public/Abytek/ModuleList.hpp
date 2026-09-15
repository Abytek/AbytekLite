#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_ModuleList final : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_ModuleList)
        
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_ModuleList);
        
    private:
        TF_Set<F_Name> _Names;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetAll() const noexcept
        {
            return _Names;
        }
        ABYTEK_FORCE_INLINE auto Num() const noexcept
        {
            return _Names.size();
        }
        
    public:
        F_ModuleList();
        ~F_ModuleList() override;
    
    public:
        ABYTEK_FORCE_INLINE B8 Has(const F_Name& Name) const noexcept
        {
            return _Names.contains(Name);
        }
    };
}