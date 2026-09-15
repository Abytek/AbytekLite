
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.MLPDemo");
        APT.Set(
            "AbytekEBT.ApplicationClass",
            "Abytek::F_MLPDemoApplication"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.MLPDemo.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);