
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.RHISamples.ReadbackBuffer");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_RHISampleModule_ReadbackBuffer"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.RHISamples.ReadbackBuffer.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);