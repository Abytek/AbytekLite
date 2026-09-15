
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.EmptyApplication");
        APT.Set(
            "AbytekEBT.ApplicationClass",
            "Abytek::F_EmptyApplication"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.EmptyApplication.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);