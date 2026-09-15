
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.RHISamples.ReadbackTexture");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_RHISampleModule_ReadbackTexture"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.RHISamples.ReadbackTexture.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);