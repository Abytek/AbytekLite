
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.NFCSamples.SimpleSRP");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_NFCSampleModule_SimpleSRP"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.NFCSamples.SimpleSRP.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);