#pragma once

#include "Abytek/Engine.Assets.prerequisites.hpp"


namespace Abytek
{
    ABYTEK_ENGINE_ASSETS_API TF_Optional<F_Text> GetModuleContentDirectory(const F_Name& ModuleName);
    
    enum class E_AssetPathType : U8
    {
        NONE,
        NORMAL,
        MODULED
    };
    struct F_AssetPathInfo
    {
        E_AssetPathType Type;
        F_Name ModuleName;
        F_Text Path;
        
        static F_AssetPathInfo Parse(const F_Text& Raw);
        static TF_Optional<F_Text> FindAbsolutePath(const F_Text& Raw, const TF_Vector<F_Text>& ExternalBasePaths);
    };
    
    class ABYTEK_ENGINE_ASSETS_API A_Asset : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(A_Asset);

    private:
        F_Text _AbsolutePath;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetAbsolutePath() const noexcept
        {
            return _AbsolutePath;
        }

    protected:
        A_Asset(const F_Text& AbsolutePath);
        
    public:
        virtual ~A_Asset();

    public:
        virtual TW<F_ReflectionType> GetCookerTypeHandle() const;
    };
}
