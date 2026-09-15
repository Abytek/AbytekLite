
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.NFCApplication");
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.NFCApplication.hpp"
            ]
        );
        APT.Set(
            "AbytekEBT.ApplicationModuleClasses",
            [
                "Abytek::F_NFCApplicationModule"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);