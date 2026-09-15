
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.RHISamples.NV12");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_RHISampleModule_NV12"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.RHISamples.NV12.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);