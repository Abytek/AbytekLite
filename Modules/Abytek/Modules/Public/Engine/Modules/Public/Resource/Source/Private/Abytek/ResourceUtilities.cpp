#include "Abytek/ResourceUtilities.hpp"
#include "Abytek/ResourceSubsystem.hpp"
#include "Abytek/RootResource.hpp"


namespace Abytek
{
    namespace Internal
    {
        AU32 RandomResourceCounter = 0;
    }
    F_Name GenerateRandomResourceName()
    {
        return ABYTEK_TEXT("Abytek.RandomResources.") + ToText(Internal::RandomResourceCounter.fetch_add(1, boost::memory_order_release));
    }
}
