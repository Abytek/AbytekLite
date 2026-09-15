
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.EmptyApplication2");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_EmptyModule2"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.EmptyApplication2.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);