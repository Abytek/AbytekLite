#include "Abytek/UUID.hpp"


namespace Abytek
{
    F_Text H_UUID::Generate()
    {
        std::array<uint8_t, 16> bytes{};

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<uint32_t> dist(0, 255);

        for (auto& byte : bytes)
            byte = static_cast<uint8_t>(dist(gen));

        // UUID v4
        bytes[6] = (bytes[6] & 0x0F) | 0x40;
        bytes[8] = (bytes[8] & 0x3F) | 0x80;

        std::ostringstream ss;
        ss << std::hex << std::setfill('0');

        for (size_t i = 0; i < bytes.size(); ++i)
        {
            ss << std::setw(2) << static_cast<int>(bytes[i]);

            if (i == 3 || i == 5 || i == 7 || i == 9)
                ss << '-';
        }

        return ToText(ss.str().c_str());
    }
}
