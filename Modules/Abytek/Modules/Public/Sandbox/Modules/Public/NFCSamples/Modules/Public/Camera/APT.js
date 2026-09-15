
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.NFCSamples.Camera");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_NFCSampleModule_Camera"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.NFCSamples.Camera.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);