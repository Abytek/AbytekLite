#pragma once

#include "Abytek/Engine.Assets.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_ASSETS_API A_AssetCooker : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(A_AssetCooker)

    private:
        
    public:

    protected:
        A_AssetCooker();
        
    public:
        virtual ~A_AssetCooker();

    public:
        virtual B8 Cook();
    };
}
