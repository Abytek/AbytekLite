
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Demo");
        APT.Set(
            "AbytekEBT.ApplicationClass",
            "Abytek::F_DemoApplication"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Templates.Demo.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);