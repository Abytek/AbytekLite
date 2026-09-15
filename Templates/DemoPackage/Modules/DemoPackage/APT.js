
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "DemoPackage");
        APT.Set(
            "AbytekEBT.ApplicationClass",
            "Abytek::F_DemoPackageApplication"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Templates.DemoPackage.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);